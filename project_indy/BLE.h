#ifndef __BLE_H__
#define __BLE_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "stc15_oled.h"

sbit BUSY_BLE = P6^4;		//	����ӵ��ָʾ(͸��ģʽ�²������壬����ģʽ��������)
							// �ߵ�ƽ��ָʾMCU ���Լ�����ģ��д�벻����200 �ֽڵ�����
							//	�͵�ƽ��ָʾMCU ��Ӧ�ü�����ģ��д�����ݡ�
sbit BT_CTL_BLE = P6^5;		//������������(��������)  �øߣ��ر��������� �õͣ���ʼ�����㲥
sbit MODE_CTL_BLE  = P6^6;	//ģʽ��������(��������) �øߣ����벢����͸��ģʽ �õͣ����벢��������ģʽ
sbit BRT_BLE = P6^7;		//���߿�������(��������) �õͣ�����ģ�� �øߣ�����ģ���������״̬

#define BT_OPEN (BT_CTL_BLE = 0)	// �õͣ���ʼ�����㲥
#define BT_CLOSE (BT_CTL_BLE = 1)	// �øߣ��ر���������
#define BLE_CMD_MODE_IN (MODE_CTL_BLE = 0) 	// �õͣ���������ģʽ
#define BLE_TRAN_MODE_IN (MODE_CTL_BLE = 1) // �øߣ�����͸��ģʽ
#define WEEK_UP_BT (BRT_BLE = 0)	//�õͣ�����ģ��
#define SLEEP_BT (BRT_BLE = 1)		//�øߣ�����ģ���������״̬

void ble_init();
signed char ble_set_baud(unsigned char id);
signed char ble_inquire_baud(void);
#endif