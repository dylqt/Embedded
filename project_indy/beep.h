#ifndef __BEEP_H__
#define __BEEP_H__

#include "IAP15.h"
#include "stc15_pwm.h"
#include "delay.h"

void click10msBZ();		//click �� ʱ��10����   �����û��������ݴ�����С��2�룩

void beep200msBZ();		//beep�� ʱ��200����   �����û�������������������2�룩��

void beep100msBZ();

void btLink3sBZ();		//������ ʱ��3��      ����BT���ӳɹ���

void warningBZ(unsigned int DUTY);	//�û�������  Ŀ�� �ı� ��Ƶģ����ȱ�������С��

void lowBattery50msBZ();		//��������5%, 1���� beep һ���� beep Ƶ�� 4KHz   ����ʱ�� 50ms
#endif 
