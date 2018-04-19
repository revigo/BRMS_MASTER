/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <xc.h>
#include <stdint.h>
#include <uart.h>
#include <Generic.h>

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
static char U1TXbuffer[128];
static uint64_t sendTickCnt;
static uint16_t printInfoCnt;
static BOOL isPrinting=FALSE;
void addPrintInfoCnt(void)
{
	printInfoCnt +=1;
}
void clearPrintInfoCnt(void)
{
	printInfoCnt =0;
}

void InitUART1( void )
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
    U1MODEbits.PDSEL = 0;       // Bits1,2 8bit, No Parity
    U1MODEbits.STSEL = 0;       // Bit0 One Stop Bit

    // Load a value into Baud Rate Generator.  Example is for 9600.
    // See section 19.3.1 of datasheet.
    //  U1BRG = (Fcy/(16*BaudRate))-1
    //  U1BRG = (37M/(16*9600))-1
    // U1BRG = 240;
    U1BRG = 383;                // 58982400hz osc, 9600 Baud

    // Load all values in for U1STA SFR
    U1STAbits.UTXISEL1 = 0;     //Bit15 Int when Char is transferred (1/2 config!)
    U1STAbits.UTXINV = 0;       //Bit14 N/A, IRDA config
    U1STAbits.UTXISEL0 = 0;     //Bit13 Other half of Bit15

    //U1STAbits.notimplemented = 0;//Bit12
    U1STAbits.UTXBRK = 0;       //Bit11 Disabled
    U1STAbits.UTXEN = 0;        //Bit10 TX pins controlled by periph
    U1STAbits.UTXBF = 0;        //Bit9 *Read Only Bit*
    U1STAbits.TRMT = 0;         //Bit8 *Read Only bit*
    U1STAbits.URXISEL = 0;      //Bits6,7 Int. on character recieved
    U1STAbits.ADDEN = 0;        //Bit5 Address Detect Disabled
    U1STAbits.RIDLE = 0;        //Bit4 *Read Only Bit*
    U1STAbits.PERR = 0;         //Bit3 *Read Only Bit*
    U1STAbits.FERR = 0;         //Bit2 *Read Only Bit*
    U1STAbits.OERR = 0;         //Bit1 *Read Only Bit*
    U1STAbits.URXDA = 0;        //Bit0 *Read Only Bit*

    IFS0bits.U1TXIF = 0;        // Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0;        // Enable Transmit Interrupts
    IFS0bits.U1RXIF = 0;        // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 0;        // Enable Recieve Interrupts

    U1MODEbits.UARTEN = 1;      // And turn the peripheral on
    U1STAbits.UTXEN = 1;
    IFS0bits.U1TXIF = 0;
}
void Uart1DMAInit(void)
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


BOOL Uart1Error(void)
{
	return (U1STAbits.PERR | U1STAbits.FERR | U1STAbits.OERR);
}
void waitPrinterIdle(void)
{
	while(isPrinting){}
}

void WriteByteToPrinter(char data)
{
	while(U1STAbits.UTXBF){}
	U1TXREG = data;
}
void sendDataToPrinter(char *buffer,unsigned int len)
{
	#if 1
	unsigned int Length = (len > 128)?128:len;
	unsigned int reqDMA1Cnt;
	//waitPrinterIdle();
	if(U1STAbits.TRMT == 1)
	{
		memcpy(U1TXbuffer,buffer,Length);
		reqDMA1Cnt= Length -1;
		U1TXbuffer[len]=0x1b;reqDMA1Cnt++;
		U1TXbuffer[len+1]=0x64;reqDMA1Cnt++;
		U1TXbuffer[len+2]=0x03;reqDMA1Cnt++;
		DMA1CNT = reqDMA1Cnt;
        DMA1CONbits.CHEN = 1; // Enable DMA channel
        DMA1REQbits.FORCE = 1;
	}

	#else
	unsigned int i;
	//if(Uart1Error())
	//{
	//	InitUART1();
	//	Delay1ms();
	//}

	for(i=0;i<len;i++)
	{
		waitPrinterIdle();
		WriteByteToPrinter(buffer[i]);
	}
	Delay10ms();
	waitPrinterIdle();
	WriteByteToPrinter(PRINTER_LF);
	WriteByteToPrinter(PRINTER_CR);
	//Delay100ms();Delay100ms();Delay100ms();Delay100ms();Delay100ms();
	#endif

}
void resetPrinter(void)
{
	WriteByteToPrinter(0x1B);
	WriteByteToPrinter(0x40);
}
void printerModeCHN(void)
{
	WriteByteToPrinter(0x1C);
	WriteByteToPrinter(0x26);
}
void printerModeENG(void)
{
	WriteByteToPrinter(0x1C);
	WriteByteToPrinter(0x2E);
}

void InitPrinter( void )
{
	Uart1DMAInit();
    InitUART1();
	Delay_ms(5);
	Delay_ms(5);
	//配置打印机接口
	TRISDbits.TRISD11= 1;TRISDbits.TRISD0 =0;
	_RP64R = 0X01;
	_U1RXR = 75;
	sendTickCnt=0;
	isPrinting=FALSE;
	//resetPrinter();resetPrinter();resetPrinter();
}


void __attribute__((__interrupt__,no_auto_psv)) _DMA1Interrupt(void)
{
   	IFS0bits.DMA1IF = 0; // Clear the DMA0 Interrupt Flag
   	U3STAbits.UTXEN = 1;
	//sendTickCnt=getSystemTick();
	isPrinting = FALSE;
}

