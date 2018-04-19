#include <xc.h>
#define SPI1_CS        LATDbits.LATD5
/******************************************************************************/
/* ����ԭ�ͣ�void init_spi1(void)                                              */
/* ������������ʼ��ϵͳʱ��                                                    */
/* �����������                                                               */
/* ���ز�������                                                               */
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
    SPI1CON1bits.PPRE = 0b00;//����Ƶ��64:1
    SPI1CON1bits.SPRE = 0b00;//������Ƶ��2:1
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
/* ����ԭ�ͣ�unsigned char SPI1_WriteByte(unsigned char val)                                            */
/* ����������ͨ��SPI1дһ���ֽ�                                                    */
/* ���������val:д����ֽ�                                                               */
/* ���ز�����ͬʱ���յ�������                                                               */
/******************************************************************************/
unsigned char SPI1_WriteByte(unsigned char val)
{
#if 1
	SPI1BUF = val;                //��������
	while(!IFS0bits.SPI1IF);      //�ȴ��������
	IFS0bits.SPI1IF = 0;          //�����־λ
        ClrWdt();
	return SPI1BUF;               //��ȡ���ռĴ���
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
/* ����ԭ�ͣ�unsigned char SPI_ReadByte(void)                                             */
/* ����������ͨ��SPI1��һ���ֽ�                                                    */
/* �����������                                                               */
/* ���ز�������ȡ���ֽ�                                                               */
/******************************************************************************/
unsigned char SPI1_ReadByte(void)
{
	SPI1BUF = 0xFF;               //��������,�������ݽ���
	while(!IFS0bits.SPI1IF);      //�ȴ��������
	IFS0bits.SPI1IF = 0;          //�����־λ
        ClrWdt();
	return SPI1BUF;               //��ȡ���յ�����

}
//*****************���������***************************
unsigned char SD_SendCommand(unsigned char cmd,unsigned long arg)
{
	unsigned char r1;
	unsigned char retry=0;       //�ظ���������

	SPI1_CS = 0;                   //ʹ��Ƭѡ�ź�

	SPI1_WriteByte(cmd | 0x40);   //�ֱ�д������
	SPI1_WriteByte(arg>>24);      //���ݶε�4�ֽ�
	SPI1_WriteByte(arg>>16);      //���ݶε�3�ֽ�
	SPI1_WriteByte(arg>>8);       //���ݶε�2�ֽ�
	SPI1_WriteByte(arg);          //���ݶε�1�ֽ�
	SPI1_WriteByte(0x95);         //CRCЧ���


	while((r1 = SPI1_WriteByte(0xff)) == 0xff)//�ȴ���Ӧ
	if(retry++ > 200) break;     //��ʱ�˳�

	SPI1_CS = 1;                   //���Ƭѡ�ź�
	ClrWdt();
	return r1;                   //����״ֵ̬
}
//******************SD ����ʼ������***********************
unsigned char sd_reset(void)
{
	unsigned char i;
	unsigned char retry = 0;           //�ظ�����
	unsigned char r1 = 0;
	do
	{
		SPI1_CS = 0;                         //SDƬѡ
		for(i=0;i<100;i++) SPI1_WriteByte(0xFF);

		r1 = SD_SendCommand(0,0);          //��idle����
		retry++;
		if(retry>20) return 1;             //��ʱ�˳�
	} while(r1 != 0x01);                   //�ȴ�IDLE�����

	retry = 0;
	SPI1_CS = 0;                             //SDƬѡ
	do
	{
		for(i=0;i<100;i++) SPI1_WriteByte(0xff);

		r1 = SD_SendCommand(1, 0);         //Active����
		retry++;
		if(retry>254) return 1;            //��ʱ�˳�
	} while(r1);

	for(i=0;i<100;i++) SPI1_WriteByte(0xff);

	r1 = SD_SendCommand(59, 0);            //�� CRC
	if(r1) return 1;                       //���ز���ȷ,�˳���ʼ��

	for(i=0;i<100;i++) SPI1_WriteByte(0xff);

	r1 = SD_SendCommand(16, 512);          //��������С 512
	if(r1 != 0) return 1;                  //���ز���ȷ,�˳���ʼ��
	ClrWdt();
	return 0;                              //��������
}

//********************дһ������**************************
unsigned char SD_WriteSingleBlock(unsigned long sector,unsigned char *pchar)
{
	unsigned char r1;
	unsigned int i;
	unsigned char retry=0;
	unsigned char *p;
	do
	{
		for(i=0;i<100;i++) SPI1_WriteByte(0xff);

		r1 = SD_SendCommand(24, sector<<9);//д����
		retry++;
		if(retry>10) return 1;             //��ʱ�˳�
	} while(r1 != 0x00);

	SPI1_CS = 0;

	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);
	SPI1_WriteByte(0xff);

	SPI1_WriteByte(0xfe);                  //����ʼ��
   	p = pchar;  //��ȡ�ṹ���׵�ַ
 	for(i=0; i<512; i++)                  //��512�ֽ�����
 	{

		SPI1_WriteByte(*p);       //���ʹ洢����
		 p++;
	}

	SPI1_WriteByte(0x95);
	SPI1_WriteByte(0x95);                 //16-bits CRC

	r1 = SPI1_WriteByte(0xff);            //����Ӧλ
	if(retry++ >10) return 1;            //��ʱ�˳�
	while(!((r1&0x0f)==5));              //�ȴ����ݳɹ����ܷ�����Ϣ
	while(!(SPI1_WriteByte(0xff)));       //�ȴ�SD���ڲ�������
    ClrWdt();
	return 0;
}


//******************��SD��һ������************************
unsigned char SD_ReadSingleBlock(unsigned long sector,unsigned char *pchar)
{
	unsigned int  i;
	unsigned char r1,retry=0;
	unsigned char *k;
	do
	{
		r1 = SD_SendCommand(17, sector<<9);//������
		retry++;
		if(retry>10) return 1;             //��ʱ�˳�
	} while(r1 != 0x00);
	SPI1_CS = 0;
	while(SPI1_WriteByte(0xff)!= 0xfe)      //�ȴ����յ���ʼ�ֽ�
	{
		if(retry++ >100) return 1;         //��ʱ�˳�
	}
	k = pchar;

	for(i=0; i<=512; i++)                   //��������������
	{
		*k = SPI1_WriteByte(0xff);   //��ȡ���յ�������
		k++;
	}

	SPI1_WriteByte(0xff);                   //α16-bits crc
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