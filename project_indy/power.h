#ifndef __POWER_H__
#define __POWER_H__
#include "stc15_adc.h"
#include "stc15_uart.h"
#include "delay.h"
#include "stdio.h"
#include "stc15_oled.h"
#include "oled_data.h"

sbit System_Power_Vcc_ON_OFF = P6^1;
sbit PG = P6^0;
sbit VSEL = P6^2;
sbit BYP = P6^3;
sbit death_ctron = P4^1;

// 电源芯片控制管脚的状态
extern unsigned char System_Power_Vcc_State;
extern unsigned char VSEL_State;
extern unsigned char BYP_State;

struct state{
	double temp_power_chip;		// 电源芯片温度
	unsigned char battery_pct;	// 电池电量
	unsigned char battery_state;	// 电量状态，用于0.5s的闪烁
	unsigned char charge_state;		//充电状态检测
	
	unsigned int temp_t0_int_cnt;	// 温度的定时器计数	5s检测一次温度， 5s为250次
	unsigned int vcc_t0_int_cnt;	// 电池的定时器计数 1min采集一次，1min为3000次
};

extern struct state workState;

// 刷新电池的状态
void freshBatteryState();


void batteryInit(void);

double batteryScan(void);

unsigned char getBatteryPercent();

double tempScan();


/*
	功能 检测充电状态
	u1.4 读到 高或低电平
*/
unsigned char checkCharging();

#endif
