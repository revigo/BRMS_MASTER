/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 
#define NAND_TRIS_nRB      TRISGbits.TRISG9
#define NAND_TRIS_nRE      TRISBbits.TRISB5
#define NAND_TRIS_nCE       TRISBbits.TRISB4
#define NAND_TRIS_nCLE      TRISBbits.TRISB3
#define NAND_TRIS_nALE      TRISBbits.TRISB2
#define NAND_TRIS_nWE       TRISBbits.TRISB1
#define NAND_TRIS_nWP       TRISBbits.TRISB0
    
#define NAND_READ_nRB      PORTGbits.RG9
#define NAND_WRITE_nRE      LATBbits.LATB5
#define NAND_WRITE_nCE       LATBbits.LATB4
#define NAND_WRITE_nCLE      LATBbits.LATB3
#define NAND_WRITE_nALE      LATBbits.LATB2
#define NAND_WRITE_nWE       LATBbits.LATB1
#define NAND_WRITE_nWP       LATBbits.LATB0

#define BLOCK_OF_DEVICE     2048
#define PAGE_OF_BLOCK       64
#define BYTES_OF_PAGE       2048

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 
void init_Nand(void);
void Nand_Write_Command(unsigned char command);
void Nand_Write_Address(unsigned char address);
void Nand_Write_Data(unsigned char write_data);
unsigned char Nand_Read_Data(void);
unsigned int nand_EraseBlock(unsigned long blockAddr);
unsigned int nand_RandomReadData(unsigned long addr,unsigned int readLen,unsigned char *pData);
unsigned int nand_RandomWriteData(unsigned long addr,unsigned int wrLen,unsigned char *pData);

unsigned int nand_ReadBlockInvalid(unsigned long BlockAddr);


//”¶”√
#define SECTION_SIZE		128
#define SEC_PER_BLOCK		1024
#define BLOCK_SIZE			(SECTION_SIZE * SEC_PER_BLOCK)

#define DATA_WR_FLAG_START_ADDR			0x0FA00000
#define DATA_RD_FLAG_START_ADDR			0x0FC00000
#define DATA_FLAG_SIZE_PER_BLOCK		0x00000400

unsigned int getReadPositionInBlock(unsigned int blockNum);
unsigned int getWritePositionInBlock(unsigned int blockNum);
unsigned int nand_WriteOneRecord(unsigned int block,unsigned int section,void *pSrc,unsigned int len);
unsigned int nand_ReadOneRecord(unsigned int block,unsigned int section,void *pDst,unsigned int len);
unsigned int nand_EraseDevice(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

