#ifndef _STC15_INT_H__
#define _STC15_INT_H__

#include "IAP15.h"
#include "result.h"
#include "stc15_oled.h"
#include "stc15_uart.h"
#include "power.h"

#define LVDF    0x20                //PCON.5,低压检测中断标志位
sbit INT0 = P3^2;
sbit INT1 = P3^3;
sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;


void t0IntInit();	//T0扩展为外部下降沿中断

void Int0Init();		// INT0中断服务程序

void Int1Init();		// INT1中断服务程序

void Int2Init();		// INT2中断服务程序

void Int3Init();		// INT3中断服务程序

void Int4Init();		// INT4中断服务程序

void LvdIntInit();		// 低压检测(LVD)中断

void t3IntInit();	// T3扩展为中断	12T


#endif
