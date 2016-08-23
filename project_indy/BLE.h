#ifndef __BLE_H__
#define __BLE_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "stc15_oled.h"
#include <string.h>
#include "indy.h"
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

void bleClearRecData();

// ������������
void bleSendData(unsigned char cmdType, char *buf, unsigned char bufLen, unsigned char recState);
/*enum BLE_CMD {
	BLANK = 1,
	ID,
	TEMP,
	SENSORCODE,
	RSSI
};
*/

enum RECSTATE
{
	SUC = 0,
	ERR = 1
};

enum ENSEND{
	UNABLE = 0,
	ENABLE = 1
};

// �������յ���״̬������
struct BLERECVSTATE{
//	unsigned char cmd;
	enum RECSTATE recState; // ��Ƭ���յ������������Ƿ���ȷ	  0: ��ȷ   1 : ���� 
//	unsigned char errNum; 	// �����������
//	unsigned char hand; // �������
	
//	unsigned char setEnSendState; 	// ��������״̬�� 1 : ���ã� 0 : ������
//	enum ENSEND enSend;	// ���������־
	
//	unsigned char setMoreInfo; // ��ʾ������Ϣ	0 : ��ͨ״̬ 1: ��ʾ������Ϣ
//	enum ENSEND enMoreInfo;	// ���������־

//	unsigned char setIndyFreq[4];	// ����Ƶ��  1�ֽ�ͨ�� 1�ֽڿ��� 2�ֽ�Ƶ��
//	unsigned char setIndyPower[2]; // ���ù���

//	unsigned char inSetStatus; 	// �������ý���
}; 

// ������ǰ��״̬
struct BLESTATE{
	enum ENSEND enSend;	// ���������־
	unsigned char moreInfor;	// ��������
	unsigned char fre[4];	// ��ǰƵ��
	unsigned char power[2];	// ��ǰ����
}; 

// �������յ�������
#define HAND 2
#define ENREAD 7
#define MOREINFO 8
#define SETINDYFRE 9
#define SETINDYPOWER 10
#define SETSTATUS 11
// ���ظ�����������
#define BLANK 0
#define ID 3
#define	TEMP 4
#define	SENSORCODE 5
#define	RSSI 6
#define SENDSTATE 7



extern struct BLERECVSTATE bleRecvFlag;
extern struct BLESTATE bleState;

void bleSendChar(char dat);
unsigned char bleParseData();
void bleDoCmd();

void bleRecvError(unsigned char errNum);
#endif