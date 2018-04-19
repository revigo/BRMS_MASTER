/* 
 * File:   uSDcard.h
 * Author: ld
 *
 * Created on 2017年9月28日, 上午8:41
 */

#ifndef USDCARD_H
#define	USDCARD_H

#ifdef	__cplusplus
extern "C" {
#endif


void init_sd(void);
unsigned char SD_WriteSingleBlock(unsigned long sector,unsigned char *pchar);
unsigned char SD_ReadSingleBlock(unsigned long sector,unsigned char *pchar);
#ifdef	__cplusplus
}
#endif

#endif	/* USDCARD_H */

