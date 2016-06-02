#ifndef _STC15_TIME_H__
#define _STC15_TIME_H__

#include "IAP15.h"


/*
功能:	定时器0定时功能，不打开中断
输入:	time : 定时的时间us
*/
void Timer0Init(unsigned int time);	//100微秒@22.1184MHz


#endif
