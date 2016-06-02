#ifndef __STD15_PWM_H__
#define __STD15_PWM_H__

#include "iap15.h"
#include "type.h"


void GetPwm();

/*
����: P4.4���PWM��
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm4(unsigned char DUTY);

/*
����: P4.2���PWM��
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm5(unsigned char DUTY);

/*
����: P2.0���PWM����ͨ����ʱ��0
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm2(unsigned char DUTY);


/*
����: PWM7_2���PWM��
����:
	FRE  : Ƶ��
	0	:  300HZ
	1	:  500HZ
	2	:  1KHZ
	3	:  2KHZ
	4	:  4KHZ
	����:  2KHZ
	
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwmBZ(unsigned char FRE, unsigned int DUTY);

#define BzOFF (PWMCR &= ~0x20)					//�ر�ʹ��PWM7�ź����


/*
����: �������� ���ӻ����
����:
	 way : 1 ������ -1 �䰵
	 5%  10%  15%   20%  50%  90%
*/
void OledBrtAdjust(signed char way);
extern unsigned char OLED_BRT_DUTY;

#endif
