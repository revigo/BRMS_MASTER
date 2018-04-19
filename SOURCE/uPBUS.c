/*
 * FreeModbus Libary: STM32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */
#include <xc.h>
#include <uart.h>
#include <dma.h>
#include <string.h>
#include "uMBcrc.h"
#include "uModbus.h"

#ifndef FCY
#define FCY                     58982400
#endif

#define U3_TRIS_RX		TRISDbits.TRISD8
#define U3_DATA_RX		PORTDbits.RD8
#define U3_TRIS_TX		TRISFbits.TRISF3
#define U3_DATA_TX		PORTFbits.RF3
#define U3_TRIS_DE		TRISDbits.TRISD9
#define U3_DATA_DE		PORTDbits.RD9

#define U3_RPINR_RX_REG        _U3RXR
#define U3_RPINR_RX_DAT        72

#define U3_RPOR_TX_REG        _RP99R
#define U3_RPOR_TX_DAT        0b011011


/*! \brief If master send a broadcast frame,the master will wait time of convert to delay,
 * then master can send other frame */
#define MB_MASTER_DELAY_MS_CONVERT              (200L )
/*! \brief If master send a frame which is not broadcast,the master will wait sometime for slave.
 * And if slave is not respond in this time,the master will process this timeout error.
 * Then master can send other frame */
#define MB_MASTER_TIMEOUT_MS_RESPOND            (100L )

#define TICKS_PER_SEC		1000L
#define COMM_BUFF_LEN               64

/* ----------------------- 局部变量 ----------------------------------------*/
static unsigned long int usT35TimeOut1ms;
unsigned int baudrate;
static unsigned char U3TxBuffer[COMM_BUFF_LEN];//={0x55,0x55,0x55,0x55,0x55,0x55};
static unsigned char U3RxBuffer[COMM_BUFF_LEN];
static unsigned int u3RxBuffLen =0 ;
static eMBMasterEventType eMasterEvent;

//函数声明
void PowerbusTimersInit(void);
void PowerbusEnable(unsigned char xRxEnable, unsigned char xTxEnable);
void PowerbusTimersT35Enable(void);
void PowerbusClose(void);
void PowerbusTimersDisable(void);
////
void PowerbusPortsInit(void)
{
    
	U3_DATA_DE = 1;
	
	U3_TRIS_TX = 0;
	U3_TRIS_DE = 1;
	U3_TRIS_RX = 1;

    U3_RPINR_RX_REG = U3_RPINR_RX_DAT;
    U3_RPOR_TX_REG = U3_RPOR_TX_DAT;
    U3_DATA_TX=1;
}

void PowerbusDMAInit(void)
{
	//发送DMA
	DMA0CON = 0x6001; // One-Shot, Post-Increment, RAM-to-Peripheral
	DMA0CNT = 7; // Eight DMA requests
	DMA0REQ = 0x0053; // Select UART2 transmitter
	DMA0PAD = (volatile unsigned int) &U3TXREG;
	DMA0STAL = __builtin_dmaoffset(U3TxBuffer);
	DMA0STAH = 0x0000;
	IFS0bits.DMA0IF = 0; // Clear DMA Interrupt Flag
	IPC1bits.DMA0IP = 5;
	IEC0bits.DMA0IE = 1; // Enable DMA interrupt
}


/* ----------------------- Start implementation -----------------------------*/
unsigned char init_PBUS( unsigned long int ulBaudRate )
{
	baudrate = ulBaudRate;
    U3MODE = 0x0000;
	PowerbusPortsInit();
	PowerbusDMAInit();
    U3STA = 0x0000;
	U3STAbits.UTXISEL0= 1;
    U3STAbits.UTXISEL1 = 0;
	//U3STAbits.UTXINV =1;
        //U3MODEbits.URXINV =1;
    U3BRG = ((FCY/ulBaudRate)/16)-1;                // 60Mhz osc, 9600 Baud
    IFS5bits.U3TXIF = 0;        // Clear the Transmit Interrupt Flag
    IEC5bits.U3TXIE = 0;        // Enable Transmit Interrupts
    IFS5bits.U3RXIF = 0;        // Clear the Recieve Interrupt Flag
    IPC20bits.U3RXIP= 5;
    IEC5bits.U3RXIE = 1;        // Enable Recieve Interrupts
    U3MODEbits.UARTEN = 1;
	U3STAbits.UTXEN = 1;
    eMasterEvent = EV_MASTER_IDLE;
    PowerbusTimersInit();
    PowerbusEnable(1,0);
    return 1;
}

void PowerbusEnable(unsigned char xRxEnable, unsigned char xTxEnable)
{
    if(xTxEnable)
    {
    //使能发送完成中断
    //IFS5bits.U3TXIF = 0;        // Clear the Transmit Interrupt Flag
    //IEC5bits.U3TXIE = 1;        // Enable Transmit Interrupts
    U3STAbits.UTXEN = 1;
    }
    else
    {
        //禁止发送完成中断
        //IEC5bits.U3TXIE = 0;        // Disable Transmit Interrupts
        //IFS5bits.U3TXIF = 0;        // Clear the Transmit Interrupt Flag
        U3STAbits.UTXEN = 0;
    }

}
void PowerbusClose(void)
{
	PowerbusTimersDisable();
	U3MODEbits.UARTEN = 0;
	U3STAbits.UTXEN = 0; 
}
unsigned char isPowerbusShortcircuited(void)
{
	return (U3_DATA_DE==1);
}

unsigned char PowerbusPutBytes(unsigned int count)
{
	/* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    unsigned int Length = (count > COMM_BUFF_LEN)?COMM_BUFF_LEN:count;
    if(eMasterEvent== EV_MASTER_IDLE)
    { 
        PowerbusEnable(0,1);
        //memcpy(U3TxBuffer,wrData,Length);
        DMA0CNT = Length; // Eight DMA requests
        DMA0CONbits.CHEN = 1; // Enable DMA channel
        DMA0REQbits.FORCE = 1;
		eMasterEvent=(EV_MASTER_FRAME_SENDING);
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char PowerbusGetBytes(unsigned char * pucBytes,unsigned int *count)
{
    if(u3RxBuffLen>0 && eMasterEvent == EV_MASTER_FRAME_RECIEVED)
    {
        memcpy(pucBytes,U3RxBuffer,u3RxBuffLen);
		*count = u3RxBuffLen;
		eMasterEvent =EV_MASTER_IDLE;
		return 1;
    }
	else
	{
    	return 0;
	}
}



/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
	PowerbusTimersDisable();
	if(eMasterEvent == EV_MASTER_FRAME_WAIT_RESPOND)
	{
		u3RxBuffLen = 0;
	}
	eMasterEvent = EV_MASTER_FRAME_RECIEVING;
	u3RxBuffLen = u3RxBuffLen % COMM_BUFF_LEN;
	U3RxBuffer[u3RxBuffLen] = (unsigned char)ReadUART3();
	u3RxBuffLen++;
    PowerbusTimersT35Enable();
}

/* ----------------------- Start implementation -----------------------------*/
void PowerbusTimersInit(void)
{
	unsigned int prescaler;

        T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
	T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
	T2CONbits.T32 = 1; // Enable 32-bit Timer mode
	T2CONbits.TCS = 0; // Select internal instruction cycle clock
	T2CONbits.TGATE = 0; // Disable Gated Timer mode
	T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR3 = 0x00; // Clear 32-bit Timer (msw)
	TMR2 = 0x00; // Clear 32-bit Timer (lsw)
	PR3 = 0x0000; // Load 32-bit period value (msw)
	PR2 = 0xffff; // Load 32-bit period value (lsw)
	IPC2bits.T3IP = 0x05; // Set Timer3 Interrupt Priority Level
	IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
	IEC0bits.T3IE = 1; // Enable Timer3 interrupt
	T2CONbits.TON = 0; // Start 32-bit Timer

        prescaler = 1;
        usT35TimeOut1ms = FCY /(1000*prescaler);
}

void PowerbusTimerEnable(unsigned long int inv)
{
	TMR2 = 0x0000;
	TMR3 = 0x0000;
	PR3 = (unsigned int)(inv>>16);
    PR2 = (unsigned int)inv;       // Timer1 period register = ?????
    T2CONbits.TON = 1;  // Enable Timer1 and start the counter
}
void PowerbusTimersT35Enable(void)
{
	unsigned long int timer_tick = (7L* TICKS_PER_SEC * 10L * usT35TimeOut1ms)/(baudrate*2L);
	PowerbusTimerEnable(timer_tick);
}

void PowerbusTimersConvertDelayEnable()
{
	unsigned long int timer_tick = MB_MASTER_DELAY_MS_CONVERT *usT35TimeOut1ms;
	PowerbusTimerEnable(timer_tick);
}

void PowerbusTimersRespondTimeoutEnable()
{
	unsigned long int timer_tick = MB_MASTER_TIMEOUT_MS_RESPOND * usT35TimeOut1ms;
	PowerbusTimerEnable(timer_tick);
}

void PowerbusTimersDisable(void)
{
	T2CONbits.TON = 0;
}
eMBMasterEventType getPowerbusMasterEvent(void)
{
    return eMasterEvent;
}
void resetPowerbusState(void)
{
	u3RxBuffLen = 0;
	eMasterEvent = EV_MASTER_IDLE;
}
/* Get Modbus Master send RTU's buffer address pointer.*/
void vMBMasterGetRTUSndBuf( unsigned char ** pucFrame )
{
	*pucFrame = ( unsigned char * ) U3TxBuffer;
}
/* Get Modbus Master Recv RTU's buffer address pointer.*/
void vMBMasterGetRTURecvBuf( unsigned char ** pucFrame )
{
	*pucFrame = ( unsigned char * ) U3RxBuffer;
}

void xMBMasterRTUTimerExpired(void)
{

	switch (eMasterEvent)
	{
		/* Timer t35 expired. Startup phase is finished. */
	case EV_MASTER_FRAME_WAIT_RESPOND:
		eMasterEvent=EV_MASTER_FRAME_TIME_OUT;
		break;

		/* A frame was received and t35 expired. Notify the listener that
		 * a new frame was received. */
	case EV_MASTER_FRAME_RECIEVING:
		eMasterEvent=EV_MASTER_FRAME_RECIEVED;
		break;
		/* Function called in an illegal state. */
	default:
		eMasterEvent=EV_MASTER_IDLE;
		break;
	}
	
}

void prvvTIMERExpiredISR(void)
{
    xMBMasterRTUTimerExpired();
}


/**
 * This function is PLC uart receive timer callback function
 *
 * @param parameter null
 */
void __attribute__ ( (interrupt, no_auto_psv) ) _T3Interrupt( void )
{
	IFS0bits.T3IF = 0;
    T2CONbits.TON = 0;
    prvvTIMERExpiredISR();
}

void __attribute__ ( (interrupt, no_auto_psv) ) _U3RXInterrupt( void )
{
    IFS5bits.U3RXIF = 0;
    prvvUARTRxISR();
    
}
void __attribute__ ( (interrupt, no_auto_psv) ) _U3TXInterrupt( void )
{
    IFS5bits.U3TXIF = 0;    
}

void __attribute__((__interrupt__,no_auto_psv)) _DMA0Interrupt(void)
{
   	IFS0bits.DMA0IF = 0; // Clear the DMA0 Interrupt Flag
   	eMasterEvent=EV_MASTER_FRAME_WAIT_RESPOND;
	PowerbusEnable(1,0);
	PowerbusTimersRespondTimeoutEnable();
   	
}



