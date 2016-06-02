#ifndef __STC15_CLK_H__
#define __STC15_CLK_H__
#include "IAP15.h"


/*
�����λ
���� from :
	0x20 : ϵͳ���´��û���������ʼ���г���
	0x60 : ϵͳ���´�ISP��������ʼ���г���
*/
#define SoftReset(from) (IAP_CONTR = from)


/*
����ģʽ
���� div : 

	0 : ����Ƶ
	1 :	��ʱ��Ƶ��/2	
	2:	��ʱ��Ƶ��/4 
	3:	��ʱ��Ƶ��/8
	4:	��ʱ��Ƶ��/16
	5:	��ʱ��Ƶ��/32
	6:	��ʱ��Ƶ��/64
	7:	��ʱ��Ƶ��/128
*/
#define InPowerSavingMode(div) (CLK_DIV = div)

/*
*	ʡ��ģʽ
*/
#define InPowerIdleMode (PCON |= 0x01)           //��IDL(PCON.0)��1,MCU���������ģʽ
#define InPowerDownMode (PCON |= 0x02)           //��STOP(PCON.1)��1,MCU���������ģʽ

/*
���绽�Ѽ�ʱ��
���� time : 
	488.28us * time	
*/
void WeakUpDownMode(unsigned char time);

/*
ʱ�����
���� div : 
	0 : �����ʱ��
	0x40 :	���ʱ�ӣ�Ƶ�� = ��ʱ��Ƶ��	
	0x80 :	���ʱ�ӣ�Ƶ�� = ��ʱ��Ƶ�� / 2	
	0xc0 :	���ʱ�ӣ�Ƶ�� = ��ʱ��Ƶ��	/ 4
*/
#define ClkOutput(div) (CLK_DIV = div)



/*
���Ź���λ
���� time :
	0 : ���ÿ��ſڹ����ʱ��Ϊ35ms @22.1184M
	1 : ���ÿ��ſڹ����ʱ��Ϊ71ms @22.1184M
	2 : ���ÿ��ſڹ����ʱ��Ϊ142ms @22.1184M
	3 : ���ÿ��ſڹ����ʱ��Ϊ284ms @22.1184M
	4 : ���ÿ��ſڹ����ʱ��Ϊ568ms @22.1184M
	5 : ���ÿ��ſڹ����ʱ��Ϊ1137ms @22.1184M
	6 : ���ÿ��ſڹ����ʱ��Ϊ2275ms @22.1184M
	7 : ���ÿ��ſڹ����ʱ��Ϊ4451ms @22.1184M
*/
#define WdtInit(time) {WDT_CONTR = time; WDT_CONTR |= 0x20;}
#define WdtFeed(time) (WDT_CONTR = time)

#endif
