/*
 * File:   uNand.c
 * Author: ld
 *
 * Created on 2017Äê4ÔÂ5ÈÕ, ÏÂÎç5:31
 */


#include "xc.h"
#include "uInit.h"
#include "uNand.h"
#include "uDelay.h"

#define WriteToBus(data) (LATB=(LATB&0x00ff) |(data<<8))
#define ReadFromBus()	(PORTB >>8)
static void nand_PortCfg(void)
{
	/************set all ports as digital port***************/
	//data ports
	ANSELBbits.ANSB8 = 0;ANSELBbits.ANSB9 = 0;ANSELBbits.ANSB10= 0;ANSELBbits.ANSB11= 0;
    ANSELBbits.ANSB12= 0;ANSELBbits.ANSB13= 0;ANSELBbits.ANSB14= 0;ANSELBbits.ANSB15= 0;

	//control ports
    ANSELBbits.ANSB0 = 0;ANSELBbits.ANSB1 = 0;ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;ANSELBbits.ANSB4 = 0;ANSELBbits.ANSB5 = 0;
    //state ports
    ANSELGbits.ANSG9=0;
	/************set values of the control port which is used ***************/
	NAND_WRITE_nCE=0;NAND_WRITE_nWP=1;
    NAND_WRITE_nWE=1;NAND_WRITE_nCLE=1;NAND_WRITE_nALE=1;NAND_WRITE_nRE=1;
	/************set directions of the ports used***************/

	TRISB=(TRISB & 0x00c0); //bit7,bit6 retained ,others output;

	NAND_TRIS_nRB=1;
	
}

void Nand_Write_Command(unsigned char command)
{
    
    
    NAND_WRITE_nCLE=1;NAND_WRITE_nALE=0;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    WriteToBus(command);
    NAND_WRITE_nWE=1;
    Nop();
}
void Nand_Write_Address(unsigned char address)
{
    
    
    NAND_WRITE_nCLE=0;NAND_WRITE_nALE=1;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    WriteToBus(address);
    NAND_WRITE_nWE=1;
    Nop();
}
void Nand_Write_Data(unsigned char write_data)
{
    
    
    NAND_WRITE_nCLE=0;NAND_WRITE_nALE=0;NAND_WRITE_nRE=1;
    NAND_WRITE_nWE=0;
    WriteToBus(write_data);
    NAND_WRITE_nWE=1;
    Nop();
}
unsigned char Nand_Read_Data(void)
{
    unsigned char result;
    NAND_WRITE_nWE=1;NAND_WRITE_nCLE=0;NAND_WRITE_nALE=0;
    NAND_WRITE_nRE=0;
    result= ReadFromBus();
    NAND_WRITE_nRE=1;
    return result;
}
void nand_wait_idle(void)
{
	while(1 != NAND_READ_nRB)
		;
}
unsigned char nand_read_status(void)
{
	Nand_Write_Command(0x70);
	return ReadFromBus();
}
/***************************************************************************************************************/
/* º¯ÊýÔ­ÐÍ£ºunsigned char nand_ReadPage(unsigned long Addr,unsigned int readLen,unsigned char *pData)*/
/* º¯Êý¹¦ÄÜ£ºËæ»ú¶ÁÈ¡Êý¾ÝÇøÒ³ÄÚÊý¾Ý*/
/* º¯Êý²ÎÊý£ºaddr--ÆðÊ¼µØÖ·                                 */
/*          			readlen--¶ÁÈ¡×Ö½ÚÊý                             */
/*          			pData--¶ÁÈ¡µÄÊý¾Ý»º´æµÄÖ¸Õë                      */
/*   ·µ»ØÖµ£º·Ç0-¶ÁÈ¡³É¹¦µÄ×Ö½ÚÊý£»                           */
/*          0--¶ÁÈ¡Ê§°Ü                      */
/***************************************************************************************************************/
unsigned int nand_ReadPage(unsigned long addr,unsigned int readLen,unsigned char *pData)
{
    unsigned int colAddr;
    unsigned long rowAddr;
    unsigned int result=0;
    colAddr=(unsigned int)(addr&0x07ff);//11Î»ÁÐµØÖ·
    addr=addr>>11;
    rowAddr=addr&0x1ffff;//17Î»ÐÐµØÖ·
    readLen=((BYTES_OF_PAGE-colAddr)<readLen)?(BYTES_OF_PAGE-colAddr):readLen;
    Nand_Write_Command(0x00);
    Nand_Write_Address((unsigned char)(colAddr&0xff));
    Nand_Write_Address((unsigned char)((colAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    Nand_Write_Command(0x30);
    nand_wait_idle();
    for(result=0;result<readLen;result++)
    {
        *pData=Nand_Read_Data();
        pData++;
    }
	return result;
}
/***************************************************************************************************************/
/* º¯ÊýÔ­ÐÍ£ºunsigned char nand_WritePage(unsigned long Addr,unsigned int wrLen,unsigned char *pData)*/
/* º¯Êý¹¦ÄÜ£ºËæ»ú¶ÁÈ¡Êý¾ÝÇøÒ³ÄÚÊý¾Ý*/
/* º¯Êý²ÎÊý£ºaddr--ÆðÊ¼µØÖ·                                 */
/*          readlen--ÒªÐ´ÈëµÄ×Ö½ÚÊý                             */
/*          pData--ÒªÐ´ÈëµÄÊý¾Ý»º´æÊ×µØÖ·µÄÖ¸Õë                      */
/*   ·µ»ØÖµ£º·Ç0-Ð´Èë³É¹¦£»                           */
/*          0--Ð´ÈëÊ§°Ü                      */
/***************************************************************************************************************/
unsigned int nand_WritePage(unsigned long addr,unsigned int wrLen,unsigned char *pData)
{
    unsigned int colAddr;
    unsigned long rowAddr;
    unsigned int result=0;
    colAddr=(unsigned int)(addr&0x07ff);//11Î»ÁÐµØÖ·
    addr=addr>>11;
    rowAddr=addr&0x1ffff;//17Î»ÐÐµØÖ·
    wrLen=((BYTES_OF_PAGE-colAddr)<wrLen)?(BYTES_OF_PAGE-colAddr):wrLen;
    Nand_Write_Command(0x80);    
    Nand_Write_Address((unsigned char)(colAddr&0xff));
    Nand_Write_Address((unsigned char)((colAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)(rowAddr&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0xff));
    for(result=0;result<wrLen;result++)
    {
        Nand_Write_Data(*pData);
        pData++;
    }
    Nand_Write_Command(0x10);
	nand_wait_idle();
    return (nand_read_status()&0x01)?0:1;
}
/***************************************************************************************************************/
/* º¯ÊýÔ­ÐÍ£ºunsigned int nand_EraseBlock(unsigned long blockAddr)*/
/* º¯Êý¹¦ÄÜ£º²Á³ý¿éÝ*/
/* º¯Êý²ÎÊý£ºblockAddr--¿éµØÖ·                                 */
/*   ·µ»ØÖµ£º·Ç0-²Á³ý³É¹¦£»                           */
/*          0--²Á³ýÊ§°Ü                      */
/***************************************************************************************************************/
unsigned int nand_EraseBlock(unsigned long blockAddr)
{
    unsigned long rowAddr;
    rowAddr=blockAddr<<6;//17Î»ÐÐµØÖ·
    Nand_Write_Command(0x60);
    Nand_Write_Address((unsigned char)(rowAddr&0xC0));
    Nand_Write_Address((unsigned char)((rowAddr>>8)&0xff));
    Nand_Write_Address((unsigned char)((rowAddr>>16)&0x01));
    Nand_Write_Command(0xD0);
    nand_wait_idle();
    return (nand_read_status()&0x01)?0:1;
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
//Ó¦ÓÃ
unsigned int getReadPositionInBlock(unsigned int blockNum)
{
	unsigned int i;
	unsigned char rdData[1024];
	unsigned long int rdAddr=DATA_RD_FLAG_START_ADDR;
	rdAddr=rdAddr+blockNum*1024;
	
	nand_RandomReadData(rdAddr,1024,rdData);
	for(i=0;i<1024;i++)
	{
		if(rdData[i]!=0x55)
		{
			break;
		}
	}
	return i;	
}
unsigned int getWritePositionInBlock(unsigned int blockNum)
{
	unsigned int i;
	unsigned char rdData[1024];
	unsigned long int rdAddr=DATA_WR_FLAG_START_ADDR;
	rdAddr=rdAddr+blockNum*1024;
	
	nand_RandomReadData(rdAddr,1024,rdData);
	for(i=0;i<1024;i++)
	{
		if(rdData[i]!=0x55)
		{
			break;
		}
	}
	return i;	
}

unsigned int nand_WriteOneRecord(unsigned int block,unsigned int section,void *pSrc,unsigned int len)
{
	unsigned char flag=0x55;
	unsigned long int wrAddr=block*BLOCK_SIZE;
	wrAddr=wrAddr+section*SECTION_SIZE;
	nand_RandomWriteData(wrAddr,len,(unsigned char *)pSrc);
	wrAddr=DATA_WR_FLAG_START_ADDR;wrAddr=wrAddr+block*SEC_PER_BLOCK;wrAddr=wrAddr+section;
	nand_RandomWriteData(wrAddr,1,&flag);
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