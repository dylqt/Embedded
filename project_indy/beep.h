#ifndef __BEEP_H__
#define __BEEP_H__

#include "IAP15.h"
#include "stc15_pwm.h"
#include "delay.h"

void click10msBZ();		//click 声 时间10毫秒   用于用户按键短暂触发（小于2秒）

void beep200msBZ();		//beep声 时间200毫秒   用于用户按键长按触发（大于2秒）。

void beep100msBZ();

void btLink3sBZ();		//长鸣声 时间3秒      用于BT链接成功。

void warningBZ(unsigned int DUTY);	//用户可设置  目的 改变 射频模块过热报警声大小。

void lowBattery50msBZ();		//电量低于5%, 1分钟 beep 一声； beep 频率 4KHz   持续时间 50ms
#endif 
