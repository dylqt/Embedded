#ifndef __OLED_DATA_H__
#define __OLED_DATA_H__

#include "stc15_oled.h"

extern code unsigned char Num[16][16];		// ����
extern code unsigned char LETTER[52][16];			// ��ĸ
extern code unsigned char HM[][16];			// HM lOGO
extern unsigned char code Fonts[];		// 5 * 7 ��ģ�� assic �� 32 - 127

extern code unsigned char stateBase[48];	// ״̬���Ĳ��䲿��
extern code unsigned char stateBtConnect[2][6];	// ״̬������������״̬
extern code unsigned char stateBattery[7][18];	// ״̬���ĵ�ص��� ��������
extern code unsigned char stateCharging[2][6];		// ״̬���ĵ�س��״̬
extern code unsigned char tempSymbol[16];			// �¶ȷ���
void OledFlushBaseState();

/*
	���� : ��ʾ����������״̬
	���� : 0 : δ���� ; 1 : ������
*/
void OledBtConnectState(unsigned char state);

/*
	���� : ��ʾ����״̬
	���� : 0 : 100% 
			1 : 80%
			2 : 60%
			3 : 40%
			4 : 20%
			5 : 0%
*/
void OledBatteryState(unsigned char batteryState);

/*
	���� : ��ʾ����
	���� : ����
*/
char OledBatteryPower(unsigned char batteryPower);

/*
	���� : ��ʾ���״̬
	���� : 0 : δ���; 1 : ���
*/
void OledChargingState(unsigned char state);


#endif
