#ifndef __STD15_PWM_H__
#define __STD15_PWM_H__

#include "iap15.h"
#include "type.h"


void GetPwm();

/*
功能: P4.4输出PWM波
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwm4(unsigned char DUTY);

/*
功能: P4.2输出PWM波
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwm5(unsigned char DUTY);

/*
功能: P2.0输出PWM波，通过定时器0
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwm2(unsigned char DUTY);


/*
功能: PWM7_2输出PWM波
输入:
	FRE  : 频率
	0	:  300HZ
	1	:  500HZ
	2	:  1KHZ
	3	:  2KHZ
	4	:  4KHZ
	其他:  2KHZ
	
	DUTY : 占空比
	10 : 10%
*/
void GetPwmBZ(unsigned char FRE, unsigned int DUTY);

#define BzOFF (PWMCR &= ~0x20)					//关闭使能PWM7信号输出


/*
功能: 调节亮度 增加或减少
输入:
	 way : 1 变亮， -1 变暗
	 5%  10%  15%   20%  50%  90%
*/
void OledBrtAdjust(signed char way);
extern unsigned char OLED_BRT_DUTY;

#endif
