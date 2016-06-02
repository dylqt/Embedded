#ifndef __STC15_CLK_H__
#define __STC15_CLK_H__
#include "IAP15.h"


/*
软件复位
输入 from :
	0x20 : 系统重新从用户代码区开始运行程序
	0x60 : 系统重新从ISP代码区开始运行程序
*/
#define SoftReset(from) (IAP_CONTR = from)


/*
低速模式
输入 div : 

	0 : 不分频
	1 :	主时钟频率/2	
	2:	主时钟频率/4 
	3:	主时钟频率/8
	4:	主时钟频率/16
	5:	主时钟频率/32
	6:	主时钟频率/64
	7:	主时钟频率/128
*/
#define InPowerSavingMode(div) (CLK_DIV = div)

/*
*	省电模式
*/
#define InPowerIdleMode (PCON |= 0x01)           //将IDL(PCON.0)置1,MCU将进入空闲模式
#define InPowerDownMode (PCON |= 0x02)           //将STOP(PCON.1)置1,MCU将进入掉电模式

/*
掉电唤醒计时器
输入 time : 
	488.28us * time	
*/
void WeakUpDownMode(unsigned char time);

/*
时钟输出
输入 div : 
	0 : 不输出时钟
	0x40 :	输出时钟，频率 = 主时钟频率	
	0x80 :	输出时钟，频率 = 主时钟频率 / 2	
	0xc0 :	输出时钟，频率 = 主时钟频率	/ 4
*/
#define ClkOutput(div) (CLK_DIV = div)



/*
看门狗复位
输入 time :
	0 : 设置看门口狗溢出时间为35ms @22.1184M
	1 : 设置看门口狗溢出时间为71ms @22.1184M
	2 : 设置看门口狗溢出时间为142ms @22.1184M
	3 : 设置看门口狗溢出时间为284ms @22.1184M
	4 : 设置看门口狗溢出时间为568ms @22.1184M
	5 : 设置看门口狗溢出时间为1137ms @22.1184M
	6 : 设置看门口狗溢出时间为2275ms @22.1184M
	7 : 设置看门口狗溢出时间为4451ms @22.1184M
*/
#define WdtInit(time) {WDT_CONTR = time; WDT_CONTR |= 0x20;}
#define WdtFeed(time) (WDT_CONTR = time)

#endif
