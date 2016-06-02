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



#define MAX_PAGE 5   			//7+8*4 ROWS 范围:0~MAX_PAGE-1
#define MAX_COLUMN 96 			//分辨率:横向96点  范围:0~MAX_COLUMN-1  96
#define MAX_ROW 39 				//分辨率:纵向39点 范围:0~MAX_ROW-1   39
#define COLUMN_OFFSET_ADDRINC 32
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
#define DEFAULT_BRIGHTNESS 0x14
#define MAX_DELAY_POWERUP 30000//用户自己调节,加入上电延时保证可靠显示

void InitOled();




// ------------------模拟SPI ----------------------------------------------
void OledPowerUp(void);
void OledPowerDown();

void OledEnteringSleepMode();
void OledExitingSleepMode();
/*
功能 : 写命令
输入 : CMD : 一个字节的命令
*/

void WR1BCmd(unsigned char cmd);
/*
功能 : 写数据
输入 : Data : 一个字节的数据
*/

void WR1BData(unsigned char dat);
void WRByte(unsigned char dat,unsigned char bIsCmd);
void uDelay(unsigned char t);

void OledFlush(unsigned char *);		//此版本为line mode
void OledClear();

void SetStartPage(uint8_t page);	 //页模式下,设置起始页
void SetStartColumn(uint8_t ColumnAddress);   //页模式下,设置起始列
void OledWriteMessage(char *str);		//调试使用，显示ASSIC码，特殊符号都是空格

/*
	功能 : 输出一个字符的HEX码
	输入 : Page 和 Col 显示的位置
		Page : 1 或 3 ; 总共5行，从0开始， 第一行只有7位，一个字符需要2个page，所以为1和3page开始
		Col : 最好8的倍数 因为每个字符需要两位16列
*/
void OledWriteWordByHex(unsigned char Page, unsigned char Col, unsigned char Data);

/*
	功能 : 输出HEX码的数据，最多输出24个数字，即12个字符
	输入 : 数组数据
*/
void OledWriteMessageByHex(unsigned char * Data);


// -------------------------- 使用5*7字模-------------------------------------------
/*
	功能 : 输出一个字符的HEX码
	输入 : Page 和 Col 显示的位置
		Page : 
		Col : 最好6的倍数 
*/

void OledWriteWordByHex57(unsigned char Page, unsigned char Col, unsigned char Data);

/*
	功能 : 输出HEX码的数据，最多输出64个数字，即32个字符
	输入 : 数组数据
*/
void OledWriteMessageByHex57(unsigned char *str);


/*
	功能 : 输出一个字符的assic码
	输入 : Page 和 Col 显示的位置
		Page : 
		Col : 最好6的倍数 
*/
void OledWriteAssic57(unsigned char Page, unsigned char Col,unsigned char Data);

/*
	功能 : 使用5*7字模 输出ASSIC码的数据，最多输出80个字符
	输入 : 字符串
*/
void OledWriteMessage57(char *str);


/*
	功能 : 清除某行某列开始的值
	输入 : Page 和 Col 的位置
*/
void clearPageCol(unsigned char Page, unsigned char Col);

#endif
