#include <xc.h>
#define SPI1_CS        LATDbits.LATD5
/******************************************************************************/
/* 函数原型：void init_spi1(void)                                              */
/* 功能描述：初始化系统时钟                                                    */
/* 输入参数：无                                                               */
/* 返回参数：无                                                               */
/******************************************************************************/
void init_spi1(void)
{
#if 1
    TRISDbits.TRISD4 =0;
    _RP68R = 0b000101;//MOSI
    TRISDbits.TRISD2=0;
    _RP67R = 0b000110;//clk
    TRISDbits.TRISD1 = 1;
    _SDI1R = 66;        //MISO
    TRISDbits.TRISD5 =0;
    SPI1_CS = 1;
    SPI1CON1bits.PPRE = 0b00;//主分频比64:1
    SPI1CON1bits.SPRE = 0b00;//辅助分频比2:1
    SPI1CON1bits.MSTEN = 0b1;
    SPI1CON1bits.CKP = 0b1;
    SPI1CON1bits.SSEN = 0b0;
    SPI1CON1bits.CKE = 0b0;
    SPI1CON1bits.SMP = 0b1;
    SPI1CON1bits.MODE16 = 0b0;
    SPI1CON1bits.DISSDO = 0b0;
    SPI1CON1bits.DISSCK = 0b0;

    SPI1CON2=0x0000;
    SPI1STAT = 0x8000;
     #else
    // setup the SPI peripheral
    SPI1STAT = 0x0;                             // disable the SPI module (just in case)
    SPI1CON1 = 0x0161;                          // FRAMEN = 0, SPIFSD = 0, DISSDO = 0, MODE16 = 0; SMP = 0; CKP = 1; CKE = 1; SSEN = 0; MSTEN = 1; SPRE = 0b000, PPRE = 0b01
    SPI1CON1bits.CKE = 0x00;
    SPI1CON1bits.CKP = 0x00;
    SPI1STAT = 0x8000;                          // enable the SPI module
    _RP67R=6;
    _SDI1R = 66;//RF3
    _RP68R=5; // RF2
    TRISDbits.TRISD5 =0;
     #endif
}
/******************************************************************************/
/* 函数原型：unsigned char SPI1_WriteByte(unsigned char val)                                            */
/* 功能描述：通过SPI1写一个字节                                                    */
/* 输入参数：val:写入的字节                                                               */
/* 返回参数：同时接收到的数据                                                               */
/******************************************************************************/
unsigned char SPI1_WriteByte(unsigned char val)
{
#if 1
	SPI1BUF = val;                //发送数据
	while(!IFS0bits.SPI1IF);      //等待发送完成
	IFS0bits.SPI1IF = 0;          //清除标志位
        ClrWdt();
	return SPI1BUF;               //读取接收寄存器
#else


    SPI1_CS = 0;                         // lower the slave select line
    SPI1BUF = val;                          // write the data out to the SPI peripheral
    while( !SPI1STATbits.SPIRBF );               // wait for the data to be sent out
#ifdef TEST_MODE
    SentData = SPI1BUF;
#endif
    SPI1_CS = 1;                         // raise the slave select line
#endif
}
/******************************************************************************/
/* 函数原型：unsigned char SPI_ReadByte(void)                                             */
/* 功能描述：通过SPI1读一个字节                                                    */
/* 输入参数：无                                                               */
/* 返回参数：读取的字节                                                               */
/******************************************************************************/
unsigned char SPI1_ReadByte(void)
{
	SPI1BUF = 0xFF;               //发送数据,启动数据接收
	while(!IFS0bits.SPI1IF);      //等待接收完成
	IFS0bits.SPI1IF = 0;          //清除标志位
        ClrWdt();
	return SPI1BUF;               //读取接收到数据

}
//*****************发送命令函数***************************
unsigned char SD_SendCommand(unsigned char cmd,unsigned long arg)
{
	unsigned char r1;
	unsigned char retry=0;       //重复操作次数

	SPI1_CS = 0;                   //使能片选信号

	SPI1_WriteByte(cmd | 0x40);   //分别写入命令
	SPI1_WriteByte(arg>>24);      //数据段第4字节
	SPI1_WriteByte(arg>>16);      //数据段第3字节
	SPI1_WriteByte(arg>>8);       //数据段第2字节
	SPI1_WriteByte(arg);          //数据段第1字节
	SPI1_WriteByte(0x95);         //CRC效验和


	while((r1 = SPI1_WriteByte(0xff)) == 0xff)//等待响应
	if(retry++ > 200) break;     //超时退出

	SPI1_CS = 1;                   //清初片选信号
	ClrWdt();
	return r1;                   //返回状态值
}
//******************SD 开初始化函数***********************
unsigned char sd_reset(void)
{
	unsigned char i;
	unsigned char retry = 0;           //重复次数
	unsigned char r1 = 0;
	do
	{
		SPI1_CS = 0;                         //SD片选
		for(i=0;i<100;i++) SPI1_WriteByte(0xFF);

		r1 = SD_SendCommand(0,0);          //发idle命令
		retry++;
		if(retry>20) return 1;             //超时退出
	} while(r1 != 0x01);                   //等待IDLE命令返回

	retry = 0;
	SPI1_CS = 0;                             //SD片选
	do
	{
		for(i=0;i<100;i++) SPI1_WriteByte(0xff);

		r1 = SD_SendCommand(1, 0);         //Active命令
		retry++;
		if(retry>254) return 1;            //超时退出
	} while(r1);

	for(i=0;i<100;i++) SPI1_WriteByte(0xff);

	r1 = SD_SendCommand(59, 0);            //关 CRC
	if(r1) return 1;                       //返回不正确,退出初始化

	for(i=0;i<100;i++) SPI1_WriteByte(0xff);

	r1 = SD_SendCommand(16, 512);          //设扇区大小 512
	if(r1 != 0) return 1;                  //返回不正确,退出初始化
	ClrWdt();
	return 0;                              //正常返回
}

//********************写一个扇区**************************
unsigned char SD_WriteSingleBlock(unsigned long sector,unsigned char *pchar)
{
	unsigned char r1;
	unsigned int i;
	unsigned char retry=0;
	unsigned char *p;
	do
	{
		for(i=0;i<100;i++) SPI1_WriteByte(0xff);

		r1 = SD_SendCommand(24, sector<<9);//写命令
		retry++;
		if(retry>10) return 1;             //超时退出
	} while(r1 != 0x00);

	SPI1_CS = 0;

	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);

	SPI1_WriteByte(0xfe);                  //发开始符
   	p = pchar;  //读取结构体首地址
 	for(i=0; i<512; i++)                  //送512字节数据
 	{

		SPI1_WriteByte(*p);       //发送存储数据
		 p++;
	}

	SPI1_WriteByte(0x95);
	SPI1_WriteByte(0x95);                 //16-bits CRC

	r1 = SPI1_WriteByte(0xff);            //读响应位
	if(retry++ >10) return 1;            //超时退出
	while(!((r1&0x0f)==5));              //等待数据成功接受返回信息
	while(!(SPI1_WriteByte(0xff)));       //等待SD卡内部编程完成
    ClrWdt();
	return 0;
}


//******************读SD卡一个扇区************************
unsigned char SD_ReadSingleBlock(unsigned long sector,unsigned char *pchar)
{
	unsigned int  i;
	unsigned char r1,retry=0;
	unsigned char *k;
	do
	{
		r1 = SD_SendCommand(17, sector<<9);//读命令
		retry++;
		if(retry>10) return 1;             //超时退出
	} while(r1 != 0x00);
	SPI1_CS = 0;
	while(SPI1_WriteByte(0xff)!= 0xfe)      //等待接收到开始字节
	{
		if(retry++ >100) return 1;         //超时退出
	}
	k = pchar;

	for(i=0; i<=512; i++)                   //读整个扇区数据
	{
		*k = SPI1_WriteByte(0xff);   //读取接收到的数据
		k++;
	}

	SPI1_WriteByte(0xff);                   //伪16-bits crc
	SPI1_WriteByte(0xff);

	SPI1_CS = 1;
	ClrWdt();
	return 0;
}
void init_sd(void)
{
    init_spi1();
    sd_reset();
}