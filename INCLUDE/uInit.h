/* 
 * File:   uInit.h
 * Author: ld
 *
 * Created on 2017年4月5日, 下午5:42
 */

#ifndef UINIT_H
#define	UINIT_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <time.h>
#include <xc.h>
#include "uI2C.h"
#define SLAVE_NUM       40

#define ENTER_CRITICAL()// (INTCON2bits.GIE=0)
#define EXIT_CRITICAL() // (INTCON2bits.GIE=1)
void init_OSC(void);
//1ms周期
void init_Timer1( void );
    typedef enum{
        IND_DIS_REFRESH=1,
                IND_RTC_REFRESH=2,
                IND_PBUS_SHORTCIRCUIT=3,
                IND_SAVE_RECORD		=4,
                IND_CAMP_TIME		=5,
    }IND_TASK_RUN;
    unsigned char getIndTaskRun(IND_TASK_RUN indTaskRun );
	void set_StartLoopTime(unsigned char SaveCircle);
typedef struct {
	unsigned char loopTime;
	unsigned char timeBCD[7];
	unsigned short slavePulse[SLAVE_NUM];
	unsigned char slaveLight[SLAVE_NUM];
	unsigned int crc16;
}sSysRecords;
//通讯参数，uModbus.c
extern sSysRecords gCurSysRecord;
extern unsigned char slaveOffline[SLAVE_NUM];
extern unsigned char slaveNewLight[SLAVE_NUM];

//实时时钟，uI2C.c
extern PCF8563_BufStruct      PCF8563_Buf;
extern PCF8563_DateStruct     PCF8563_Date;
#define INIT_FLAG			0x1234
typedef struct{
    unsigned long rs232_baud;//上位机波特率
    unsigned int wrBlock;//当前应写的nandflash的块位置
    unsigned int rdBlock;//当前应读的NANDFLASH的块位置
	unsigned int slaveSaveInv;//模块数据存储周期
	unsigned int autoControl;//是否自动控制，指示SLAVE_CONTROL是否有效
	unsigned int breakSection;//损坏的扇区数
	unsigned int flag;
    unsigned int crc16;
}STORE_PARAMS;


typedef struct{
	unsigned int light_on_time;//开灯时长(分钟)
	unsigned int light_off_time;//关灯时长(分钟)
	time_t startTimeSec;//开始时间(1900年开始)
	unsigned int loopTime;//亮灯熄灯循环次数
}SLAVE_CONTROL;
typedef struct{
	unsigned long rs232_baud;//上位机波特率
	unsigned int wrBlock;//当前应写的nandflash的块位置
	unsigned int wrSection;//当前应写的山区位置(在一个块中)
	unsigned int rdBlock;//当前应读的NANDFLASH的块位置
	unsigned int rdSection;//当前应读的山区位置(在一个块中)
	unsigned char slaveSaveInv;//模块数据存储周期
	unsigned int autoControl;//是否自动控制，指示SLAVE_CONTROL是否有效
	unsigned int breakSection;//损坏的扇区数
	SLAVE_CONTROL slaveControls[SLAVE_NUM];
}RUN_PARAMS;

#define STORE_PARAMS_ADDR       0X0000
#define SLAVE_CONTROL_ADDR		0x0100
extern RUN_PARAMS gRunParams;
#ifdef	__cplusplus
}
#endif

#endif	/* UINIT_H */

