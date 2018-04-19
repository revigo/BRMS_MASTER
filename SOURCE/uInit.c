#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "uInit.h"
#include "uLCD.h"
#include "uI2C.h"
#include "uNand.h"
#include "uDelay.h"

unsigned long SysTick_s=0;
unsigned int SysTick_ms=0;;
unsigned char saveMinuteCnt=0;
unsigned char startMin=0;
unsigned char startSec;
unsigned char exCmpMin=0x00;
void set_StartLoopTime(unsigned char SaveCircle)
{
	//gCurSysRecord.loopTime=SaveCircle;
	startMin=PCF8563_Buf.Minute;startSec=PCF8563_Buf.Second;
	saveMinuteCnt=SaveCircle;
}

/******************************************************************************/
/* 函数原型：void init_osc(void)                                              */
/* 功能描述：初始化系统时钟                                                    */
/* 输入参数：无                                                               */
/* 返回参数：无                                                               */
/******************************************************************************/
void init_OSC(void)
{
	RCONbits.WDTO=0;
	// Enable Watch Dog Timer
    RCONbits.SWDTEN = 0;

    PLLFBD = 62;                                // M=62
    CLKDIVbits.PLLPOST = 0;                     // N1=2
    CLKDIVbits.PLLPRE = 0;                      // N2=2

    __builtin_write_OSCCONH( 0x01 );            // Initiate Clock Switch to FRCPLL

    // Oscillator(FRC) with PLL (NOSC=0b011)
    __builtin_write_OSCCONL( OSCCON | 0x01 );  // Start clock switching
    while( OSCCONbits.COSC != 0b001 );
	Nop();Nop();
}
//1ms周期
void init_Timer1( void )
{
    T1CON = 0;          // Timer reset
    T1CONbits.TCKPS=0b11;
    IFS0bits.T1IF = 0;  // Reset Timer1 interrupt flag
    IPC0bits.T1IP = 2;  // Timer1 Interrupt priority level=4
    IEC0bits.T1IE = 1;  // Enable Timer1 interrupt
    TMR1 = 0x0000;
    PR1 = 0x5a00;       // Timer1 period register = ????? 100ms
    T1CONbits.TON = 1;  // Enable Timer1 and start the counter
}
static unsigned char indDisRefresh;
static unsigned char indRtcRefresh;

static unsigned char indPbusShortCircuit;
static unsigned char indSaveRecord;
static unsigned char indCampareTime;
unsigned char getIndTaskRun(IND_TASK_RUN indTaskRun )
{
    unsigned char result;
    switch(indTaskRun)
    {
        case IND_DIS_REFRESH:result=indDisRefresh;indDisRefresh=0;break;
        case IND_RTC_REFRESH:result=indRtcRefresh;indRtcRefresh=0;break;
        case IND_PBUS_SHORTCIRCUIT:result=indPbusShortCircuit;break;
		case IND_SAVE_RECORD:result=indSaveRecord;indSaveRecord=0;break;
		case IND_CAMP_TIME:result=indCampareTime;indCampareTime=0;break;
        default:result=0xff;
    }
    return result;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _T1Interrupt( void )
{
    IFS0bits.T1IF = 0;
    //T1CONbits.TON = 0;
    indPbusShortCircuit=(PORTDbits.RD9==1)?1:0;
    SysTick_ms++;
	if(saveMinuteCnt!=0)
	{
		if(PCF8563_Buf.Minute!=startMin)
		{
			startMin=PCF8563_Buf.Minute;
			saveMinuteCnt--;
			if(saveMinuteCnt==0)indSaveRecord=1;
		}
	}
	if(SysTick_ms ==1)indRtcRefresh=1;
    else if(SysTick_ms == 2)indDisRefresh=1;
	//else if((SysTick_ms==300)&&(saveMinuteCnt!=0))
	//{
	//	if((PCF8563_Buf.Minute!=startMin)&&(PCF8563_Buf.Second==startSec))
	//	{
	//		saveMinuteCnt--;
	//		if(saveMinuteCnt==0)indSaveRecord=1;
	//	}
	//		
	//}
	else if(SysTick_ms == 4)
	{
		if(PCF8563_Buf.Minute!=exCmpMin)
                {
                    indCampareTime=1;
                    exCmpMin=PCF8563_Buf.Minute;
                }
	}
    else if(SysTick_ms==10)
	{
		SysTick_ms=0;
		SysTick_s++;
    }
	
	
    //TMR1 = 0;
    //T1CONbits.TON = 1;

}
