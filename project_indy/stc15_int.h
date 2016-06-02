#ifndef _STC15_INT_H__
#define _STC15_INT_H__

#include "IAP15.h"
#include "result.h"
#include "stc15_oled.h"
#include "stc15_uart.h"
#include "power.h"

#define LVDF    0x20                //PCON.5,��ѹ����жϱ�־λ
sbit INT0 = P3^2;
sbit INT1 = P3^3;
sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;


void t0IntInit();	//T0��չΪ�ⲿ�½����ж�

void Int0Init();		// INT0�жϷ������

void Int1Init();		// INT1�жϷ������

void Int2Init();		// INT2�жϷ������

void Int3Init();		// INT3�жϷ������

void Int4Init();		// INT4�жϷ������

void LvdIntInit();		// ��ѹ���(LVD)�ж�

void t3IntInit();	// T3��չΪ�ж�	12T


#endif
