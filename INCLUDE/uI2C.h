/* 
 * File:   uI2C.h
 * Author: ld
 *
 * Created on 2017年4月5日, 下午5:33
 */

#ifndef UI2C_H
#define	UI2C_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <time.h>

#define E2P_DEV_SIZE		32768
#define E2P_PAGE_SIZE		64
#define E2P_PAGES			(E2P_DEV_SIZE/E2P_PAGE_SIZE)
#define DATA_WR_FLAG_START_ADDR			0x7E00
#define DATA_RD_FLAG_START_ADDR			0x7F00
#define DATA_FLAG_SIZE_PER_BLOCK		256
#define SEC_PER_BLOCK		256


// *****************************************************************************
// Section: 日期时间结构体定义及变量声明
// *****************************************************************************
//===================日期时间变结构体声明==========================
//按位存储日期及时间，方便显示调用
typedef struct
{
  unsigned char  YearH;
  unsigned char  YearL;
  unsigned char  MonthH;
  unsigned char  MonthL;
  unsigned char  DayH;
  unsigned char  DayL;
  unsigned char  HourH;
  unsigned char  HourL;
  unsigned char  MinuteH;
  unsigned char  MinuteL;
  unsigned char  SecondH;
  unsigned char  SecondL;
  unsigned char  Weekday;
}PCF8563_DateStruct;

//日期及时间缓存BCD格式
typedef struct
{
	unsigned char  Second;
	unsigned char  Minute;
	unsigned char  Hour;
	unsigned char  Day;	
    unsigned char  Weekday;
    unsigned char  Month;
    unsigned char  Year;
}PCF8563_BufStruct;//

void init_I2C(void);
unsigned char E2P_Write_1Byte(unsigned char wdata,unsigned int dataaddress);
unsigned char E2P_Write_NByte(void * outbuf,unsigned int n,unsigned int dataaddress);
unsigned char E2P_Write_NByte_Randomaddress(void *wrbuf,unsigned int n,unsigned int dataaddress);
unsigned char E2P_Read_1Byte_Randomaddress(unsigned int dataaddress);
unsigned char E2P_Read_NByte_Randomaddress(void * readbuf,unsigned int n,unsigned int dataaddress);

void SetTime_8563(void) ;
void GetTime_8563(void);
time_t get_mktime(PCF8563_BufStruct *pBufStruct);
time_t get_mktimeBCDArray(unsigned char bcdArray[7]);

void transfer_BufStructToArrayBCD(PCF8563_BufStruct *pBufStruct,unsigned char BCDArray[7]);
void transfer_ArrayBCDToBufStruct(unsigned char BCDArray[7],PCF8563_BufStruct *pBufStruct);
unsigned int compare_time(time_t pBufStruct1,time_t pBufStruct2);

#ifdef	__cplusplus
}
#endif

#endif	/* UI2C_H */

