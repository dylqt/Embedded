#ifndef __STC15_OLED_H__
#define __STC15_OLED_H__

#include "IAP15.h"
#include "intrins.h"
#include "delay.h"
#include "stc15_uart.h"
#include "type.h"
#include "stdlib.h"
#include "string.h"
#include "oled_data.h"

sbit BR_OLED = P2^0;
sbit BS1_OLED = P2^1;
sbit BS2_OLED = P2^2;
sbit CS_OLED = P2^3;
sbit DC_OLED = P2^4;
sbit RD_OLED = P2^5;
sbit WR_OLED = P2^6;
sbit SDIN_OLED = P2^7;
sbit SCLK_OLED = P7^4;
sbit SDA_OLED = P7^5;
sbit SCL_OLED = P7^6;
sbit D0_OLED = P7^7;
sbit D1_OLED = P4^5;
sbit D2_OLED = P4^6;
sbit D3_OLED = P0^0;
sbit D4_OLED = P0^1;
sbit D5_OLED = P0^2;
sbit D6_OLED = P0^3;
sbit D7_OLED = P0^4;
sbit RES_OLED = P4^1;
sbit VDDB_OLED = P6^1;



#define MAX_PAGE 5   			//7+8*4 ROWS ��Χ:0~MAX_PAGE-1
#define MAX_COLUMN 96 			//�ֱ���:����96��  ��Χ:0~MAX_COLUMN-1  96
#define MAX_ROW 39 				//�ֱ���:����39�� ��Χ:0~MAX_ROW-1   39
#define COLUMN_OFFSET_ADDRINC 32
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
#define DEFAULT_BRIGHTNESS 0x14
#define MAX_DELAY_POWERUP 30000//�û��Լ�����,�����ϵ���ʱ��֤�ɿ���ʾ

void InitOled();




// ------------------ģ��SPI ----------------------------------------------
void OledPowerUp(void);
void OledPowerDown();

void OledEnteringSleepMode();
void OledExitingSleepMode();
/*
���� : д����
���� : CMD : һ���ֽڵ�����
*/

void WR1BCmd(unsigned char cmd);
/*
���� : д����
���� : Data : һ���ֽڵ�����
*/

void WR1BData(unsigned char dat);
void WRByte(unsigned char dat,unsigned char bIsCmd);
void uDelay(unsigned char t);

char OledFlush(unsigned char *);		//�˰汾Ϊline mode
void OledClear();

char SetStartPage(uint8_t page);	 //ҳģʽ��,������ʼҳ
char SetStartColumn(uint8_t ColumnAddress);   //ҳģʽ��,������ʼ��
void OledWriteMessage(char *str);		//����ʹ�ã���ʾASSIC�룬������Ŷ��ǿո�


/*
	���� : ˢ��ĳһ��
	���� : ĳһ�п�ʼλ�ã�������
*/
char OledFlushPage(unsigned char Page, unsigned char Col,unsigned char *buf, unsigned char bufNum);


/*
	���� : ���һ���ַ���HEX��
	���� : Page �� Col ��ʾ��λ��
		Page : 1 �� 3 ; �ܹ�5�У���0��ʼ�� ��һ��ֻ��7λ��һ���ַ���Ҫ2��page������Ϊ1��3page��ʼ
		Col : ���8�ı��� ��Ϊÿ���ַ���Ҫ��λ16��
*/
char OledWriteWordByHex(unsigned char Page, unsigned char Col, unsigned char Data);

/*
	���� : ���HEX������ݣ�������24�����֣���12���ַ�
	���� : �������ݣ��Լ�����ĳ���
	��� : �����鳬��12���ַ�ʱ����ʾ12���ַ��������ش������
*/
char OledWriteBufByHex(unsigned char *Data, unsigned char Num);



// -------------------------- ʹ��5*7��ģ-------------------------------------------
/*
	���� : ���һ���ַ���HEX��
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
*/
char OledWriteWordByHex57(unsigned char Page, unsigned char Col, unsigned char Data);

/*
	���� : ʹ��5*7��ģ  ���HEX������ݣ�������64�����֣���32���ַ�
	���� : ��������
*/
char OledWriteBufByHex57index(unsigned char Page, unsigned char Col, unsigned char *str, unsigned char Num);

char OledWriteBufByHex57(unsigned char *str, unsigned char Num);


/*
	���� : ���һ���ַ���assic��
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
*/
char OledWriteAssic57(unsigned char Page, unsigned char Col,unsigned char Data);

/*
	���� : ����state ʹ��5*7��ģ ���һ���ַ���assic��	
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
		Data : ��ĸ
*/
char OledAssicForState(unsigned char Page, unsigned char Col,unsigned char Data);


/*
	���� : ʹ��5*7��ģ ���ASSIC������ݣ�������80���ַ�
	���� : �ַ���
*/
void OledWriteMessage57(char *str);


/*
	���� : ���ĳ��ĳ�п�ʼ��ֵ
	���� : Page �� Col ��λ��
*/
char clearPageCol(unsigned char Page, unsigned char Col);

/*
	���� : ��ĳ��ĳ����ʾdouble�͵���ֵ
	���� : Page �� Col ��λ�ã�double��ֵ
*/
unsigned char OledWriteDouble(unsigned char Page, unsigned char Col, double input);

/*
	���� : ʹ��5*7��ģ  ���assic�����ݣ�������64�����֣���32���ַ�
	���� : ��ʼҳ����ʼλ�ã���������
*/
char OledWriteBufByAssic57(unsigned char Page, unsigned char Col, unsigned char *arr, unsigned char Num);

/*
	���� : ��ĳ��ĳ����ʾ�¶ȷ���
	���� : Page �� Col ��λ��
*/
char OledWriteTempSymbol(unsigned char Page, unsigned char Col);
#endif
