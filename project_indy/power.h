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

// ��ԴоƬ���ƹܽŵ�״̬
extern unsigned char System_Power_Vcc_State;
extern unsigned char VSEL_State;
extern unsigned char BYP_State;

struct state{
	double temp_power_chip;		// ��ԴоƬ�¶�
	unsigned char battery_pct;	// ��ص���
	unsigned char battery_state;	// ����״̬������0.5s����˸
	unsigned char charge_state;		//���״̬���
	
	unsigned int temp_t0_int_cnt;	// �¶ȵĶ�ʱ������	5s���һ���¶ȣ� 5sΪ250��
	unsigned int vcc_t0_int_cnt;	// ��صĶ�ʱ������ 1min�ɼ�һ�Σ�1minΪ3000��
};

extern struct state workState;

// ˢ�µ�ص�״̬
void freshBatteryState();


void batteryInit(void);

double batteryScan(void);

unsigned char getBatteryPercent();

double tempScan();


/*
	���� �����״̬
	u1.4 ���� �߻�͵�ƽ
*/
unsigned char checkCharging();

#endif
