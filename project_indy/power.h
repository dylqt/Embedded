#ifndef __POWER_H__
#define __POWER_H__
#include "stc15_adc.h"
#include "stc15_uart.h"
#include "delay.h"
#include "stdio.h"
#include "stc15_oled.h"

sbit System_Power_Vcc_ON_OFF = P6^1;
sbit PG = P6^0;
sbit VSEL = P6^2;
sbit BYP = P6^3;
sbit death_ctron = P4^1;

void batteryInit(void);

double batteryScan(void);
unsigned char getBatteryPercent();

double tempScan(void);


/*
	功能 检测充电状态
	u1.4 读到 高或低电平
*/
unsigned char checkCharging(void);

#endif
