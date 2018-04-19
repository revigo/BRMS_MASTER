/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <xc.h>
#include <stdint.h>
#include <uart.h>
#include <Generic.h>
#include "uDelay.h"
#include "uRS232.h"
#include "uModbus.h"
#include "uInit.h"
#include "uNand.h"
#include "uMBcrc.h"
#define PRINTER_CR				0x0D
#define PRINTER_LF				0x0A
/******************************************************************************
 * Function:        void InitUART1()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function configures UART module
 *****************************************************************************/
#define TICKS_PER_SEC		1000L
static unsigned char U1TXbuffer[512];
static unsigned char U1RXbuffer[768];
static unsigned char U1RXbuffer_BK[768];
static unsigned int U1RXcnt;
static RS232_STATE rs232State;
static uint32_t usT35TimeOut1ms;
static unsigned long rs232Baudrate;
static CommCommand rs232Cmd;
void setRS232State(RS232_STATE stat)
{
    rs232State=stat;
}
RS232_STATE getRS232State(void)
{
    return rs232State;
}
void init_Timer45(void)
{

        T5CONbits.TON = 0; // Stop any 16-bit Timer3 operation
	T4CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
	T4CONbits.T32 = 1; // Enable 32-bit Timer mode
	T4CONbits.TCS = 0; // Select internal instruction cycle clock
	T4CONbits.TGATE = 0; // Disable Gated Timer mode
	T4CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR5 = 0x00; // Clear 32-bit Timer (msw)
	TMR4 = 0x00; // Clear 32-bit Timer (lsw)
	PR5 = 0x0000; // Load 32-bit period value (msw)
	PR4 = 0xffff; // Load 32-bit period value (lsw)
	IPC7bits.T5IP = 0x05; // Set Timer3 Interrupt Priority Level
	IFS1bits.T5IF = 0; // Clear Timer3 Interrupt Flag
	IEC1bits.T5IE = 1; // Enable Timer3 interrupt
	T4CONbits.TON = 0; // Start 32-bit Timer

        //prescaler = 1;
        usT35TimeOut1ms = 58982400 /1000;
}

void Timer45Enable(uint32_t inv)
{
	TMR4 = 0x0000;
	TMR5 = 0x0000;
	PR5 = (uint16_t)(inv>>16);
    PR4 = (uint16_t)inv;       // Timer1 period register = ?????
    T4CONbits.TON = 1;  // Enable Timer1 and start the counter
}
void Timers45T35Enable(void )
{
	uint32_t timer_tick = (7L* TICKS_PER_SEC * 10L * usT35TimeOut1ms)/(rs232Baudrate*2L);
	Timer45Enable(timer_tick);
}
static void init_UART1( unsigned long usbaudrate )
{
	    // This is an EXAMPLE, so brutal typing goes into explaining all bit sets
    // The HPC16 board has a DB9 connector wired to UART2, so we will
    // be configuring this port only
    // configure U1MODE
    U1MODEbits.UARTEN = 0;      // Bit15 TX, RX DISABLED, ENABLE at end of func

    //U1MODEbits.notimplemented;// Bit14
    U1MODEbits.USIDL = 0;       // Bit13 Continue in Idle
    U1MODEbits.IREN = 0;        // Bit12 No IR translation
    U1MODEbits.RTSMD = 0;       // Bit11 Simplex Mode
    
    //U1MODEbits.notimplemented;// Bit10
    U1MODEbits.UEN = 0;         // Bits8,9 TX,RX enabled, CTS,RTS not
    U1MODEbits.WAKE = 0;        // Bit7 No Wake up (since we don't sleep here)
    U1MODEbits.LPBACK = 0;      // Bit6 No Loop Back
    U1MODEbits.ABAUD = 0;       // Bit5 No Autobaud (would require sending '55')
    U1MODEbits.BRGH = 0;        // Bit3 16 clocks per bit period
    U1MODEbits.URXINV=0;
    U1MODEbits.PDSEL = 0;       // Bits1,2 8bit, No Parity
    U1MODEbits.STSEL = 0;       // Bit0 One Stop Bit

    // Load all values in for U1STA SFR
    U1STAbits.UTXISEL1 = 0;     //Bit15 Int when Char is transferred (1/2 config!)
    U1STAbits.UTXISEL0 = 0;     //Bit13 Other half of Bit15
    U1STAbits.UTXINV = 0;       //Bit14 N/A, IRDA config
    U1STAbits.UTXBRK = 0;       //Bit11 Disabled
    U1STAbits.UTXEN = 0;        //Bit10 TX pins controlled by periph
    U1STAbits.UTXBF = 0;        //Bit9 *Read Only Bit*
    U1STAbits.TRMT = 1;         //Bit8 *Read Only bit*
    U1STAbits.URXISEL = 0;      //Bits6,7 Int. on character recieved
    U1STAbits.ADDEN = 0;        //Bit5 Address Detect Disabled
    U1STAbits.RIDLE = 0;        //Bit4 *Read Only Bit*
    U1STAbits.PERR = 0;         //Bit3 *Read Only Bit*
    U1STAbits.FERR = 0;         //Bit2 *Read Only Bit*
    U1STAbits.OERR = 0;         //Bit1 *Read Only Bit*
    U1STAbits.URXDA = 0;        //Bit0 *Read Only Bit*

    IPC3bits.U1TXIP=4;
    IFS0bits.U1TXIF = 0;        // Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0;        // Enable Transmit Interrupts
    IPC2bits.U1RXIP=4;
    IFS0bits.U1RXIF = 0;        // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 1;        // Enable Recieve Interrupts

    // Load a value into Baud Rate Generator.  Example is for 9600.
    // See section 19.3.1 of datasheet.
    //  U1BRG = (Fcy/(16*BaudRate))-1
    //  U1BRG = (37M/(16*9600))-1
    // U1BRG = 240;
    U1BRG = 58982400/16/usbaudrate-1;//383;                // 58982400hz osc, 9600 Baud

    U1MODEbits.UARTEN = 1;      // And turn the peripheral on
    U1STAbits.UTXEN = 1;
    IFS0bits.U1TXIF = 0;
}
static void init_DMAs(void)
{
	//发送DMA
	DMA1CON = 0x6001; // One-Shot, Post-Increment, RAM-to-Peripheral
	DMA1CNT = 0; // Eight DMA requests
	DMA1REQ = 0x000C; // Select UART1 TX transmitter
	DMA1PAD = (volatile unsigned int) &U1TXREG;
	DMA1STAL = __builtin_dmaoffset(U1TXbuffer);
	DMA1STAH = 0x0000;
	IFS0bits.DMA1IF = 0; // Clear DMA Interrupt Flag
	IPC3bits.DMA1IP = 4;
	IEC0bits.DMA1IE = 1; // Enable DMA interrupt
}
void send_RS232(unsigned int len)
{
    //char *pTx=U1TXbuffer;
    if(U1STAbits.TRMT == 1)
    {
        DMA1CNT=len-1;
        //while(len!=0)
        //{
        //    *pTx++=*buffer++;
        //    len--;
        //}
        DMA1CONbits.CHEN = 1; // Enable DMA channel
        DMA1REQbits.FORCE = 1;
    }
}
unsigned int recv_RS232(unsigned char *buffer)
{
    unsigned int result=0;
    unsigned char *pRx=U1RXbuffer;
    while(U1RXcnt!=0)
    {
        *buffer++=*pRx++;
        U1RXcnt--;
        result++;
    }
    rs232State=RS232_IDLE;
    return result;
}
void init_RS232( unsigned long usbaudrate )
{
    TRISDbits.TRISD11= 1;TRISDbits.TRISD0 =0;
	_RP64R = 0X01;
	_U1RXR = 75;
        U1STAbits.OERR=0;
    rs232Baudrate=usbaudrate;
    rs232State=RS232_IDLE;
	init_DMAs();
        init_Timer45();
        init_UART1(usbaudrate);
	//Delay_ms(5);
	//Delay_ms(5);
	//配置端口
	
}
void rs232_response_Lights_Mannual(unsigned int ctl)
{
	unsigned char res=0;
	int i;
	res=(ctl==17)?0:1;
	for(i=0;i<SLAVE_NUM;i++)
	{
		gCurSysRecord.slaveLight[i]=res;
		slaveNewLight[i]=res;
	}
	gRunParams.autoControl=0;//静止自动点灯
	/*
	if(ctl==17)
	{
		set_AllLightsSwitch(0);
	}
	else if(ctl==18)
	{
		set_AllLightsSwitch(1);
	}
	*/
	rs232Cmd.ucAddr =0;
	rs232Cmd.Funcs = MB_FUNC_WRITE_SINGLE_COIL;
	rs232Cmd.usRegAddr = 0x0000;
	if((ctl==17)||(ctl==18))
	{
		
		rs232Cmd.usRegData= (ctl==17)?0xFF00:0x0000;
		res=1;
			
	}
	rs232Cmd.reqCmdState = REQ_STATE_PREPARE;
	modbusReqestCommunicate(&rs232Cmd);
	if(res==1)
	{
		for(i=0;i<6;i++)U1TXbuffer[i]=U1RXbuffer_BK[i];
		send_RS232(6);
	}
	
}
void send_rs232_response_Lights_Mannual(unsigned int regdata)
{
	if(regdata==0xff00)
	{
		U1TXbuffer[0]=0x11;
		U1TXbuffer[1]=0x00;
		U1TXbuffer[2]=0x06;
		U1TXbuffer[3]=0x00;
		U1TXbuffer[4]=0xb8;
		U1TXbuffer[5]=0x06;
	}
	else
	{
		U1TXbuffer[0]=0x12;
		U1TXbuffer[1]=0x00;
		U1TXbuffer[2]=0x06;
		U1TXbuffer[3]=0x00;
		U1TXbuffer[4]=0xfc;
		U1TXbuffer[5]=0x06;
	}
	gRunParams.autoControl=0;
	send_RS232(6);
}
void trans_RecordToNetList(unsigned char *pNet,sSysRecords * pRecord)
{
	int i;
	*pNet++=pRecord->loopTime;
	for(i=0;i<7;i++)
	{
	*pNet++=pRecord->timeBCD[i];
	}
	for(i=0;i<SLAVE_NUM;i++)
	{
		*pNet++=(unsigned char )(pRecord->slavePulse[i]&0x00ff);
		*pNet++=(unsigned char )((pRecord->slavePulse[i]>>8)&0x00ff);
	}
	for(i=0;i<SLAVE_NUM;i++)
	{
		*pNet++=pRecord->slaveLight[i];
	}
}
extern void load_OneRecord(void *pDst);
extern void save_RunParams(void);
void send_his_data_success(void)
{
	unsigned long rdAddr;
	unsigned char flag;
        unsigned long rdRecordCnt,wrRecordCnt;
        rdRecordCnt=gRunParams.rdBlock*SEC_PER_BLOCK+gRunParams.rdSection;
	wrRecordCnt=gRunParams.wrBlock*SEC_PER_BLOCK+gRunParams.wrSection;
        if(rdRecordCnt<wrRecordCnt)
        {
            rdAddr=DATA_RD_FLAG_START_ADDR;rdAddr=rdAddr+gRunParams.rdSection;
			flag=(unsigned char)gRunParams.rdBlock;
            E2P_Write_NByte(&flag,1,rdAddr);
            gRunParams.rdSection++;
            if(gRunParams.rdSection==SEC_PER_BLOCK)
            {
                    gRunParams.rdBlock++;
                    gRunParams.rdSection=0;
                    save_RunParams();
            }
     }
}
void rs232_response_his_data(void)
	{
		int pos=0;
		unsigned long rdRecordCnt,wrRecordCnt;
		unsigned int crc16;
		unsigned int totalNum=134;
		sSysRecords lSysRecord;
		rdRecordCnt=gRunParams.rdBlock*SEC_PER_BLOCK+gRunParams.rdSection;
		wrRecordCnt=gRunParams.wrBlock*SEC_PER_BLOCK+gRunParams.wrSection;
		if(rdRecordCnt<wrRecordCnt)
		{
			load_OneRecord(&lSysRecord);
			
			U1TXbuffer[pos++]=0x0C;//12
			U1TXbuffer[pos++]=0x00;
			U1TXbuffer[pos++]=totalNum;
			U1TXbuffer[pos++]=0x00;
			trans_RecordToNetList(&U1TXbuffer[pos],&lSysRecord);
			crc16=usMBCRC16(U1TXbuffer,totalNum-2);
			U1TXbuffer[totalNum-1]=(unsigned char )((crc16>>8)&0x00ff);
			U1TXbuffer[totalNum-2]=(unsigned char )(crc16&0x00ff);
			send_RS232(totalNum);
			/*Delay_ms(1);
			crc16=usMBCRC16((unsigned char *)&lSysRecord,sizeof(lSysRecord)-2);
			if(crc16!=lSysRecord.crc16)
			{
				gRunParams.breakSection=(gRunParams.breakSection>10000)?10000:gRunParams.breakSection++;
				save_RunParams();
			}*/
		}
		else
		{
			U1TXbuffer[0]=0x20;
			U1TXbuffer[1]=0x00;
			U1TXbuffer[2]=0x06;
			U1TXbuffer[3]=0x00;
			U1TXbuffer[4]=0x3c;
			U1TXbuffer[5]=0x0d;
			send_RS232(6);
		}
	}
void send_response_reset(void)
{
    U1TXbuffer[0]=0x55;
			U1TXbuffer[1]=0x00;
			U1TXbuffer[2]=0x06;
			U1TXbuffer[3]=0x00;
			U1TXbuffer[4]=0x7e;
			U1TXbuffer[5]=0x10;
			send_RS232(6);
}
void rs232_response_cur_data(void)
	{
		int pos=0;
		unsigned int crc16;
		unsigned int totalNum=134;
		U1TXbuffer[pos++]=0x0B;//11
		U1TXbuffer[pos++]=0x00;
		U1TXbuffer[pos++]=totalNum;
		U1TXbuffer[pos++]=0x00;
		trans_RecordToNetList(&U1TXbuffer[pos],&gCurSysRecord);
		crc16=usMBCRC16(U1TXbuffer,totalNum-2);
		U1TXbuffer[totalNum-1]=(unsigned char )((crc16>>8)&0x00ff);
		U1TXbuffer[totalNum-2]=(unsigned char )(crc16&0x00ff);
		send_RS232(totalNum);
	}
void rs232_response_all_data(void)
	{}

#define RS232_CONTROL_VALID_POS		4
#define RS232_CONTROL_CHNS_STRUCT_LEN		13
#define RS232_CONTROL_CHNS_STRUCT_POS	7

extern void save_AllParams(void);
void rs232_response_control(void)
{
	unsigned int pos;
	unsigned int chnNum;
	unsigned int tempL,tempH,temp;
	int i;
	tempL=U1RXbuffer_BK[RS232_CONTROL_VALID_POS]&0x00ff;
	tempH=U1RXbuffer_BK[RS232_CONTROL_VALID_POS+1]&0x00ff;
	chnNum=(tempH<<8)|tempL;
	temp=U1RXbuffer_BK[RS232_CONTROL_VALID_POS+2];
	gRunParams.slaveSaveInv=temp;
	i=0;
	while(chnNum!=0)
	{
		pos=RS232_CONTROL_CHNS_STRUCT_POS + i* RS232_CONTROL_CHNS_STRUCT_LEN;
		tempL=U1RXbuffer_BK[pos++]&0x00ff;
		tempH=U1RXbuffer_BK[pos++]&0x00ff;
		temp=(tempH<<8)|tempL;		
		gRunParams.slaveControls[i].light_on_time=temp;
		tempL=U1RXbuffer_BK[pos++]&0x00ff;
		tempH=U1RXbuffer_BK[pos++]&0x00ff;
		temp=(tempH<<8)|tempL;		
		gRunParams.slaveControls[i].light_off_time=temp;
		gRunParams.slaveControls[i].startTimeSec= get_mktimeBCDArray(&U1RXbuffer_BK[pos]);pos=pos+7;
		tempL=U1RXbuffer_BK[pos++]&0x00ff;
		tempH=U1RXbuffer_BK[pos++]&0x00ff;
		temp=(tempH<<8)|tempL;		
		gRunParams.slaveControls[i].loopTime=temp;
		i++;
		chnNum--;
	}
	gRunParams.autoControl=1;
	save_AllParams();
	U1TXbuffer[0]=0x0d;
	U1TXbuffer[1]=0x00;
	U1TXbuffer[2]=0x06;
	U1TXbuffer[3]=0x00;
	U1TXbuffer[4]=0x28;
	U1TXbuffer[5]=0x01;
	send_RS232(6);
}
void rs232_response_systime(void)
{ 
	PCF8563_Buf.Year=U1RXbuffer_BK[5];
	PCF8563_Buf.Month=U1RXbuffer_BK[6];
	PCF8563_Buf.Day=U1RXbuffer_BK[7];
	PCF8563_Buf.Hour=U1RXbuffer_BK[8];
	PCF8563_Buf.Minute=U1RXbuffer_BK[9];
	PCF8563_Buf.Second=U1RXbuffer_BK[10];
	SetTime_8563();
	U1TXbuffer[0]=0x0e;
	U1TXbuffer[1]=0x00;
	U1TXbuffer[2]=0x06;
	U1TXbuffer[3]=0x00;
	U1TXbuffer[4]=0x6c;
	U1TXbuffer[5]=0x01;
	send_RS232(6);
}
extern void reset_param_flag(void);
void rs232_poll(void)
{
	unsigned int tempInt,rxCount,rxCrc,rxCrc2;
	if(rs232State==RS232_FRAME_RECIEVED)//接收完成
	{
		rxCount=recv_RS232(U1RXbuffer_BK);
		tempInt=U1RXbuffer_BK[3];tempInt=tempInt*256+U1RXbuffer_BK[2];
		if((rxCount>=6)&&(tempInt==rxCount))//接收数据量正确
		{
			rxCrc=U1RXbuffer_BK[rxCount-1];
                        rxCrc=rxCrc<<8;
                        rxCrc2=U1RXbuffer_BK[rxCount-2];
                        rxCrc=rxCrc|(rxCrc2&0x00FF);
			tempInt=usMBCRC16(U1RXbuffer_BK,rxCount-2);
			if(rxCrc==tempInt)//校验正确
			{
				tempInt=U1RXbuffer_BK[1];tempInt=tempInt*256+U1RXbuffer_BK[0];
				switch(tempInt)
				{
					case 18:
					case 17:rs232_response_Lights_Mannual(tempInt);break;
					case 12:rs232_response_his_data();break;
					//case 11:rs232_response_cur_data();break;
					case 10:rs232_response_all_data();break;
					case 22:send_his_data_success();rs232_response_his_data();break;
					case 13:rs232_response_control();break;
					case 14:rs232_response_systime();break;
                                    case 85:reset_param_flag();send_response_reset();break;
									case 86:reset_param_read_flag();send_response_reset();break;
					default:break;
				}
			}
		}
	}
}

void __attribute__((__interrupt__,no_auto_psv)) _DMA1Interrupt(void)
{
   	IFS0bits.DMA1IF = 0; // Clear the DMA0 Interrupt Flag
   	//U3STAbits.UTXEN = 1;
        U1STAbits.OERR=0;
}

void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void)
{
   	IFS0bits.U1RXIF = 0;
        
        U1RXbuffer[U1RXcnt]=U1RXREG;
        U1RXcnt=(U1RXcnt+1)%768;
        rs232State=RS232_FRAME_RECIEVING;
	    U1STAbits.OERR=0;
   	Timers45T35Enable();
}
void __attribute__ ( (interrupt, no_auto_psv) ) _T5Interrupt( void )
{
	IFS1bits.T5IF = 0;
    T4CONbits.TON = 0;
    U1STAbits.OERR=0;
    if(rs232State==RS232_FRAME_RECIEVING)
    {
        rs232State=RS232_FRAME_RECIEVED;
    }
}
