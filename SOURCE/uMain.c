/*
 * File:   uMain.c
 * Author: ld
 *
 * Created on 2017年4月5日, 下午5:30
 */


#include "xc.h"
// DSPIC33EP256MU806 Configuration Bit Settings

// 'C' source line config statements

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF               // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Initial Oscillator Source Selection bits (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS1024         // Watchdog Timer Postscaler bits (1:1,024)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = OFF              // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // Power-on Reset Timer Value Select bits (128ms)
#pragma config BOREN = ON               // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF            // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF              // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF               // Auxiliary Segment Write-protect bit (Aux Flash may be written)
#pragma config APL = OFF                // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF               // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <stdio.h>
#include "uInit.h"
#include "uDelay.h"
#include "uI2C.h"
#include "uLCD.h"
#include "uSDcard.h"
#include "uModbus.h"
#include "uRS232.h"
#include "uMBcrc.h"
#include "uMBcrc.h"


RUN_PARAMS gRunParams;
//应用
unsigned int getReadPositionInBlock(unsigned int blockNum)
{
	unsigned int i;
	unsigned char flag=(unsigned char)blockNum;
	unsigned char rdData[SEC_PER_BLOCK];
	unsigned long int rdAddr=DATA_RD_FLAG_START_ADDR;
	for(i=0;i<(SEC_PER_BLOCK/E2P_PAGE_SIZE);i++)
	{
		E2P_Read_NByte_Randomaddress(&rdData[i*E2P_PAGE_SIZE],E2P_PAGE_SIZE,rdAddr+(i*E2P_PAGE_SIZE));
	}
	for(i=0;i<SEC_PER_BLOCK;i++)
	{
		if(rdData[i]!=flag)break;
	}
	return i;
}
unsigned int getWritePositionInBlock(unsigned int blockNum)
{
	unsigned int i;
	unsigned char flag=(unsigned char)blockNum;
	unsigned char rdData[SEC_PER_BLOCK];
	unsigned long int rdAddr=DATA_WR_FLAG_START_ADDR;
	for(i=0;i<(SEC_PER_BLOCK/E2P_PAGE_SIZE);i++)
	{
		E2P_Read_NByte_Randomaddress(&rdData[i*E2P_PAGE_SIZE],E2P_PAGE_SIZE,rdAddr+(i*E2P_PAGE_SIZE));
	}
	for(i=0;i<SEC_PER_BLOCK;i++)
	{
		if(rdData[i]!=flag)break;
	}
	return i;	
}

void save_RunParams(void)
{
	STORE_PARAMS storeParams;
	storeParams.rs232_baud=gRunParams.rs232_baud;
	storeParams.rdBlock=gRunParams.rdBlock;
	storeParams.wrBlock=gRunParams.wrBlock;
	storeParams.slaveSaveInv=gRunParams.slaveSaveInv;
	storeParams.autoControl=gRunParams.autoControl;
	storeParams.flag=INIT_FLAG;
	storeParams.crc16=usMBCRC16( (unsigned char *)&storeParams, sizeof(STORE_PARAMS)-sizeof(unsigned int));
	E2P_Write_NByte((unsigned char *)&storeParams,sizeof(STORE_PARAMS),STORE_PARAMS_ADDR);
}
void reset_param_flag(void)
{
	STORE_PARAMS storeParams;
	storeParams.rs232_baud=gRunParams.rs232_baud;
	storeParams.rdBlock=gRunParams.rdBlock;
	storeParams.wrBlock=gRunParams.wrBlock;
	storeParams.slaveSaveInv=gRunParams.slaveSaveInv;
	storeParams.autoControl=gRunParams.autoControl;
	storeParams.breakSection=gRunParams.breakSection;
	storeParams.flag=INIT_FLAG-1;
	storeParams.crc16=usMBCRC16( (unsigned char *)&storeParams, sizeof(STORE_PARAMS)-sizeof(unsigned int));
	E2P_Write_NByte((unsigned char *)&storeParams,sizeof(STORE_PARAMS),STORE_PARAMS_ADDR);
}
void reset_param_read_flag(void)
{
	STORE_PARAMS storeParams;
	unsigned int i;
	unsigned char tempArray[SEC_PER_BLOCK];
	for(i=0;i<SEC_PER_BLOCK;i++)tempArray[i]=0xff;
	for(i=0;i<(SEC_PER_BLOCK/E2P_PAGE_SIZE);i++)
	{
		E2P_Write_NByte(&tempArray[i*E2P_PAGE_SIZE],E2P_PAGE_SIZE,DATA_RD_FLAG_START_ADDR+(i*E2P_PAGE_SIZE));
	}
	storeParams.rs232_baud=gRunParams.rs232_baud;
	storeParams.rdBlock=0;
	storeParams.wrBlock=gRunParams.wrBlock;
	storeParams.slaveSaveInv=gRunParams.slaveSaveInv;
	storeParams.autoControl=gRunParams.autoControl;
	storeParams.breakSection=gRunParams.breakSection;
	storeParams.flag=INIT_FLAG-1;
	storeParams.crc16=usMBCRC16( (unsigned char *)&storeParams, sizeof(STORE_PARAMS)-sizeof(unsigned int));
	E2P_Write_NByte((unsigned char *)&storeParams,sizeof(STORE_PARAMS),STORE_PARAMS_ADDR);
}
void save_OneRecord(void)
{
	unsigned int i,len;
	sSysRecords exSysRecords;
	unsigned int wrAddr;
	unsigned char flag;
	unsigned char *pSrc=(unsigned char *)&gCurSysRecord;
	unsigned char *pDst=(unsigned char *)&exSysRecords;
	len=sizeof(sSysRecords);
	for(i=0;i<len;i++){*pDst++=*pSrc++;}
	exSysRecords.crc16= usMBCRC16( (unsigned char *)&exSysRecords,len-2);
	SD_WriteSingleBlock(gRunParams.wrBlock*SEC_PER_BLOCK+gRunParams.wrSection,(unsigned char * )&exSysRecords);
    //nand_WriteOneRecord(gRunParams.wrBlock,gRunParams.wrSection,&exSysRecords,len);
	
	wrAddr=DATA_WR_FLAG_START_ADDR;wrAddr=wrAddr+gRunParams.wrSection;
	flag=(unsigned char)gRunParams.wrBlock;
        E2P_Write_NByte(&flag,1,wrAddr);
        gRunParams.wrSection++;
	if(gRunParams.wrSection==SEC_PER_BLOCK)
	{
		gRunParams.wrBlock++;
		gRunParams.wrSection=0;
		save_RunParams();
	}
       /*
	if(nand_WriteOneRecord(gRunParams.wrBlock,gRunParams.wrSection,&exSysRecords,len))
	{
		gRunParams.wrSection++;
		if(gRunParams.wrSection==SEC_PER_BLOCK)
		{
			gRunParams.wrBlock++;
			gRunParams.wrSection=0;
			save_RunParams();
		}
		reset_SysRecord();
		
	}
         */
}
unsigned char rdArray[512];
void load_OneRecord(unsigned char *pDst)
{
    unsigned int len,i;
	
	len=sizeof(sSysRecords);
	
	SD_ReadSingleBlock(gRunParams.rdBlock*SEC_PER_BLOCK +gRunParams.rdSection ,rdArray);
	for(i=0;i<len;i++)
		{
		*pDst++=rdArray[i];
		}
	//nand_ReadOneRecord(gRunParams.rdBlock,gRunParams.rdSection,pDst,len);
	
}
void load_SlaveControls(SLAVE_CONTROL *pSlaveControl,unsigned int pos)
{
	E2P_Read_NByte_Randomaddress(pSlaveControl,sizeof(SLAVE_CONTROL),SLAVE_CONTROL_ADDR +pos*16);
}
void save_SlaveControls(SLAVE_CONTROL *pSlaveControl,unsigned int pos)
{
	E2P_Write_NByte(pSlaveControl,sizeof(SLAVE_CONTROL),SLAVE_CONTROL_ADDR +pos*16);
}
void printf_paramInfo(unsigned int row,char *pStr1,unsigned long param)
{
	char str[10];
	LCD_WriteCharRow_16_8(pStr1,row,1,FALSE);
	sprintf(str,"%ld",param);
	LCD_WriteCharRow_16_8(str,row,20,FALSE);
}
void init_StoreParams(void)
{
	STORE_PARAMS StoreParams;
	unsigned int i;
    unsigned int calCRC16;
	unsigned int tempInt;
	unsigned char tempArray[SEC_PER_BLOCK];
	LCD_WriteCharRow_16_8("Now Load RunParams....",4,1,FALSE);
    E2P_Read_NByte_Randomaddress(&StoreParams,sizeof(STORE_PARAMS),STORE_PARAMS_ADDR);
    calCRC16=usMBCRC16( (unsigned char *)&StoreParams, sizeof(STORE_PARAMS)-sizeof(unsigned int));
    if((INIT_FLAG!=StoreParams.flag)||(calCRC16!=StoreParams.crc16))//校验不正确，重新初始化
    {
		LCD_WriteCharRow_16_8("load RunParams error / Running in first time",20,1,FALSE);
		StoreParams.rs232_baud=115200;	
		StoreParams.rdBlock=0;
		StoreParams.wrBlock=0;
		StoreParams.slaveSaveInv=3;
		StoreParams.autoControl=0;
		StoreParams.breakSection=0;
		StoreParams.flag=INIT_FLAG;
		StoreParams.crc16=usMBCRC16( (unsigned char *)&StoreParams, sizeof(STORE_PARAMS)-sizeof(unsigned int));
		E2P_Write_NByte((unsigned char *)&StoreParams,sizeof(STORE_PARAMS),STORE_PARAMS_ADDR);
		for(i=0;i<SEC_PER_BLOCK;i++)tempArray[i]=0xff;
		for(i=0;i<(SEC_PER_BLOCK/E2P_PAGE_SIZE);i++)
		{
			E2P_Write_NByte(&tempArray[i*E2P_PAGE_SIZE],E2P_PAGE_SIZE,DATA_WR_FLAG_START_ADDR+(i*E2P_PAGE_SIZE));
			E2P_Write_NByte(&tempArray[i*E2P_PAGE_SIZE],E2P_PAGE_SIZE,DATA_RD_FLAG_START_ADDR+(i*E2P_PAGE_SIZE));
		}
		gRunParams.rdSection=0;
		gRunParams.wrSection=0;
		for(i=0;i<SLAVE_NUM;i++)
		{
			gRunParams.slaveControls[i].light_off_time=60;
			gRunParams.slaveControls[i].light_on_time=60;
			gRunParams.slaveControls[i].loopTime=1;
			gRunParams.slaveControls[i].startTimeSec=0xffffffff;	
		}
		
    }
	else
	{
		tempInt=getReadPositionInBlock(gRunParams.rdBlock);
		if(tempInt>=SEC_PER_BLOCK)
		{
			gRunParams.rdBlock=gRunParams.rdBlock+1;gRunParams.rdSection=0;
		}
		else
		{
			gRunParams.rdSection=tempInt;
		}
		tempInt=getWritePositionInBlock(gRunParams.wrBlock);
		if(tempInt>=SEC_PER_BLOCK)
		{
			gRunParams.wrBlock=gRunParams.wrBlock+1;gRunParams.wrSection=0;
		}
		else
		{
			gRunParams.wrSection=tempInt;
		}
		for(i=0;i<SLAVE_NUM;i++)
		{
			load_SlaveControls(&gRunParams.slaveControls[i],i);
			Delay_ms(10);
		}
	}
	gRunParams.rs232_baud=StoreParams.rs232_baud;
	gRunParams.rdBlock=StoreParams.rdBlock;
	gRunParams.wrBlock=StoreParams.wrBlock;
	gRunParams.slaveSaveInv=StoreParams.slaveSaveInv;
	gRunParams.autoControl=StoreParams.autoControl;
	gRunParams.breakSection=StoreParams.breakSection;
	printf_paramInfo(36,"rs232_baud:",gRunParams.rs232_baud);		
	printf_paramInfo(52,"rdBlock:",gRunParams.rdBlock);	
	printf_paramInfo(68,"wrBlock:",gRunParams.wrBlock);
	printf_paramInfo(84,"slaveSaveInv:",gRunParams.slaveSaveInv);
	printf_paramInfo(100,"autoControl:",gRunParams.autoControl);
	printf_paramInfo(116,"rdSection:",gRunParams.rdSection);
	printf_paramInfo(132,"wrSection:",gRunParams.wrSection);
	printf_paramInfo(148,"breakSection:",gRunParams.breakSection);
    
}
void save_AllParams(void)
{
	int i;
	save_RunParams();
	for(i=0;i<SLAVE_NUM;i++)
	{
		save_SlaveControls(&gRunParams.slaveControls[i],i);
		Delay_ms(5);
	}
}

void start_system(void)
{
	GetTime_8563();
	init_Display();
	start_NewCircle();
}
int main(void) {
    int i;
   /* for(i=0;i<256;i++)
    {
        txxxx[i]=0x55;
        txxxx[i+256]=i;
    }*/
    init_OSC();
    init_Timer1();
    init_I2C();
    init_LCD();
	init_sd();
	init_Modbus(9600);
    init_StoreParams();
	
	Delay_ms(5000);
	start_system();
	Delay_ms(1000);    
        //nand_RandomWriteData(0x08200000,2,&txxxx[0]);
        //nand_EraseBlock(2024);
        //nand_RandomReadData(0x0FC00000,128,tstRdDatas);
        init_RS232(gRunParams.rs232_baud);
    while(1)
    {
        rs232_poll();
        modbusPoll();
        if(getIndTaskRun(IND_RTC_REFRESH))
        {
            GetTime_8563();
        }
        rs232_poll();
        modbusPoll();
        if(getIndTaskRun(IND_DIS_REFRESH))
        {
            dis_refresh();
        }
        rs232_poll();
        modbusPoll();
	if(getIndTaskRun(IND_CAMP_TIME))
		{
			setNewLightsState();
		}
       rs232_poll();
        modbusPoll();
		if(getIndTaskRun(IND_SAVE_RECORD))
		{
			save_OneRecord();
			start_NewCircle();
		}
		
        //if(getRS232State()==RS232_FRAME_RECIEVED)
        //{
        //    recv_RS232(txxxx);
       // send_RS232(512);
       // }
    }

    //LCD_WriteCharRow_8_8("ABCDEFGH",4,1,0);
   // nand_RandomReadData(4097,16,tstRdDatas);
    /*
    while(1)
    {
        Delay_ms(500);
        if(nand_RandomReadData(0x00000000,16,tstRdDatas)!=0)
            LCD_WriteCharRow_8_8("read sucess!",16,1,0);
        else
            LCD_WriteCharRow_8_8("read failed!",16,1,0);
        LATDbits.LATD10=!LATDbits.LATD10;
    }
 */
    return 0;
}


