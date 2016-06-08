#ifndef __BLE_H__
#define __BLE_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "stc15_oled.h"

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
#endif