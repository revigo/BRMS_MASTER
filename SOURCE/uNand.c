/*
 * File:   uNand.c
 * Author: ld
 *
 * Created on 2017年4月5日, 下午5:31
 */


#include "xc.h"
#include "uInit.h"
#include "uNand.h"
#include "uDelay.h"
static void set_DataBusIn(unsigned char setIn)
{
    unsigned int reg;
    reg=TRISB;
    reg=(setIn==0x01)?(reg|0xff00):(reg&0x00ff);
   TRISB=reg;
   Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
}
static void Nand_WriteByte(unsigned char wrData)
{
    unsigned int latBus;
    unsigned int Data=wrData;
    //set_DataBusIn(0x00);
    Data=Data<<8;
    latBus=LATB;
    latBus=latBus&0x00ff;
    LATB=latBus|Data;
}
static unsigned char Nand_ReadByte(void)
{
    unsigned int rdData;
    //set_DataBusIn(0x01);
    rdData=PORTB;
    return (unsigned char)(rdData>>8);
}
static void init_Ports(void)
{
    ANSELBbits.ANSB8 = 0;ANSELBbits.ANSB9 = 0;ANSELBbits.ANSB10= 0;ANSELBbits.ANSB11= 0;
    ANSELBbits.ANSB12= 0;ANSELBbits.ANSB13= 0;ANSELBbits.ANSB14= 0;ANSELBbits.ANSB15= 0;
    
    ANSELBbits.ANSB0 = 0;ANSELBbits.ANSB1 = 0;ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;ANSELBbits.ANSB4 = 0;ANSELBbits.ANSB5 = 0;
    
    ANSELGbits.ANSG9=0;
    
    NAND_TRIS_nRB = 1;
    NAND_TRIS_nRE = 0;NAND_TRIS_nCE = 0;NAND_TRIS_nCLE= 0;
    NAND_TRIS_nALE= 0;NAND_TRIS_nWE = 0;NAND_TRIS_nWP = 0;
}
void init_Nand(void)
{
    NAND_WRITE_nCE=0;NAND_WRITE_nWP=1;
    NAND_WRITE_nWE=1;NAND_WRITE_nCLE=1;NAND_WRITE_nALE=1;NAND_WRITE_nRE=1;
    init_Ports();

    //NAND_WRITE_nCE=0;NAND_WRITE_nWP=1;
    //NAND_WRITE_nCLE=1;NAND_WRITE_nALE=1;NAND_WRITE_nRE=1;NAND_WRITE_nWE=1;
}
void Nand_Write_Command(unsigned char command)
{
    
    
    NAND_WRITE_nCLE=1;NAND_WRITE_nALE=0;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    Nand_WriteByte(command);
    NAND_WRITE_nWE=1;
    Nop();
}
void Nand_Write_Address(unsigned char address)
{
    
    
    NAND_WRITE_nCLE=0;NAND_WRITE_nALE=1;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    Nand_WriteByte(address);
    NAND_WRITE_nWE=1;
    Nop();
}
void Nand_Write_Data(unsigned char write_data)
{
    
    
    NAND_WRITE_nCLE=0;NAND_WRITE_nALE=0;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    Nand_WriteByte(write_data);
    NAND_WRITE_nWE=1;
    Nop();
}
unsigned char Nand_Read_Data(void)
{
    unsigned char result;
    NAND_WRITE_nWE=1;NAND_WRITE_nCLE=0;NAND_WRITE_nALE=0;
    NAND_WRITE_nRE=0;
    result= Nand_ReadByte();
    NAND_WRITE_nRE=1;
    return result;
}

/***************************************************************************************************************/
/* 函数原型：unsigned char nand_RandomReadExt(unsigned long Addr,unsigned int readLen,unsigned char *pData)*/
/* 函数功能：随机读取数据区页内数据*/
/* 函数参数：addr--起始地址                                 */
/*          readlen--读取字节数                             */
/*          pData--读取的数据缓存的指针                      */
/*   返回值：非0-读取成功的字节数；                           */
/*          0--读取失败                      */
/***************************************************************************************************************/
unsigned int nand_ReadBlockInvalid(unsigned long addr)
{
    unsigned int colAddr;
    unsigned long rowAddr;
    unsigned int result=0;
    colAddr=(unsigned int)(addr&0x07ff);//11位列地址
    colAddr=colAddr|0x0800;
    addr=addr>>11;
    rowAddr=addr&0x1ffff;//17位行地址
    
    //readLen=((2048-colAddr)<readLen)?(2048-colAddr):readLen;
    set_DataBusIn(0x00);
    Nand_Write_Command(0x00);
    Nand_Write_Address((unsigned char)(colAddr&0xff));
    Nand_Write_Address((unsigned char)((colAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    
    Nand_Write_Command(0x30);
    while(NAND_READ_nRB==0){}
    set_DataBusIn(0x01);
    /*while(readLen)
    {
        *pData=Nand_Read_Data();
        pData++;
        result++;
        readLen--;
    }*/
   // for(result=0;result<readLen;result++)
   // {
        result=(Nand_Read_Data()!=0xff)?1:0;
    //    pData++;
    //}
    Delay_ms(2);
    return result;
}
/***************************************************************************************************************/
/* 函数原型：unsigned char nand_RandomReadData(unsigned long Addr,unsigned int readLen,unsigned char *pData)*/
/* 函数功能：随机读取数据区页内数据*/
/* 函数参数：addr--起始地址                                 */
/*          readlen--读取字节数                             */
/*          pData--读取的数据缓存的指针                      */
/*   返回值：非0-读取成功的字节数；                           */
/*          0--读取失败                      */
/***************************************************************************************************************/
unsigned int nand_RandomReadData(unsigned long addr,unsigned int readLen,unsigned char *pData)
{
    unsigned int colAddr;
    unsigned long rowAddr;
    unsigned int result=0;
    colAddr=(unsigned int)(addr&0x07ff);//11位列地址
    addr=addr>>11;
    rowAddr=addr&0x1ffff;//17位行地址
    ENTER_CRITICAL();
    readLen=((2048-colAddr)<readLen)?(2048-colAddr):readLen;
    set_DataBusIn(0x00);
    Nand_Write_Command(0x00);
    Nand_Write_Address((unsigned char)(colAddr&0xff));
    Nand_Write_Address((unsigned char)((colAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    
    Nand_Write_Command(0x30);
    while(NAND_READ_nRB==0){}
    set_DataBusIn(0x01);
    /*while(readLen)
    {
        *pData=Nand_Read_Data();
        pData++;
        result++;
        readLen--;
    }*/
    for(result=0;result<readLen;result++)
    {
        *pData=Nand_Read_Data();
        pData++;
    }
    Delay_ms(2);
    EXIT_CRITICAL();
    return ((NAND_READ_nRB==0)?0:result);
}
/***************************************************************************************************************/
/* 函数原型：unsigned char nand_RandomWriteData(unsigned long Addr,unsigned int wrLen,unsigned char *pData)*/
/* 函数功能：随机读取数据区页内数据*/
/* 函数参数：addr--起始地址                                 */
/*          readlen--要写入的字节数                             */
/*          pData--要写入的数据缓存首地址的指针                      */
/*   返回值：非0-写入成功的字节数；                           */
/*          0--写入失败                      */
/***************************************************************************************************************/
unsigned int nand_RandomWriteData(unsigned long addr,unsigned int wrLen,unsigned char *pData)
{
    unsigned int colAddr;
    unsigned long rowAddr;
    unsigned int result=0;
    colAddr=(unsigned int)(addr&0x07ff);//11位列地址
    addr=addr>>11;
    rowAddr=addr&0x1ffff;//17位行地址
    ENTER_CRITICAL();
    wrLen=((2048-colAddr)<wrLen)?(2048-colAddr):wrLen;
    set_DataBusIn(0x00);
    Nand_Write_Command(0x80);
    
    Nand_Write_Address((unsigned char)(colAddr&0xff));
    Nand_Write_Address((unsigned char)((colAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
	Nop();Nop();Nop();Nop();
    for(result=0;result<wrLen;result++)
    {
        Nand_Write_Data(*pData);
        pData++;
    }
    Nand_Write_Command(0x10);
    Delay_ms(2);
    EXIT_CRITICAL();
    return ((NAND_READ_nRB==0)?result:0);
}
unsigned int nand_EraseBlock(unsigned long blockAddr)
{
    unsigned long rowAddr;
    rowAddr=blockAddr<<6;//17位行地址
    set_DataBusIn(0x00);
    Nand_Write_Command(0x60);
    //Nand_Write_Address(0x00);
    //Nand_Write_Address(0x00);
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    Nand_Write_Command(0xD0);
    Delay_ms(10);
    return (NAND_READ_nRB==0);
}

unsigned int nand_EraseDevice(void)
{
	unsigned long i;
	for(i=0;i<BLOCK_OF_DEVICE;i++)
	{
		nand_EraseBlock(i);
	}
	return 1;
}

unsigned int nand_WriteOneRecord(unsigned int block,unsigned int section,void *pSrc,unsigned int len)
{
	unsigned char flag=(unsigned char )block;
	unsigned long int wrAddr=block*BLOCK_SIZE;
	wrAddr=wrAddr+section*SECTION_SIZE;
	nand_RandomWriteData(wrAddr,len,(unsigned char *)pSrc);
	wrAddr=DATA_WR_FLAG_START_ADDR;wrAddr=wrAddr+section;
	E2P_Write_NByte(&flag,1,wrAddr);
	return 1;
}
unsigned int nand_ReadOneRecord(unsigned int block,unsigned int section,void *pDst,unsigned int len)
{
	unsigned long rdAddr=block*BLOCK_SIZE;
	rdAddr=rdAddr+section*SECTION_SIZE;
	nand_RandomReadData(rdAddr,len,(unsigned char *)pDst);
	//rdAddr=DATA_RD_FLAG_START_ADDR;rdAddr=rdAddr+block*SEC_PER_BLOCK;rdAddr=rdAddr+section;
	//nand_RandomWriteData(rdAddr,1,&flag);
	
	return 1;
}