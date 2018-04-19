#include <xc.h>
#include "uInit.h"
#include "uDelay.h"
#include "uI2C.h"


#define  PCF8563_ADDR   0xA2 
#define  AT24C64_ADDR    0xA0 

#define set_clk_out()   (TRISFbits.TRISF5=0)
#define set_clk_in()    (TRISFbits.TRISF5=1)
#define set_clk_odc()   (ODCFbits.ODCF5=1)
#define set_sda_out()   (TRISFbits.TRISF4=0)
#define set_sda_in()    (TRISFbits.TRISF4=1)
#define set_sda_odc()   (ODCFbits.ODCF4=1)

#define set_clk(x)  (LATFbits.LATF5=(x))
#define set_sda(x)  (LATFbits.LATF4=(x))
#define get_sda()   (PORTFbits.RF4)

PCF8563_BufStruct      PCF8563_Buf;
PCF8563_DateStruct     PCF8563_Date;
void init_I2C(void)
{
    set_clk(1);set_sda(1);
    set_clk_out();set_sda_out();
    set_clk_odc();set_sda_odc();
    set_clk(1);
	set_sda(1);    
}
/*******************************************
函数名称：void I2C_Start(void)
功    能：完成IIC的起始条件操作
参    数：无
返回值  ：无
********************************************/
void I2C_Start(void)
{
  set_clk(1);
  set_sda(1);
  Delay_us(10);
  set_sda(0);
  Delay_us(10);
  set_clk(0);
  Delay_us(10);
}
/*******************************************
函数名称：void I2C_Stop(void)
功    能：完成IIC的终止条件操作
参    数：无
返回值  ：无
********************************************/
void I2C_Stop(void)
{
  set_sda(0);
  Delay_us(10);
  set_clk(1);
  Delay_us(10);
  set_sda(1);
  Delay_us(10); 
}
/*******************************************
函数名称：void I2C_Mack(void)
功    能：完成IIC的主机应答操作
参    数：无
返回值  ：无
********************************************/
void I2C_Mack(void)
{
  set_sda(0);
  Nop();Nop();
  set_clk(1);
  Delay_us(10); 
  set_clk(0);
  Nop();Nop();
  set_sda(1);
  //Delay_us(10);
}
/*******************************************
函数名称：void I2C_Mnack(void)
功    能：完成IIC的主机无应答操作
参    数：无
返回值  ：无
********************************************/
void I2C_Mnack(void)
{
  set_sda(1);
  Nop();Nop();
  set_clk(1);
  Delay_us(10); 
  set_clk(0);
 Nop();Nop();
  //i2c_sda   =    1;
 // Delay_us(10);
}

/**********检查应答信号函数******************/
/*如果返回值为1则证明有应答信号，反之没有*/
/*******************************************
函数名称：unsigned char I2C_Check(void)
功    能：检查从机的应答操作
参    数：无
返回值  ：从机是否有应答：1--有，0--无
********************************************/
unsigned char I2C_Check(void)
{
  unsigned char slaveack=0;
 unsigned char i=100;
  set_sda(1);
  Nop();Nop();
  //Delay_us(10);
 
  Nop();Nop();
  set_sda_in();
  Nop();Nop();
  //Delay_us(10);
  while(get_sda()!=0&&i--);
  slaveack  =  get_sda();
   set_clk(1);
   Delay_us(10);
  set_clk(0);
 // Delay_us(10);
  i=100;
  while(get_sda()==0&&i--);
 Nop();
 Nop();
  set_sda_out();
  
  if(slaveack)    return 0;
  else            return 1;
}

/*******************************************
函数名称：void I2C_Write1(void)
功    能：向IIC总线发送一个1
参    数：无
返回值  ：无
********************************************/
void I2C_Write1(void)
{
  set_sda(1);
  Delay_us(10); 
  set_clk(1);
  Delay_us(10); 
  set_clk(0);
  Delay_us(10); 
}
/*******************************************
函数名称：void I2C_Write0(void)
功    能：向IIC总线发送一个0
参    数：无
返回值  ：无
********************************************/
void I2C_Write0(void)
{
    set_sda(0);
  Delay_us(10); 
  set_clk(1);
  Delay_us(10); 
  set_clk(0);
  Delay_us(10); 
}

/*******************************************
函数名称：void E2P_Write1byte(unsigned char wdata)
功    能：向IIC总线发送一个字节的数据
参    数：wdata--发送的数据
返回值  ：无
********************************************/
void I2C_Write1byte(unsigned char wdata)
{
  unsigned char i;
  for(i = 8;i > 0;i--)
  {
    if(wdata & 0x80)	I2C_Write1();
    else 	            I2C_Write0();
    wdata <<= 1;
  }
  set_sda(1);
  Nop();Nop();
}

/*******************************************
函数名称：unsigned char I2C_WriteNbyte(u8 * outbuffer,u8 n)
功    能：向IIC总线发送N个字节的数据
参    数：outbuffer--指向发送数据存放首地址
          的指针
          n--数据的个数
返回值  ：发送是否成功的标志：1--成功，0--失败
********************************************/
unsigned char I2C_WriteNbyte(unsigned char * outbuffer,unsigned int n)
{
  unsigned char i;
  for(i = 0;i < n;i++)
  {
    I2C_Write1byte(* outbuffer);
    if(I2C_Check())      
    {
      outbuffer++;			
    }
    else 
    {
      I2C_Stop();
      return 0;
    }
  }
  I2C_Stop();
  return 1;			
}

/*******************************************
函数名称：unsigned char I2C_Read1byte(void)
功    能：从IIC总线读取一个字节
参    数：无
返回值  ：读取的数据
********************************************/
unsigned char I2C_Read1byte(void)
{
  unsigned char  rdata = 0x00,i,flag;

  for(i = 0;i < 8;i++)
  {
    set_sda(1);
    Delay_us(10);
    set_clk(1);
    set_sda_in();
    Delay_us(10);
    flag = get_sda();
    rdata <<= 1;
    if(flag)	rdata |= 0x01;
    set_sda_out();
    set_clk(0);
    Delay_us(10);
  }
  return rdata;
}
/*******************************************
函数名称：readNbyte
功    能：从IIC总线读取N个字节的数据
参    数：inbuffer--读取后数据存放的首地址
          n--数据的个数
返回值  ：无
********************************************/
void I2C_ReadNbyte(unsigned char * inbuffer,unsigned int n)
{
  unsigned int i;    
  for(i = 0;i < n;i++)
  {
    inbuffer[i] =   I2C_Read1byte();
    if(i < (n-1))   I2C_Mack();
    else            I2C_Mnack();
  }
  I2C_Stop();
}
/*******************************************
函数名称：u8 E2P_Write_1Byte(u8 wdata,deviceaddress,u8 dataaddress)
功    能：向EEPROM中写入1个字节的数据
参    数：Wdata--写入的数据
          deviceaddress--数据页
          dataaddress--数据的写入地址
返回值  ：写入结果：1--成功，0--失败
********************************************/

unsigned char E2P_Write_1Byte(unsigned char wdata,unsigned int dataaddress)
{
  unsigned char  addr_H,addr_L;
  addr_H  =   dataaddress>>8;
  addr_L  =   dataaddress&0x00ff;
  I2C_Start();
  I2C_Write1byte(AT24C64_ADDR);
  if(I2C_Check()) 
    I2C_Write1byte(addr_H);
  else 
    return 0;
  if(I2C_Check()) 
    I2C_Write1byte(addr_L);
  else 
    return 0;
  if(I2C_Check()) 
    I2C_Write1byte(wdata);
  else 
    return 0;
  if(I2C_Check()) I2C_Stop();
  else            return 0;    
  Delay_ms(10);       //等待EEPROM完成内部写入
  return 1; 
}
/*******************************************
函数名称：Write_NByte
功    能：向EEPROM中写入N个字节的数据
参    数：outbuf--指向写入数据存放首地址的指针
          n--数据个数，AT24C64最大不能超过32，
		  AT24C128/256最大不能超过64，
		  由页地址决定其最大长度
          deviceaddress--页
          dataaddress--数据写入的首地址
返回值  ：写入结果：1--成功，0--失败
********************************************/
unsigned char E2P_Write_NByte(void *wrbuf,unsigned int n,unsigned int dataaddress)
{
	unsigned char * outbuf=(unsigned char * )wrbuf;
  unsigned char  flag; 
  unsigned char  addr_H,addr_L;
  
  addr_H  =   dataaddress>>8;
  addr_L  =   dataaddress&0x00ff;
  I2C_Start();
  I2C_Write1byte(AT24C64_ADDR);              //写入器件地址
  if(I2C_Check()) 
    I2C_Write1byte(addr_H);
  else 
    return 0;
  if(I2C_Check()) 
    I2C_Write1byte(addr_L);
  else 
    return 0;
  if(I2C_Check()) 
    flag=I2C_WriteNbyte(outbuf,n);
  else 
    return 0;
  Delay_ms(10);       //等待EEPROM完成内部写入
  if(flag)    return 1;
  else        return 0;
}
unsigned char E2P_Write_NByte_Randomaddress(void *wrbuf,unsigned int n,unsigned int dataaddress)
{
	unsigned char CurBankCap; //当前bank容量
	unsigned int Address=dataaddress;
	unsigned char *pData = wrbuf;
	while(1)
	{
		CurBankCap = 32 - (Address % 32);
		if(CurBankCap <n)
		{
			n=n-CurBankCap;
			E2P_Write_NByte(pData,CurBankCap,Address);
			pData +=CurBankCap;
			Address += CurBankCap;
		}
		else
		{
			CurBankCap =n;
			E2P_Write_NByte(pData,CurBankCap,Address);
			break;
		}
	}
        return 0;
}




/*******************************************
函数名称：Read_1Byte_Randomaddress
功    能：从EEPROM的指定地址读取1个字节的数据
参    数：dataaddress--数据读取的地址
返回值  ：读取的数据
********************************************/
unsigned char E2P_Read_1Byte_Randomaddress(unsigned int dataaddress)
{
  unsigned char  temp;
  unsigned char  addr_H,addr_L;
  addr_H  =   dataaddress>>8;
  addr_L  =   dataaddress&0x00ff;
  I2C_Start();
  I2C_Write1byte(AT24C64_ADDR);
  if(I2C_Check()) 
    I2C_Write1byte(addr_H);
  else 
    return 0;
  if(I2C_Check()) 
    I2C_Write1byte(addr_L);
  else 
    return 0;
  if(I2C_Check()) 
  {
    I2C_Start();
    I2C_Write1byte((AT24C64_ADDR|0x01));
  }
  else    
    return 0;
  if(I2C_Check())	
    temp = I2C_Read1byte();
  else 
    return 0;
  I2C_Mnack();
  I2C_Stop();
  return temp;
}
/*******************************************
函数名称：Read_NByte_Randomaddress
功    能：从EEPROM的指定地址读取N个字节的数据
参    数：readbuf--指向保存数据地址的指针
          n--读取数据的个数
          dataaddress--数据读取的首地址
返回值  ：读取结果：1--成功，0--失败
********************************************/
unsigned char E2P_Read_NByte_Randomaddress(void * rdbuf,unsigned int n,unsigned int dataaddress)
{
	unsigned char * readbuf =(unsigned char *) rdbuf;
  unsigned char  addr_H,addr_L;
  addr_H  =   dataaddress>>8;
  addr_L  =   dataaddress&0x00ff;
  I2C_Start();
  I2C_Write1byte(AT24C64_ADDR);
  if(I2C_Check()) 
    I2C_Write1byte(addr_H);
  else 
    return 0;
  if(I2C_Check()) 
    I2C_Write1byte(addr_L);
  else 
    return 0;
  if(I2C_Check()) 
  {
    I2C_Start();
    I2C_Write1byte((AT24C64_ADDR|0x01));
  }
  else    
    return 0;
  if(I2C_Check()) 
    I2C_ReadNbyte(readbuf,n);
  else 
    return 0;
  return 1;	
}

/***************************PCF8563操作程序************************/
/***************************PCF8563操作程序************************/
/***************************PCF8563操作程序************************/
/***************************PCF8563操作程序************************/
/***************************PCF8563操作程序************************/

/*******************************************
函数名称：unsigned char Write8563(unsigned char ucAddr,unsigned char ucData)
功    能：对8563写一个字节数据
参    数：ucAddr寄存器地址；ucData数据
返回值  ：0失败  1成功
********************************************/
unsigned char Write8563(unsigned char ucAddr,unsigned char ucData) 
{ 
	I2C_Start( );
	I2C_Write1byte(PCF8563_ADDR);
	if(I2C_Check()) 
        {
    	    I2C_Write1byte(ucAddr);                //写入数据字地址
        }
  	else 
        {
           Nop();
            Nop();
            return 0;
        }
    	
	if(I2C_Check()) 
        {
            I2C_Write1byte(ucData);   
        }
    	             
  	else 
    	{
           Nop();
            Nop();
            return 0;
        }
    if(I2C_Check()) I2C_Stop();
    else            return 0; 
	Delay_ms(10);
	return 1;
} 
/*******************************************
函数名称：unsigned char Read8563_1Byte(unsigned char ucAddr) 
功    能：从IIC总线读取8563一个字节
参    数：ucAddr寄存器地址
返回值  ：读取的数据；返回0可能失败
********************************************/
unsigned char Read8563_1Byte(unsigned char ucAddr) 
{ 
	unsigned char  ucData; 	
	I2C_Start( );
	I2C_Write1byte(PCF8563_ADDR);          //写器件地址 
	if(I2C_Check()) 
       I2C_Write1byte(ucAddr);     //写入数据字地址
  	else 
       return 0; 
	if(I2C_Check()) 
  	{
	    I2C_Stop();
	    I2C_Start();
	    I2C_Write1byte((PCF8563_ADDR|0x01));
  	}
  	else    
      return 0;
	if(I2C_Check()) 
		ucData=I2C_Read1byte();    ////读数据 
	else 
	    return 0;
    I2C_Mnack();
	I2C_Stop();
	Delay_ms(10);
	return ucData;
} 
/*******************************************
函数名称：unsigned char Read8563_NByte(unsigned char ucAddr, unsigned int n,unsigned char * buff)
功    能：从IIC总线读取8563  N个字节
参    数：ucAddr寄存器首地址，n字节数，buff数据存放首地址
返回值  ：1成功  0失败
********************************************/
unsigned char Read8563_NByte(unsigned char ucAddr, unsigned int n,unsigned char * buff)
{
  
  //unsigned char i=0;  
  I2C_Start( );
  I2C_Write1byte(PCF8563_ADDR);          //写器件地址 
	if(I2C_Check()) 
       I2C_Write1byte(ucAddr);     //写入数据字地址
  	else 
       return 0; 
  if(I2C_Check()) 
  {
    I2C_Stop();
    I2C_Start();
    I2C_Write1byte((PCF8563_ADDR|0x01));
  }
  else    
    return 0;
  if(I2C_Check()) 
    I2C_ReadNbyte(buff,n);
  else 
    return 0;

  return 1;	
}
/*******************************************
函数名称：void SetTime_8563(void)
功    能：设置并初始化8563
参    数：时间参数按规定格式预存到PCF8563_Buf；
返回值  ：无
********************************************/
void SetTime_8563(void)
{
    unsigned char i, *prt,flag=5;
	prt = (unsigned char *)& PCF8563_Buf;
         Nop();
          Nop();
	flag=Write8563(0x00,0x00);//初始化控制位1
         Nop();
          Nop();
	flag=Write8563(0x01,0x00);//初始化控制位2
         Nop();
          Nop();
/**********************初始化DATA 共7个参数********************************/
	for(i=0;i<7;i++)
	{
            Write8563(i+2,*prt);
	  prt++;
	}
}
/*******************************************
函数名称：void GetTime_8563(void)
功    能：读取8563
参    数：时间参数按规定格式存到PCF8563_Date；
返回值  ：无
********************************************/
void GetTime_8563(void)
{
	unsigned char   *prt;
	prt = (unsigned char *)& PCF8563_Buf;
	Read8563_NByte(2, 7,prt);
	PCF8563_Buf.Second  &=0x7f; //屏蔽无效位
	PCF8563_Buf.Minute  &=0x7f;
	PCF8563_Buf.Hour    &=0x3f;
	PCF8563_Buf.Day     &=0x3f;
	PCF8563_Buf.Weekday &=0x07;
	PCF8563_Buf.Month   &=0x1f;
    //转存到DATE数据地址，按位分配，方便使用
	PCF8563_Date.YearH    =   PCF8563_Buf.Year/16;
	PCF8563_Date.YearL    =   PCF8563_Buf.Year%16;
    PCF8563_Date.MonthH   =   PCF8563_Buf.Month/16;
	PCF8563_Date.MonthL   =   PCF8563_Buf.Month%16;
	PCF8563_Date.DayH     =   PCF8563_Buf.Day/16;
	PCF8563_Date.DayL     =   PCF8563_Buf.Day%16;
	PCF8563_Date.HourH    =   PCF8563_Buf.Hour/16;
	PCF8563_Date.HourL    =   PCF8563_Buf.Hour%16;
	PCF8563_Date.MinuteH  =   PCF8563_Buf.Minute/16;
	PCF8563_Date.MinuteL  =   PCF8563_Buf.Minute%16;
	PCF8563_Date.SecondH  =   PCF8563_Buf.Second/16;
	PCF8563_Date.SecondL  =   PCF8563_Buf.Second%16;
        PCF8563_Date.Weekday  =   PCF8563_Buf.Weekday;
}

//返回的是分钟数
time_t get_mktime(PCF8563_BufStruct *pBufStruct)
{
	struct tm  tempTm;
	time_t result;
        tempTm.tm_sec=((pBufStruct->Second >> 4) * 10)+ (pBufStruct->Second & 0x0f);
        tempTm.tm_min=((pBufStruct->Minute >> 4) * 10)+ (pBufStruct->Minute & 0x0f);
        tempTm.tm_hour=((pBufStruct->Hour >> 4) * 10)+ (pBufStruct->Hour & 0x0f);
        tempTm.tm_mday=((pBufStruct->Day >> 4) * 10)+ (pBufStruct->Day & 0x0f);
        tempTm.tm_mon=((pBufStruct->Month >> 4) * 10)+ (pBufStruct->Month & 0x0f);
        tempTm.tm_mon=tempTm.tm_mon-1;
        tempTm.tm_year=((pBufStruct->Year >> 4) * 10)+ (pBufStruct->Year & 0x0f);
        tempTm.tm_year=tempTm.tm_year+100;
	result=mktime(&tempTm);
	result=result-28800;
	result=result / 60;
        return result;
}
time_t get_mktimeBCDArray(unsigned char bcdArray[7])
{
	struct tm  tempTm;
	time_t result;
        tempTm.tm_sec=((bcdArray[6] >> 4) * 10)+ (bcdArray[6] & 0x0f);
        tempTm.tm_min=((bcdArray[5] >> 4) * 10)+ (bcdArray[5] & 0x0f);
        tempTm.tm_hour=((bcdArray[4] >> 4) * 10)+ (bcdArray[4] & 0x0f);
        tempTm.tm_mday=((bcdArray[3] >> 4) * 10)+ (bcdArray[3] & 0x0f);
        tempTm.tm_mon=((bcdArray[2] >> 4) * 10)+ (bcdArray[2] & 0x0f);
        tempTm.tm_mon=tempTm.tm_mon-1;
        tempTm.tm_year=((bcdArray[1] >> 4) * 10)+ (bcdArray[1] & 0x0f);
        tempTm.tm_year=tempTm.tm_year+100;

        result=mktime(&tempTm);
	result=result-28800;
	result=result / 60;
        return result;
}

void transfer_BufStructToArrayBCD(PCF8563_BufStruct *pBufStruct,unsigned char BCDArray[7])
{
	BCDArray[0]=0x20;
	BCDArray[1]=pBufStruct->Year;
	BCDArray[2]=pBufStruct->Month;
	BCDArray[3]=pBufStruct->Day;
	BCDArray[4]=pBufStruct->Hour;
	BCDArray[5]=pBufStruct->Minute;
	BCDArray[6]=pBufStruct->Second;
}
void transfer_ArrayBCDToBufStruct(unsigned char BCDArray[7],PCF8563_BufStruct *pBufStruct)
{
	pBufStruct->Year=BCDArray[1];
	pBufStruct->Month=BCDArray[2];
	pBufStruct->Day=BCDArray[3];
	pBufStruct->Hour=BCDArray[4];
	pBufStruct->Minute=BCDArray[5];
	pBufStruct->Second=BCDArray[6];
}
//0:pBufStruct1==pBufStruct2
// 1:pBufStruct1在pBufStruct2之后
// 2:pBufStruct1在pBufStruct2之前
unsigned int compare_time(time_t tm1,time_t tm2)
{
	if(tm1>tm2)return 1;
	else if(tm1<tm2) return -1;
	else
		return 0;
}