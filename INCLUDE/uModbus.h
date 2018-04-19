#ifndef MODBUS__H__
#define MODBUS__H__
#include <xc.h>
#include <stdint.h>
#include <generic.h>


#define FAULT_LX_POSITION					0x00

#define COMM_SLAVE_STATE_ADDR               0x0000
#define COMM_SLAVE_STATE_NREGS              0x0004

#define SLAVE_RESET_REG_ADDR				0x0000
#define SLAVE_CLEAR_ALARM_REG_ADDR			0x0001
#define COMM_TIMEOUT_CONFIRM_CNT            1
/* ----------------------- Defines (func)------------------------------------------*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )
//
#define MB_RTU_ADDR_OFF     0       /*!< Offset of slave address in Ser-RTU. */
#define MB_RTU_FUNC_OFF      1       /*!< RTU_FUNC OFFSET. */

typedef enum
{
    EV_MASTER_IDLE                    	= 1<<0,  /*!< 空闲. */
    EV_MASTER_FRAME_SENDING           	= 1<<1,  /*!< 正在发送. */
    EV_MASTER_FRAME_SENT                = 1<<2,  /*!< 发送完成 */
    EV_MASTER_FRAME_WAIT_RESPOND        = 1<<3,  /*!< 等待响应. */
    EV_MASTER_FRAME_RECIEVING           = 1<<4,  /*!< 正在接收. */
    EV_MASTER_FRAME_RECIEVED           	= 1<<5, /*!< 接收完成. */
    EV_MASTER_FRAME_TIME_OUT			= 1<<6 /*接收超时*/
} eMBMasterEventType;
typedef enum
{
	EV_ERROR_NONE,						/*无故障*/
    EV_ERROR_RESPOND_TIMEOUT,         /*!< 等待响应超时. */
    EV_ERROR_RECEIVE_DATA,            /*!< 接收数据错误. */
    EV_ERROR_EXECUTE_FUNCTION,        /*!< 执行错误. */
} eMBMasterErrorEventType;
typedef enum
{
    MB_MRE_NO_ERR,                  /*!< no error. */
    MB_MRE_NO_REG,                  /*!< illegal register address. */
    MB_MRE_ILL_ARG,                 /*!< illegal argument. */
    MB_MRE_REV_DATA,                /*!< receive data error. */
    MB_MRE_TIMEDOUT,                /*!< timeout error occurred. */
    MB_MRE_MASTER_BUSY,             /*!< master is busy now. */
    MB_MRE_EXE_FUN                  /*!< execute function error. */
} eMBMasterReqErrCode;
typedef enum
{
    MB_ENOERR,                  /*!< no error. */
    MB_ENOREG,                  /*!< illegal register address. */
    MB_EINVAL,                  /*!< illegal argument. */
    MB_EPORTERR,                /*!< porting layer error. */
    MB_ENORES,                  /*!< insufficient resources. */
    MB_EIO,                     /*!< I/O error. */
    MB_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT,                /*!< timeout error occurred. */
    MB_EDATA,
    MB_EFUNC,
    MB_EADDR

} eMBErrorCode;
typedef enum{
	REQ_STATE_NONE,
	REQ_STATE_PREPARE,
	REQ_STATE_SENT,
	REQ_STATE_RECIEVED,
	REQ_STATE_TIMEOUT,
	REQ_STATE_ERR,
}ReqCmdState;
typedef struct{
	ReqCmdState reqCmdState;
	uint8_t ucAddr;
	uint8_t Funcs;
	uint16_t usRegAddr;
	uint16_t usNRegs;
	uint16_t usRegData;
	uint16_t *usRegs;
}CommCommand;

/* ----------------------- Start implementation -----------------------------*/

unsigned char init_PBUS( unsigned long int ulBaudRate );
void PowerbusEnable(unsigned char xRxEnable, unsigned char xTxEnable);
void PowerbusClose(void);
unsigned char isPowerbusShortcircuited(void);
void resetPowerbusState(void);
unsigned char PowerbusPutBytes(unsigned int count);

unsigned char PowerbusGetBytes(unsigned char * pucBytes,unsigned int *count);
eMBMasterEventType getPowerbusMasterEvent(void);
void vMBMasterGetRTUSndBuf( unsigned char ** pucFrame );
void vMBMasterGetRTURecvBuf( unsigned char ** pucFrame );
void modbusSendCircuteCommand(void);
void modbusReqestCommunicate(CommCommand *cmd);
void init_Modbus(unsigned long int ulBaudRate);
void modbusPoll(void);
void start_NewCircle(void);
void setNewLightsState(void);

#endif

