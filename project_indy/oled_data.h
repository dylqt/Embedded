#ifndef __OLED_DATA_H__
#define __OLED_DATA_H__

#include "stc15_oled.h"

extern code unsigned char Num[16][16];		// 数字
extern code unsigned char LETTER[52][16];			// 字母
extern code unsigned char HM[][16];			// HM lOGO
extern unsigned char code Fonts[];		// 5 * 7 字模， assic 从 32 - 127

extern code unsigned char stateBase[48];	// 状态栏的不变部分
extern code unsigned char stateBtConnect[2][6];	// 状态栏的蓝牙链接状态
extern code unsigned char stateBattery[7][18];	// 状态栏的电池电量 从满到空
extern code unsigned char stateCharging[2][6];		// 状态栏的电池充电状态
extern code unsigned char tempSymbol[16];			// 温度符号
void OledFlushBaseState();

/*
	功能 : 显示蓝牙的链接状态
	输入 : 0 : 未连接 ; 1 : 已连接
*/
void OledBtConnectState(unsigned char state);

/*
	功能 : 显示电量状态
	输入 : 0 : 100% 
			1 : 80%
			2 : 60%
			3 : 40%
			4 : 20%
			5 : 0%
*/
void OledBatteryState(unsigned char batteryState);

/*
	功能 : 显示电量
	输入 : 电量
*/
char OledBatteryPower(unsigned char batteryPower);

/*
	功能 : 显示充电状态
	输入 : 0 : 未充电; 1 : 充电
*/
void OledChargingState(unsigned char state);


#endif
