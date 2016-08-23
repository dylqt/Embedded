#ifndef __BLE_H__
#define __BLE_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "stc15_oled.h"
#include <string.h>
#include "indy.h"
sbit BUSY_BLE = P6^4;		//	数据拥塞指示(透传模式下才有意义，命令模式下无意义)
							// 高电平：指示MCU 可以继续向模块写入不超过200 字节的数据
							//	低电平：指示MCU 不应该继续向模块写入数据。
sbit BT_CTL_BLE = P6^5;		//蓝牙控制引脚(不可悬空)  置高：关闭蓝牙功能 置低：开始蓝牙广播
sbit MODE_CTL_BLE  = P6^6;	//模式控制引脚(不可悬空) 置高：进入并保持透传模式 置低：进入并保持命令模式
sbit BRT_BLE = P6^7;		//休眠控制引脚(不可悬空) 置低：唤醒模块 置高：允许模块进入休眠状态

#define BT_OPEN (BT_CTL_BLE = 0)	// 置低：开始蓝牙广播
#define BT_CLOSE (BT_CTL_BLE = 1)	// 置高：关闭蓝牙功能
#define BLE_CMD_MODE_IN (MODE_CTL_BLE = 0) 	// 置低：进入命令模式
#define BLE_TRAN_MODE_IN (MODE_CTL_BLE = 1) // 置高：进入透传模式
#define WEEK_UP_BT (BRT_BLE = 0)	//置低：唤醒模块
#define SLEEP_BT (BRT_BLE = 1)		//置高：允许模块进入休眠状态

void ble_init();
signed char ble_set_baud(unsigned char id);
signed char ble_inquire_baud(void);

void bleClearRecData();

// 蓝牙发送数据
void bleSendData(unsigned char cmdType, char *buf, unsigned char bufLen, unsigned char recState);
/*enum BLE_CMD {
	BLANK = 1,
	ID,
	TEMP,
	SENSORCODE,
	RSSI
};
*/

enum RECSTATE
{
	SUC = 0,
	ERR = 1
};

enum ENSEND{
	UNABLE = 0,
	ENABLE = 1
};

// 蓝牙接收到的状态和数据
struct BLERECVSTATE{
//	unsigned char cmd;
	enum RECSTATE recState; // 单片机收到蓝牙的命令是否正确	  0: 正确   1 : 错误 
//	unsigned char errNum; 	// 错误命令代码
//	unsigned char hand; // 握手命令；
	
//	unsigned char setEnSendState; 	// 发送设置状态； 1 : 设置； 0 : 不设置
//	enum ENSEND enSend;	// 发送允许标志
	
//	unsigned char setMoreInfo; // 显示更多信息	0 : 普通状态 1: 显示更多信息
//	enum ENSEND enMoreInfo;	// 发送允许标志

//	unsigned char setIndyFreq[4];	// 设置频率  1字节通道 1字节开关 2字节频率
//	unsigned char setIndyPower[2]; // 设置功率

//	unsigned char inSetStatus; 	// 进入设置界面
}; 

// 蓝牙当前的状态
struct BLESTATE{
	enum ENSEND enSend;	// 发送允许标志
	unsigned char moreInfor;	// 更多数据
	unsigned char fre[4];	// 当前频率
	unsigned char power[2];	// 当前功率
}; 

// 蓝牙接收到的命令
#define HAND 2
#define ENREAD 7
#define MOREINFO 8
#define SETINDYFRE 9
#define SETINDYPOWER 10
#define SETSTATUS 11
// 返回给蓝牙的命令
#define BLANK 0
#define ID 3
#define	TEMP 4
#define	SENSORCODE 5
#define	RSSI 6
#define SENDSTATE 7



extern struct BLERECVSTATE bleRecvFlag;
extern struct BLESTATE bleState;

void bleSendChar(char dat);
unsigned char bleParseData();
void bleDoCmd();

void bleRecvError(unsigned char errNum);
#endif