/* 
 * File:   uRS232.h
 * Author: ld
 *
 * Created on 2017��4��14��, ����1:32
 */

#ifndef URS232_H
#define	URS232_H

#ifdef	__cplusplus
extern "C" {
#endif
typedef enum{
    RS232_IDLE                    	= 1<<0,  /*!< ����. */
    RS232_FRAME_SENDING           	= 1<<1,  /*!< ���ڷ���. */
    RS232_FRAME_SENT                = 1<<2,  /*!< ������� */
    RS232_FRAME_WAIT_RESPOND        = 1<<3,  /*!< �ȴ���Ӧ. */
    RS232_FRAME_RECIEVING           = 1<<4,  /*!< ���ڽ���. */
    RS232_FRAME_RECIEVED           	= 1<<5, /*!< �������. */
    RS232_FRAME_TIME_OUT			= 1<<6 /*���ճ�ʱ*/
}RS232_STATE;
void init_RS232( unsigned long usbaudrate );
void setRS232State(RS232_STATE stat);
RS232_STATE getRS232State(void);
void send_RS232(unsigned int len);
unsigned int recv_RS232(unsigned char *buffer);
#ifdef	__cplusplus
}
#endif

#endif	/* URS232_H */

