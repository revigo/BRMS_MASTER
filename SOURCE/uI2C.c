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
�������ƣ�void I2C_Start(void)
��    �ܣ����IIC����ʼ��������
��    ������
����ֵ  ����
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
�������ƣ�void I2C_Stop(void)
��    �ܣ����IIC����ֹ��������
��    ������
����ֵ  ����
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
�������ƣ�void I2C_Mack(void)
��    �ܣ����IIC������Ӧ�����
��    ������
����ֵ  ����
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
�������ƣ�void I2C_Mnack(void)
��    �ܣ����IIC��������Ӧ�����
��    ������
����ֵ  ����
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

/**********���Ӧ���źź���******************/
/*�������ֵΪ1��֤����Ӧ���źţ���֮û��*/
/*******************************************
�������ƣ�unsigned char I2C_Check(void)
��    �ܣ����ӻ���Ӧ�����
��    ������
����ֵ  ���ӻ��Ƿ���Ӧ��1--�У�0--��
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
�������ƣ�void I2C_Write1(void)
��    �ܣ���IIC���߷���һ��1
��    ������
����ֵ  ����
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
�������ƣ�void I2C_Write0(void)
��    �ܣ���IIC���߷���һ��0
��    ������
����ֵ  ����
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
�������ƣ�void E2P_Write1byte(unsigned char wdata)
��    �ܣ���IIC���߷���һ���ֽڵ�����
��    ����wdata--���͵�����
����ֵ  ����
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
�������ƣ�unsigned char I2C_WriteNbyte(u8 * outbuffer,u8 n)
��    �ܣ���IIC���߷���N���ֽڵ�����
��    ����outbuffer--ָ�������ݴ���׵�ַ
          ��ָ��
          n--���ݵĸ���
����ֵ  �������Ƿ�ɹ��ı�־��1--�ɹ���0--ʧ��
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
�������ƣ�unsigned char I2C_Read1byte(void)
��    �ܣ���IIC���߶�ȡһ���ֽ�
��    ������
����ֵ  ����ȡ������
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
�������ƣ�readNbyte
��    �ܣ���IIC���߶�ȡN���ֽڵ�����
��    ����inbuffer--��ȡ�����ݴ�ŵ��׵�ַ
          n--���ݵĸ���
����ֵ  ����
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
�������ƣ�u8 E2P_Write_1Byte(u8 wdata,deviceaddress,u8 dataaddress)
��    �ܣ���EEPROM��д��1���ֽڵ�����
��    ����Wdata--д�������
          deviceaddress--����ҳ
          dataaddress--���ݵ�д���ַ
����ֵ  ��д������1--�ɹ���0--ʧ��
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
  Delay_ms(10);       //�ȴ�EEPROM����ڲ�д��
  return 1; 
}
/*******************************************
�������ƣ�Write_NByte
��    �ܣ���EEPROM��д��N���ֽڵ�����
��    ����outbuf--ָ��д�����ݴ���׵�ַ��ָ��
          n--���ݸ�����AT24C64����ܳ���32��
		  AT24C128/256����ܳ���64��
		  ��ҳ��ַ��������󳤶�
          deviceaddress--ҳ
          dataaddress--����д����׵�ַ
����ֵ  ��д������1--�ɹ���0--ʧ��
********************************************/
unsigned char E2P_Write_NByte(void *wrbuf,unsigned int n,unsigned int dataaddress)
{
	unsigned char * outbuf=(unsigned char * )wrbuf;
  unsigned char  flag; 
  unsigned char  addr_H,addr_L;
  
  addr_H  =   dataaddress>>8;
  addr_L  =   dataaddress&0x00ff;
  I2C_Start();
  I2C_Write1byte(AT24C64_ADDR);              //д��������ַ
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
  Delay_ms(10);       //�ȴ�EEPROM����ڲ�д��
  if(flag)    return 1;
  else        return 0;
}
unsigned char E2P_Write_NByte_Randomaddress(void *wrbuf,unsigned int n,unsigned int dataaddress)
{
	unsigned char CurBankCap; //��ǰbank����
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
�������ƣ�Read_1Byte_Randomaddress
��    �ܣ���EEPROM��ָ����ַ��ȡ1���ֽڵ�����
��    ����dataaddress--���ݶ�ȡ�ĵ�ַ
����ֵ  ����ȡ������
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
�������ƣ�Read_NByte_Randomaddress
��    �ܣ���EEPROM��ָ����ַ��ȡN���ֽڵ�����
��    ����readbuf--ָ�򱣴����ݵ�ַ��ָ��
          n--��ȡ���ݵĸ���
          dataaddress--���ݶ�ȡ���׵�ַ
����ֵ  ����ȡ�����1--�ɹ���0--ʧ��
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

/***************************PCF8563��������************************/
/***************************PCF8563��������************************/
/***************************PCF8563��������************************/
/***************************PCF8563��������************************/
/***************************PCF8563��������************************/

/*******************************************
�������ƣ�unsigned char Write8563(unsigned char ucAddr,unsigned char ucData)
��    �ܣ���8563дһ���ֽ�����
��    ����ucAddr�Ĵ�����ַ��ucData����
����ֵ  ��0ʧ��  1�ɹ�
********************************************/
unsigned char Write8563(unsigned char ucAddr,unsigned char ucData) 
{ 
	I2C_Start( );
	I2C_Write1byte(PCF8563_ADDR);
	if(I2C_Check()) 
        {
    	    I2C_Write1byte(ucAddr);                //д�������ֵ�ַ
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
�������ƣ�unsigned char Read8563_1Byte(unsigned char ucAddr) 
��    �ܣ���IIC���߶�ȡ8563һ���ֽ�
��    ����ucAddr�Ĵ�����ַ
����ֵ  ����ȡ�����ݣ�����0����ʧ��
********************************************/
unsigned char Read8563_1Byte(unsigned char ucAddr) 
{ 
	unsigned char  ucData; 	
	I2C_Start( );
	I2C_Write1byte(PCF8563_ADDR);          //д������ַ 
	if(I2C_Check()) 
       I2C_Write1byte(ucAddr);     //д�������ֵ�ַ
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
		ucData=I2C_Read1byte();    ////������ 
	else 
	    return 0;
    I2C_Mnack();
	I2C_Stop();
	Delay_ms(10);
	return ucData;
} 
/*******************************************
�������ƣ�unsigned char Read8563_NByte(unsigned char ucAddr, unsigned int n,unsigned char * buff)
��    �ܣ���IIC���߶�ȡ8563  N���ֽ�
��    ����ucAddr�Ĵ����׵�ַ��n�ֽ�����buff���ݴ���׵�ַ
����ֵ  ��1�ɹ�  0ʧ��
********************************************/
unsigned char Read8563_NByte(unsigned char ucAddr, unsigned int n,unsigned char * buff)
{
  
  //unsigned char i=0;  
  I2C_Start( );
  I2C_Write1byte(PCF8563_ADDR);          //д������ַ 
	if(I2C_Check()) 
       I2C_Write1byte(ucAddr);     //д�������ֵ�ַ
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
�������ƣ�void SetTime_8563(void)
��    �ܣ����ò���ʼ��8563
��    ����ʱ��������涨��ʽԤ�浽PCF8563_Buf��
����ֵ  ����
********************************************/
void SetTime_8563(void)
{
    unsigned char i, *prt,flag=5;
	prt = (unsigned char *)& PCF8563_Buf;
         Nop();
          Nop();
	flag=Write8563(0x00,0x00);//��ʼ������λ1
         Nop();
          Nop();
	flag=Write8563(0x01,0x00);//��ʼ������λ2
         Nop();
          Nop();
/**********************��ʼ��DATA ��7������********************************/
	for(i=0;i<7;i++)
	{
            Write8563(i+2,*prt);
	  prt++;
	}
}
/*******************************************
�������ƣ�void GetTime_8563(void)
��    �ܣ���ȡ8563
��    ����ʱ��������涨��ʽ�浽PCF8563_Date��
����ֵ  ����
********************************************/
void GetTime_8563(void)
{
	unsigned char   *prt;
	prt = (unsigned char *)& PCF8563_Buf;
	Read8563_NByte(2, 7,prt);
	PCF8563_Buf.Second  &=0x7f; //������Чλ
	PCF8563_Buf.Minute  &=0x7f;
	PCF8563_Buf.Hour    &=0x3f;
	PCF8563_Buf.Day     &=0x3f;
	PCF8563_Buf.Weekday &=0x07;
	PCF8563_Buf.Month   &=0x1f;
    //ת�浽DATE���ݵ�ַ����λ���䣬����ʹ��
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

//���ص��Ƿ�����
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
// 1:pBufStruct1��pBufStruct2֮��
// 2:pBufStruct1��pBufStruct2֮ǰ
unsigned int compare_time(time_t tm1,time_t tm2)
{
	if(tm1>tm2)return 1;
	else if(tm1<tm2) return -1;
	else
		return 0;
}