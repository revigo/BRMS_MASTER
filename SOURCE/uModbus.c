#include "uMBcrc.h"
#include "uModbus.h"
#include "uInit.h"
#include <stddef.h>

unsigned int slaveInputRegisters[16];//当前从机输入寄存器组，从地址0X0000开始
unsigned int slaveHoldingRegisters[16];//当前从机保持寄存器组,从地址0X0000开始
unsigned int slaveCoilRegisters[16];//当前从机线圈寄存器组，从地址0x0000开始
unsigned char slaveOffline[SLAVE_NUM]={0};
unsigned char slaveNewLight[SLAVE_NUM];
sSysRecords gCurSysRecord;

BOOL sendReqCommand = FALSE;
static CommCommand circuteCommand ={REQ_STATE_NONE,1,MB_FUNC_READ_INPUT_REGISTER,0x0001,0x0001,0x0000,NULL};
static CommCommand rCommand;
static CommCommand *reqCommand=NULL;
static uint8_t commRepeat=0;
BOOL commIsAllowed = TRUE;
//static BOOL resetCircuteCmd=FALSE;
/*****************************************************************************
 * 各发送函数
 *****************************************************************************/

/**
 * This function will request read input register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqReadInputRegister( uint8_t ucSndAddr, uint16_t usRegAddr, uint16_t usNRegs )
{
    uint16_t                 crc16;
    uint16_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_READ_INPUT_REGISTER;
        usSndFrame[pos++]=usRegAddr >> 8;
        usSndFrame[pos++]=usRegAddr;
        usSndFrame[pos++]=usNRegs >> 8;
        usSndFrame[pos++]=usNRegs;
        crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16 ;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
//******************************输入寄存器回调函数**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResReadInputCB( uint8_t *usAddress, uint16_t usRegAddr,uint16_t usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
	uint16_t *pRecvReg=&slaveInputRegisters[usRegAddr];
    uint16_t respondDataLen = usNRegs *2 + 3;
    uint16_t crc16,recvCrc16;
    uint8_t pos;
    uint16_t i;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
    crc16 = usMBCRC16(usRecvFrame,respondDataLen);
    recvCrc16 = usRecvFrame[respondDataLen];
    recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
    if(crc16!=recvCrc16)
    {
        eStatus = MB_EDATA;
    }
    else
    {
		*usAddress = usRecvFrame[0];
        pos = 3;
        for(i=0;i<usNRegs;i++)
        {
			//高字节在前
            *pRecvReg = usRecvFrame[pos+i*2];
            *pRecvReg = (*pRecvReg)<<8;
            *pRecvReg = (*pRecvReg) + usRecvFrame[pos+i*2+1];
            pRecvReg++;
        }
        eStatus = MB_ENOERR;
    }
    return eStatus;
}
//******************************输入寄存器回调函数仅供循环检测用**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResReadInputStateCB( uint8_t *usAddress, uint16_t *recvRegs,uint16_t usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR; 
    uint16_t respondDataLen = usNRegs *2 + 3;
    uint16_t crc16,recvCrc16;
    uint8_t pos;
    uint16_t i;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
    crc16 = usMBCRC16(usRecvFrame,respondDataLen);
    recvCrc16 = usRecvFrame[respondDataLen];
    recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
    if(crc16!=recvCrc16)
    {
        eStatus = MB_EDATA;
    }
    else
    {
		*usAddress = usRecvFrame[0];
        pos = 3;
        for(i=0;i<usNRegs;i++)
        {
			//高字节在前
            *recvRegs = usRecvFrame[pos+i*2];
            *recvRegs = (*recvRegs)<<8;
            *recvRegs = (*recvRegs) + usRecvFrame[pos+i*2+1];
            recvRegs++;
        }
        eStatus = MB_ENOERR;
    }
    return eStatus;
}

/**
 * This function will request write holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usRegData register data to be written
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqWriteHoldingRegister( uint8_t ucSndAddr, uint16_t usRegAddr, uint16_t usRegData)
{
     uint16_t                 crc16;
    uint8_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_WRITE_REGISTER;
        usSndFrame[pos++]=usRegAddr >> 8;
        usSndFrame[pos++]=usRegAddr;
        usSndFrame[pos++]=usRegData >> 8;
        usSndFrame[pos++]=usRegData;
        crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
//******************************写单独保持寄存器响应函数**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResWriteHoldingCB( void)
{
	eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t respondDataLen = 6;
    uint16_t crc16,recvCrc16;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
	crc16 = usMBCRC16(usRecvFrame,respondDataLen);
	recvCrc16 = usRecvFrame[respondDataLen];
	recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
	if(crc16!=recvCrc16)
	{
	    eStatus = MB_EDATA;
	}
	else
	{
	    eStatus = MB_ENOERR;
	}
    return eStatus;
}

/**
 * This function will request write multiple holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param pusDataBuffer data to be written
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqWriteMultipleHoldingRegister( uint8_t ucSndAddr,uint16_t usRegAddr, uint16_t usNRegs, uint16_t * pusDataBuffer )
{
    uint16_t                 usRegIndex = 0;
     uint16_t                 crc16;
    uint8_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_WRITE_MULTIPLE_REGISTERS;
        usSndFrame[pos++]=usRegAddr >> 8;
        usSndFrame[pos++]=usRegAddr;
        usSndFrame[pos++]=usNRegs >> 8;
        usSndFrame[pos++]=usNRegs;
        usSndFrame[pos++]    = usNRegs * 2;
	while( usNRegs > usRegIndex)
	{
            //高字节在前
            usSndFrame[pos++] = pusDataBuffer[usRegIndex] >> 8 ;
			usSndFrame[pos++] = pusDataBuffer[usRegIndex] ;
			usRegIndex++;
			
        }
	crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
//******************************写多个保持寄存器响应函数**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResWriteMultipleHoldingCB( void )
{
	eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t respondDataLen = 6;
    uint16_t crc16,recvCrc16;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
	crc16 = usMBCRC16(usRecvFrame,respondDataLen);
	recvCrc16 = usRecvFrame[respondDataLen];
	recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
	if(crc16!=recvCrc16)
	{
	    eStatus = MB_EDATA;
	}
	else
	{
	    eStatus = MB_ENOERR;
	}
    return eStatus;
}

/**
 * This function will request read holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqReadHoldingRegister( uint8_t ucSndAddr, uint16_t usRegAddr, uint16_t usNRegs )
{
    uint16_t                 crc16;
    uint8_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_READ_HOLDING_REGISTER;
        usSndFrame[pos++]=usRegAddr >> 8;
        usSndFrame[pos++]=usRegAddr;
        usSndFrame[pos++]=usNRegs >> 8;
        usSndFrame[pos++]=usNRegs;
        crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
//******************************读保持寄存器响应函数**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResReadHoldingCB( uint16_t usRegAddr,uint16_t usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
	uint16_t *pRecvReg=&slaveHoldingRegisters[usRegAddr];
    uint16_t respondDataLen = usNRegs *2 + 3;
    uint16_t crc16,recvCrc16;
    uint8_t pos;
    uint16_t i;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
    crc16 = usMBCRC16(usRecvFrame,respondDataLen);
    recvCrc16 = usRecvFrame[respondDataLen];
    recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
    if(crc16!=recvCrc16)
    {
        eStatus = MB_EDATA;
    }
    else
    {
        pos = 3;
        for(i=0;i<usNRegs;i++)
        {
			//高字节在前
            *pRecvReg = usRecvFrame[pos+i*2];//
            *pRecvReg = (*pRecvReg)<<8;
            *pRecvReg = (*pRecvReg) + usRecvFrame[pos+i*2+1];
            pRecvReg++;
        }
        eStatus = MB_ENOERR;
    }
    return eStatus;
}

/**
 * This function will request read coil.
 *
 * @param ucSndAddr salve address
 * @param usCoilAddr coil start address
 * @param usNCoils coil total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqReadCoils( uint8_t ucSndAddr, uint16_t usCoilAddr, uint16_t usNCoils)
{
   uint16_t                 crc16;
    uint8_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_READ_COILS;
        usSndFrame[pos++]=usCoilAddr >> 8;
        usSndFrame[pos++]=usCoilAddr;
        usSndFrame[pos++]=usNCoils >> 8;
        usSndFrame[pos++]=usNCoils;
        crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
//******************************读线圈寄存器响应函数**********************************
//函数定义: eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2013-11-25    Company: BXXJS
//**********************************************************************************
eMBErrorCode
ePBResReadCoilsCB( uint16_t usCoilAddr,uint16_t usNCoils )
{
	eMBErrorCode    eStatus = MB_ENOERR;
        return eStatus;
}

/**
 * This function will request read coil.
 *
 * @param ucSndAddr salve address
 * @param usCoilAddr coil start address
 * @param usNCoils coil total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
ePBMasterReqWriteSingleCoil( uint8_t ucSndAddr, uint16_t usCoilAddr, uint16_t usCoilReg)
{
   uint16_t                 crc16;
    uint8_t pos=0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
	uint8_t *usSndFrame;
    if ( ucSndAddr > 254 ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( getPowerbusMasterEvent() != EV_MASTER_IDLE ) eErrStatus = MB_MRE_MASTER_BUSY;
	else if ( usCoilReg!=0xFF00 && usCoilReg!=0x0000) eErrStatus = MB_MRE_ILL_ARG;
    else
    {
		vMBMasterGetRTUSndBuf(&usSndFrame);
        usSndFrame[pos++]=ucSndAddr;
        usSndFrame[pos++]=MB_FUNC_WRITE_SINGLE_COIL;
        usSndFrame[pos++]=usCoilAddr >> 8;
        usSndFrame[pos++]=usCoilAddr;
        usSndFrame[pos++]=usCoilReg >> 8;
        usSndFrame[pos++]=usCoilReg;
        crc16=usMBCRC16(usSndFrame,pos);
        usSndFrame[pos++]=crc16 >> 8;
        usSndFrame[pos++]=crc16;
        PowerbusPutBytes(pos);
        eErrStatus = MB_MRE_NO_ERR;
    }
    return eErrStatus;
}
eMBErrorCode
ePBResWriteSingleCoilCB(void)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t respondDataLen = 6;
    uint16_t crc16,recvCrc16;
	uint8_t *usRecvFrame;
	vMBMasterGetRTURecvBuf(&usRecvFrame);
	crc16 = usMBCRC16(usRecvFrame,respondDataLen);
	recvCrc16 = usRecvFrame[respondDataLen];
	recvCrc16 = (recvCrc16<<8) + usRecvFrame[respondDataLen+1];
	if(crc16!=recvCrc16)
	{
	    eStatus = MB_EDATA;
	}
	else
	{
	    eStatus = MB_ENOERR;
	}
    return eStatus;
}
void modbusReqestCommunicate(CommCommand *cmd)
{
	reqCommand=cmd;
}

void modbusSendCircuteCommand(void)
{
	/*
	if(resetCircuteCmd)
	{
		circuteCommand.ucAddr = 1;
		commRepeat=0;
		resetCircuteCmd=FALSE;
	}
	*/
	if(slaveNewLight[circuteCommand.ucAddr-1]!=gCurSysRecord.slaveLight[circuteCommand.ucAddr-1])
	{
		rCommand.ucAddr =circuteCommand.ucAddr;
		rCommand.Funcs = MB_FUNC_WRITE_SINGLE_COIL;
		rCommand.usRegAddr = 0x0000;
		rCommand.usRegData= (slaveNewLight[circuteCommand.ucAddr-1]==0)?0xFF00:0x0000;		
		rCommand.reqCmdState = REQ_STATE_PREPARE;
		reqCommand=&rCommand;
		if(circuteCommand.ucAddr >=SLAVE_NUM)
		{
			circuteCommand.ucAddr=1;
		}
		else
		{
			circuteCommand.ucAddr++;
		}
	}
	else if(ePBMasterReqReadInputRegister(circuteCommand.ucAddr,circuteCommand.usRegAddr,circuteCommand.usNRegs)==MB_MRE_NO_ERR)
	{
		sendReqCommand = FALSE;
	}

}
void send_rs232_response_Lights_Mannual(unsigned int regdata);
void modbusRecvTimeout(void)
{
	if(!sendReqCommand)
	{
		if(slaveOffline[circuteCommand.ucAddr-1]==1)
		{
			if(circuteCommand.ucAddr >= SLAVE_NUM)
			{
				circuteCommand.ucAddr=1;
			}
			else
			{
				circuteCommand.ucAddr++;
			}
			commRepeat = 0;
		}
		else if(commRepeat >= COMM_TIMEOUT_CONFIRM_CNT)
		{
			slaveOffline[circuteCommand.ucAddr-1]=1;
			if(circuteCommand.ucAddr >= SLAVE_NUM)
			{
				circuteCommand.ucAddr=1;
			}
			else
			{
				circuteCommand.ucAddr++;
			}
			commRepeat = 0;

		}
		else
		{
			commRepeat++;
		}
	}
	else
	{
		/*
		
		if((reqCommand->ucAddr==0x00)&&(reqCommand->Funcs = MB_FUNC_WRITE_SINGLE_COIL))
		{
			send_rs232_response_Lights_Mannual(reqCommand->usRegData);
			if(reqCommand->usRegData==0x0000) set_AllLightsSwitch(1);
			else
				set_AllLightsSwitch(0);
		}*/
		reqCommand->reqCmdState= REQ_STATE_TIMEOUT;
	}
	resetPowerbusState();
}
eMBErrorCode modbusRecvCircuteResponse(void)
{
	uint8_t recvAddr;
	uint16_t recvRegs;
	eMBErrorCode err = ePBResReadInputStateCB(&recvAddr,&recvRegs,0x0001);
	if(err == MB_ENOERR)
	{
            slaveOffline[circuteCommand.ucAddr-1]=0;
            gCurSysRecord.slavePulse[recvAddr-1]+=recvRegs;

		if(recvAddr==circuteCommand.ucAddr)
		{
			if(circuteCommand.ucAddr >=SLAVE_NUM)
			{
				circuteCommand.ucAddr=1;
			}
			else
			{
				circuteCommand.ucAddr++;
			}
			commRepeat=0;
		}
	}
	else
	{
		if(commRepeat >= COMM_TIMEOUT_CONFIRM_CNT)
		{
                    slaveOffline[circuteCommand.ucAddr-1]=1;
			if(circuteCommand.ucAddr >= SLAVE_NUM)
			{
				circuteCommand.ucAddr=1;
			}
			else
			{
				circuteCommand.ucAddr++;
			}
			commRepeat = 0;
		}
		else
		{
			commRepeat++;
		}
	}
	return err;
}
void modbusRecvRecieved(void)
{
	eMBErrorCode err;
	uint8_t recvAddr;
//	uint8_t recvFunc;
//	uint8_t *usRecvFrame;
	if(sendReqCommand)
	{
		switch(reqCommand->Funcs)
		{
			case MB_FUNC_READ_INPUT_REGISTER:
				err=ePBResReadInputCB(&recvAddr,reqCommand->usRegAddr,reqCommand->usNRegs);
				break;
			case MB_FUNC_READ_HOLDING_REGISTER:
				err=ePBResReadHoldingCB(reqCommand->usRegAddr,reqCommand->usNRegs);
				break;
			case MB_FUNC_READ_COILS:
				err=ePBResReadCoilsCB(reqCommand->usNRegs,reqCommand->usNRegs);
				break;
			case MB_FUNC_WRITE_REGISTER:
				err=ePBResWriteHoldingCB();
				break;
			case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
				err=ePBResWriteMultipleHoldingCB();
				break;
			case MB_FUNC_WRITE_SINGLE_COIL:
				err=ePBResWriteSingleCoilCB();
				break;
			default:err = MB_EFUNC;break;
		}
		if(err == MB_ENOERR)
		{
			reqCommand->reqCmdState= REQ_STATE_RECIEVED;
		}
		else
		{
			reqCommand->reqCmdState=REQ_STATE_ERR;
		}
	}
	else
	{
		err = modbusRecvCircuteResponse();
	}
	/*
	err = modbusRecvCircuteResponse();
	if(err != MB_ENOERR)
	{
		if(sendReqCommand)
		{
			vMBMasterGetRTURecvBuf(&usRecvFrame);
			recvFunc = usRecvFrame[MB_RTU_FUNC_OFF];
			switch(recvFunc)
			{
				case MB_FUNC_READ_INPUT_REGISTER:
					err=ePBResReadInputCB(&recvAddr,reqCommand->usRegAddr,reqCommand->usNRegs);
					break;
				case MB_FUNC_READ_HOLDING_REGISTER:
					err=ePBResReadHoldingCB(reqCommand->usRegAddr,reqCommand->usNRegs);
					break;
				case MB_FUNC_READ_COILS:
					err=ePBResReadCoilsCB(reqCommand->usNRegs,reqCommand->usNRegs);
					break;
				case MB_FUNC_WRITE_REGISTER:
					err=ePBResWriteHoldingCB();
					break;
				case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
					err=ePBResWriteMultipleHoldingCB();
					break;
				case MB_FUNC_WRITE_SINGLE_COIL:
					err=ePBResWriteSingleCoilCB();
					break;
				default:err = MB_EFUNC;break;
			}
			if(err == MB_ENOERR)
			{
				reqCommand->reqCmdState= REQ_STATE_RECIEVED;
			}
			else
			{
				reqCommand->reqCmdState=REQ_STATE_ERR;
			}
		}
		else
		{
				if(commRepeat >= COMM_TIMEOUT_CONFIRM_CNT)
				{
					if((slavesState[circuteCommand.ucAddr-1].machType & 0xff00)!=0xff00)
					{
						slavesState[circuteCommand.ucAddr-1].machType |=0xff00;
						addFaultInfo(circuteCommand.ucAddr,BUWEI_TONGXIN,ALARM_DX);
					}
					if(circuteCommand.ucAddr >= systemParams.slaveNum)
					{
						circuteCommand.ucAddr=1;
					}
					else
					{
						circuteCommand.ucAddr++;
					}
					commRepeat = 0;

				}
				else
				{
					commRepeat++;
				}
		}
	}
	*/
	resetPowerbusState();

}
void modbusRecvResponse(void)
{
	eMBMasterEventType event = getPowerbusMasterEvent();
	if(event==EV_MASTER_FRAME_RECIEVED)
	{
		modbusRecvRecieved();
	}
	else if(event ==EV_MASTER_FRAME_TIME_OUT)
	{
		modbusRecvTimeout();
	}
}
extern void set_StartLoopTime(unsigned char SaveCircle);
void start_NewCircle(void)
{
	int i;
	gCurSysRecord.loopTime=gRunParams.slaveSaveInv;
	transfer_BufStructToArrayBCD(&PCF8563_Buf,gCurSysRecord.timeBCD);
	for(i=0;i<SLAVE_NUM;i++)
    {
        gCurSysRecord.slavePulse[i]=0;
		//gCurSysRecord.slaveLight[i]=0;
    }
	set_StartLoopTime(gRunParams.slaveSaveInv);
}
void set_AllLightsSwitch(unsigned int ison)
{
	int i;
	unsigned char value=(ison==17)?0:1;
	for(i=0;i<SLAVE_NUM;i++)
	{
		gCurSysRecord.slaveLight[i]=value;
	}
}
void init_Modbus(unsigned long int ulBaudRate)
{
    int i;
    init_PBUS(ulBaudRate);
    for(i=0;i<SLAVE_NUM;i++)
    {
        slaveOffline[i]=0;
		slaveNewLight[i]=0;
		gCurSysRecord.slavePulse[i]=0;
		gCurSysRecord.slaveLight[i]=0;
		
    }
}
void setNewLightsState(void)
{
	time_t curTime_t,diff_t;
	SLAVE_CONTROL *pctl;
	unsigned long loop,temp,lightTimes;
	int i;
	if(gRunParams.autoControl==1)
	{
		curTime_t=get_mktime(&PCF8563_Buf);
		for(i=0;i<SLAVE_NUM;i++)
		{
			pctl=&gRunParams.slaveControls[i];
                        lightTimes=(unsigned long )pctl->loopTime;
			if(compare_time(curTime_t,pctl->startTimeSec)!=-1)
			{
                            if(pctl->loopTime==0x0000)
                            {
				diff_t=curTime_t - pctl->startTimeSec;
				loop=(pctl->light_on_time+pctl->light_off_time);
				temp=diff_t%loop;
				if(temp < pctl->light_on_time)
					slaveNewLight[i]=1;
				else
					slaveNewLight[i]=0;
                            }
                            else
                            {
                                diff_t=curTime_t - pctl->startTimeSec;
				loop=(pctl->light_on_time+pctl->light_off_time);
                                temp=diff_t/loop;
                                if(temp<lightTimes)
                                {
                                    temp=diff_t%loop;
                                    if(temp < pctl->light_on_time)
					slaveNewLight[i]=1;
                                    else
					slaveNewLight[i]=0;
                                }
                                else
                                {
                                    slaveNewLight[i]=0;
                                }
                            }
			}
			else
			{
				slaveNewLight[i]=0;
			}
		}
	}
}
void modbusSendReqCommand(void)
{
	eMBMasterReqErrCode err;
	switch(reqCommand->Funcs)
	{
		case MB_FUNC_READ_INPUT_REGISTER:
				err=ePBMasterReqReadInputRegister(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usNRegs);
				break;
		case MB_FUNC_READ_HOLDING_REGISTER:
				err=ePBMasterReqReadHoldingRegister(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usNRegs);
				break;
		case MB_FUNC_READ_COILS:
				err=ePBMasterReqReadCoils(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usNRegs);
				break;
		case MB_FUNC_WRITE_REGISTER:
				err=ePBMasterReqWriteHoldingRegister(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usRegData);
				break;
		case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
				err=ePBMasterReqWriteMultipleHoldingRegister(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usNRegs,reqCommand->usRegs);
				break;
		case MB_FUNC_WRITE_SINGLE_COIL:
				err=ePBMasterReqWriteSingleCoil(reqCommand->ucAddr,reqCommand->usRegAddr,reqCommand->usRegData);
				break;
		default:err=MB_MRE_EXE_FUN;
	}
	if(err==MB_MRE_NO_ERR)
	{
		reqCommand->reqCmdState= REQ_STATE_SENT;
		sendReqCommand = TRUE;
	}
}
void modbusPoll(void)
{

    eMBMasterEventType event = getPowerbusMasterEvent();
    if(event==EV_MASTER_IDLE)
    {
	//if((SystemTickCnt-sendTickFlag)>100)
	//{
		if((reqCommand !=NULL)&&(reqCommand->reqCmdState== REQ_STATE_PREPARE))
		{
				modbusSendReqCommand();
		}
		else //if(pageFuncs == &pageMain)
		{
			modbusSendCircuteCommand();
		//	sendTickFlag=SystemTickCnt;

		}
	//}
    }
    else if(event==EV_MASTER_FRAME_RECIEVED)
	{
		modbusRecvRecieved();
		if((reqCommand!=NULL)&&(sendReqCommand))
		{
			if(reqCommand->reqCmdState==REQ_STATE_RECIEVED)
			{
				gCurSysRecord.slaveLight[reqCommand->ucAddr-1]=slaveNewLight[reqCommand->ucAddr-1];
			}
			reqCommand=NULL;
		}
	}
    else if(event ==EV_MASTER_FRAME_TIME_OUT)
	{
		modbusRecvTimeout();
	}

}