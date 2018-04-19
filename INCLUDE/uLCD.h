/* 
 * File:   uLCD.h
 * Author: ld
 *
 * Created on 2017年4月6日, 上午9:53
 */

#ifndef ULCD_H
#define	ULCD_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <Generic.h>

/********************************************************************************
*概要:液晶显示驱动头文件
*说明:本驱动适用于JD320240NZ2液晶模块
*作者:刘达
*版本:v1.0
*日期:2014.10.07
*/
//数据引脚定义
#define LCD_WRITE_7           LATEbits.LATE2
#define LCD_WRITE_6           LATEbits.LATE1
#define LCD_WRITE_5           LATEbits.LATE0
#define LCD_WRITE_4           LATFbits.LATF1
#define LCD_WRITE_3           LATFbits.LATF0
#define LCD_WRITE_2           LATDbits.LATD7 
#define LCD_WRITE_1           LATCbits.LATC14
#define LCD_WRITE_0           LATCbits.LATC13

#define LCD_READ_7			PORTEbits.RE2
#define LCD_READ_6			PORTEbits.RE1
#define LCD_READ_5			PORTEbits.RE0
#define LCD_READ_4			PORTFbits.RF1
#define LCD_READ_3			PORTFbits.RF0
#define LCD_READ_2			PORTDbits.RD7 
#define LCD_READ_1			PORTCbits.RC14
#define LCD_READ_0			PORTCbits.RC13
//数据引脚方向寄存器定义
#define LCD_TRIS_7       	TRISEbits.TRISE2
#define LCD_TRIS_6       	TRISEbits.TRISE1
#define LCD_TRIS_5       	TRISEbits.TRISE0
#define LCD_TRIS_4       	TRISFbits.TRISF1
#define LCD_TRIS_3       	TRISFbits.TRISF0
#define LCD_TRIS_2       	TRISDbits.TRISD7 
#define LCD_TRIS_1       	TRISCbits.TRISC14
#define LCD_TRIS_0       	TRISCbits.TRISC13
//控制引脚定义
#define LCD_WRITE_A0                 LATEbits.LATE6       /* PORT for E */
#define LCD_WRITE_WR                 LATEbits.LATE3       /* PORT for RW */
#define LCD_WRITE_RD                 LATEbits.LATE4      /* PORT for RS */
#define LCD_WRITE_CS                 LATEbits.LATE5 
#define LCD_WRITE_RST                LATEbits.LATE7
#define LCD_WRITE_BK                 LATGbits.LATG6

#define LCD_TRIS_A0                TRISEbits.TRISE6    /* TRIS for E */
#define LCD_TRIS_WR                TRISEbits.TRISE3    /* TRIS for RW */
#define LCD_TRIS_RD                TRISEbits.TRISE4   /* TRIS for RS */
#define LCD_TRIS_CS                TRISEbits.TRISE5
#define LCD_TRIS_RST               TRISEbits.TRISE7
#define LCD_TRIS_BK                TRISGbits.TRISG6
/*************************************LCD 命令***********************************/
#define DISP_OFF            0x58 
#define SYSTEM_SET          0x40
#define SLEEP_IN            0x53
#define DISP_ON             0x59
#define SCROLL              0x44
#define CSR_FORM            0x5d
#define CGRAM_ADR           0x50
#define CSR_DIR_RIGHT       0x4c
#define CSR_DIR_LEFT        0x4d
#define CSR_DIR_UP          0x4e
#define CSR_DIR_DOWN        0x4f
#define HDOT_SCR            0x5a
#define OVLAY               0x5b
#define CSRW                0x46
#define CSRR                0x47
#define MWRITE              0x42
#define MREAD               0x43

#define DISP_ROWS_COUNT     30
#define DISP_COLS_COUNT     40
#define GRAPHIC_BASE        0x04b0	//图形显示区首地址
/*************************************LCD 命令值***********************************/
//unsigned char SystemSetting[] ={0x30,0x87,0x07,0x27,0x3a,0xef,0x28,0x00};
//unsigned char ScrollSetting[] = {0x00,0x00,0xf0,0xb0,0x04,0xf0,0x30,0x2a,0xb0,0x4f};

#define SYSTEM_SET_P1		0x30
#define SYSTEM_SET_P2		0x87
#define SYSTEM_SET_P3		0x07
#define SYSTEM_SET_P4		0x28
#define SYSTEM_SET_P5		0x3a
#define SYSTEM_SET_P6		0xef
#define SYSTEM_SET_P7		0x28
#define SYSTEM_SET_P8		0x00
//
#define DSP_OFF_VALUE		0x00
#define DSP_ON_VALUE		0x54
//
#define SCROLL_P1			0x00
#define SCROLL_P2			0x00
#define SCROLL_P3			0xf0
#define SCROLL_P4			0xb0
#define SCROLL_P5			0x04
#define SCROLL_P6			0xf0
#define SCROLL_P7			0x30
#define SCROLL_P8			0x2a
#define SCROLL_P9			0xb0
#define SCROLL_P10			0x4f
//
#define CSR_FORM_VALUE		0x8f07
//
#define HDOT_SCR_VALUE		0x00
#define OVLAY_VALUE			0x00
//
#define BYTES_PER_ROW		((SYSTEM_SET_P8 * 256)+SYSTEM_SET_P7)

#define TEXT_BASEADDR		((SCROLL_P2*256) + SCROLL_P1)
#define TEXT_ROWS			(SCROLL_P3)
#define TEXT_LENGTH			(BYTES_PER_ROW * TEXT_ROWS)
#define GRAPHIC_BASEADDR	((SCROLL_P5*256) + SCROLL_P4)
#define GRAPHIC_ROWS		(SCROLL_P6)
#define GRAPHIC_LENGTH		(BYTES_PER_ROW * GRAPHIC_ROWS)
///////////////////////////////////////////////////////
#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		240
#define SCREEN_ROW_START	0
#define SCREEN_COL_START	0

#define CHAR_WIDTH			8
#define CHAR_HEIGHT			8
#define SCREEN_CHAR_ROW		(SCREEN_HEIGHT / CHAR_HEIGHT)
#define SCREEN_CHAR_COL		(SCREEN_WIDTH / CHAR_WIDTH)
#define BIT_PER_BYTE		8

#define WORD_WIDTH			16
#define WORD_HEIGHT			16
//----------------------------------------------------
#define CHN_WIDTH_BYTES		2
#define CHN_WIDTH			(BIT_PER_BYTE * CHN_WIDTH_BYTES)
#define CHN_HEIGHT_BYTES	2
#define CHN_HEIGHT			(BIT_PER_BYTE * CHN_HEIGHT_BYTES)
#define BYTES_PER_CHN		((CHN_WIDTH * CHN_HEIGHT) / BIT_PER_BYTE)
//----------------------------------------------------
#define ENG_WIDTH_BYTES		1
#define ENG_WIDTH			(BIT_PER_BYTE * ENG_WIDTH_BYTES)
#define ENG_HEIGTH_BYTES	1
#define ENG_HEIGHT			(BIT_PER_BYTE * ENG_HEIGTH_BYTES)
/* Exported functions ------------------------------------------------------- */
void init_LCD(void);

//填充文本区
void fillTextRAM(char data);
//填充图形区
void fillGraphicRAM(char data);

void clear_All_RAM(void);
void clearGraphicRam(int row,int col,int width,int height);
void clearTextRam(int row,int col,int width,int height);


//..........填充显示RAM..........
void fill_All_RAM(void);
//显示一行英文字符
void LCD_WriteCharRow(char wrData,int row,int col,int length);
//显示一列英文字符
void LCD_WriteCharColumn(char wrData,int row,int col,int length);
void LCD_WriteMatrixRow(char **pMatrix,int row,int col,int length,BOOL Inv);
void LCD_WriteMatrix32Row(char **pMatrix,int row,int col,int length,BOOL Inv);
void LCD_WriteCharRow_16_8(char *pData,int row,int col,BOOL Inv);
void LCD_WriteCharRow_8_8(char *pData,int row,int col,BOOL Inv);

void LCD_WriteTextRow(char *pData,int row,int col,int length);
void LCD_WriteMatrixScreen(const char *pMatrix);
void drawHorizontalSolidLine(int RowPixel,int ColPixel,int LenPixel);

void drawVerticalSolidLine(int RowPixel,int ColPixel,int LenPixel);
void drawFrame(int row,int col,int width,int height);
void LCD_WriteBlankSpace(int row,int col,int width,int height,BOOL inv);
void init_Display(void);
void  dis_refresh(void);
#ifdef	__cplusplus
}
#endif

#endif	/* ULCD_H */

