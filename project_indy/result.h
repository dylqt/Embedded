#ifndef __RESULT_H__
#define __RESULT_H__

struct RESULT{
	double temp_power_chip;		// 电源芯片温度
	unsigned char battery_pct;	// 电池电量
};

extern struct RESULT result;

#endif
