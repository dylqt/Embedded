#include "stc15_time.h"
#include "type.h"

/*
功能:	定时器0定时功能，不打开中断
输入:	time : 定时的时间us
*/
void Timer0Init(unsigned int time)		//100微秒@22.1184MHz
{

	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = (65536 - (FOSC / 1000000 * time));		//设置定时初值
	TH0 = (65536 - (FOSC / 1000000 * time))>>8;		//设置定时初值
	TF0 = 0;		//清除TF0标志		等待TF0
	TR0 = 1;		//定时器0开始计时
}
