#include "beep.h"


void click10msBZ()
{	
	GetPwmBZ(1, 60);			//����
	
	Delay10ms();
	
	PWMCR &= ~0x20;			//�ر�PWM7�ź����
	P06 = 0;				//pin3 ������ ��  ���� ʱ  Ϊ ǿ����͡�
}



void beep200msBZ()
{
	
	GetPwmBZ(3, 60);			//����

	Delay200ms();
	
	PWMCR &= ~0x20;			//�ر�PWM7�ź����
	P06 = 0;				//pin3 ������ ��  ���� ʱ  Ϊ ǿ����͡�
}

void beep100msBZ()
{
	
	GetPwmBZ(3, 60);			//����

	Delay100ms();
	
	PWMCR &= ~0x20;			//�ر�PWM7�ź����
	P06 = 0;				//pin3 ������ ��  ���� ʱ  Ϊ ǿ����͡�
}

void btLink3sBZ()
{
	GetPwmBZ(3, 60);			//����
	Delay3000ms();
	PWMCR &= ~0x20;			//�ر�PWM7�ź����
	P06 = 0;				//pin3 ������ ��  ���� ʱ  Ϊ ǿ����͡�
}


void warningBZ(unsigned int DUTY)
{
	GetPwmBZ(4, DUTY);			//����
}


void lowBattery50msBZ()		//��������5%, 1���� beep һ���� beep Ƶ�� 4KHz   ����ʱ�� 50ms
{
	GetPwmBZ(4, 60);			//����
	Delay50ms();
	PWMCR &= ~0x20;			//�ر�PWM7�ź����
	P06 = 0;				//pin3 ������ ��  ���� ʱ  Ϊ ǿ����͡�
}


