#include "beep.h"


void click10msBZ()
{	
	GetPwmBZ(1, 60);			//发声
	
	Delay10ms();
	
	PWMCR &= ~0x20;			//关闭PWM7信号输出
	P06 = 0;				//pin3 工作完 及  开机 时  为 强输出低。
}



void beep200msBZ()
{
	
	GetPwmBZ(3, 60);			//发声

	Delay200ms();
	
	PWMCR &= ~0x20;			//关闭PWM7信号输出
	P06 = 0;				//pin3 工作完 及  开机 时  为 强输出低。
}

void beep100msBZ()
{
	
	GetPwmBZ(3, 60);			//发声

	Delay100ms();
	
	PWMCR &= ~0x20;			//关闭PWM7信号输出
	P06 = 0;				//pin3 工作完 及  开机 时  为 强输出低。
}

void btLink3sBZ()
{
	GetPwmBZ(3, 60);			//发声
	Delay3000ms();
	PWMCR &= ~0x20;			//关闭PWM7信号输出
	P06 = 0;				//pin3 工作完 及  开机 时  为 强输出低。
}


void warningBZ(unsigned int DUTY)
{
	GetPwmBZ(4, DUTY);			//发声
}


void lowBattery50msBZ()		//电量低于5%, 1分钟 beep 一声； beep 频率 4KHz   持续时间 50ms
{
	GetPwmBZ(4, 60);			//发声
	Delay50ms();
	PWMCR &= ~0x20;			//关闭PWM7信号输出
	P06 = 0;				//pin3 工作完 及  开机 时  为 强输出低。
}


