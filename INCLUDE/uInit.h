/* 
 * File:   uInit.h
 * Author: ld
 *
 * Created on 2017��4��5��, ����5:42
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
//1ms����
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
//ͨѶ������uModbus.c
extern sSysRecords gCurSysRecord;
extern unsigned char slaveOffline[SLAVE_NUM];
extern unsigned char slaveNewLight[SLAVE_NUM];

//ʵʱʱ�ӣ�uI2C.c
extern PCF8563_BufStruct      PCF8563_Buf;
extern PCF8563_DateStruct     PCF8563_Date;
#define INIT_FLAG			0x1234
typedef struct{
    unsigned long rs232_baud;//��λ��������
    unsigned int wrBlock;//��ǰӦд��nandflash�Ŀ�λ��
    unsigned int rdBlock;//��ǰӦ����NANDFLASH�Ŀ�λ��
	unsigned int slaveSaveInv;//ģ�����ݴ洢����
	unsigned int autoControl;//�Ƿ��Զ����ƣ�ָʾSLAVE_CONTROL�Ƿ���Ч
	unsigned int breakSection;//�𻵵�������
	unsigned int flag;
    unsigned int crc16;
}STORE_PARAMS;


typedef struct{
	unsigned int light_on_time;//����ʱ��(����)
	unsigned int light_off_time;//�ص�ʱ��(����)
	time_t startTimeSec;//��ʼʱ��(1900�꿪ʼ)
	unsigned int loopTime;//����Ϩ��ѭ������
}SLAVE_CONTROL;
typedef struct{
	unsigned long rs232_baud;//��λ��������
	unsigned int wrBlock;//��ǰӦд��nandflash�Ŀ�λ��
	unsigned int wrSection;//��ǰӦд��ɽ��λ��(��һ������)
	unsigned int rdBlock;//��ǰӦ����NANDFLASH�Ŀ�λ��
	unsigned int rdSection;//��ǰӦ����ɽ��λ��(��һ������)
	unsigned char slaveSaveInv;//ģ�����ݴ洢����
	unsigned int autoControl;//�Ƿ��Զ����ƣ�ָʾSLAVE_CONTROL�Ƿ���Ч
	unsigned int breakSection;//�𻵵�������
	SLAVE_CONTROL slaveControls[SLAVE_NUM];
}RUN_PARAMS;

#define STORE_PARAMS_ADDR       0X0000
#define SLAVE_CONTROL_ADDR		0x0100
extern RUN_PARAMS gRunParams;
#ifdef	__cplusplus
}
#endif

#endif	/* UINIT_H */

