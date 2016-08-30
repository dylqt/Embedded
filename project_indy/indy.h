#ifndef __INDY_H__
#define __INDY_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "hostifregs.h"
#include "maccmds.h"
#include "stc15_oled.h"
#include "beep.h"
#include "BLE.h"
//#include "RFID.h"

#define TAG_READ_ERR    0
#define EPC_CODE_NUM    12
#define VAL_DATA_NUM    2
#define UART2_SEND_NUM  32
#define RESERVED_DATA   0
#define READ_TAG        1
#define READ_NO_TAG     0

#define UART2_RECV_NUM   8

// --------------  У׼�¶� ------------------
// temp = 1/10 * [(TEMP2 - TEMP1) / (CODE2 - CODE1) * (getCode - CODE1) + TEMP1 - 800]
// getCode Ϊ�����Լ�������ֵ
// TEMP2 ��TEMP1 = temp * 10 + 800
// CODE2 ��CODE1 �ֱ�ΪTEMP2��TEMP1ʱ������CODEֵ
extern uint16 TEMP2;
extern uint16 TEMP1;
extern uint16 CODE2;
extern uint16 CODE1;

typedef struct  uart2_body
{
    uint8   cmd_type;	
    uint8   epc_code[12];
    uint8   val_data[2];
    char    res[16];
    uint8   check_sum;
}UART2_BODY;

extern xdata   UART2_BODY  mes_body;	// �����Ϣ

void write_mac_register(uint16 addr,uint16 w_data,uint16 w_data_l);

void read_mac_register(uint16 addr);

void indySendMes(void);

void clear_mes_body();

void process_packets();

void indy_read(uint16 bank, uint16 addr, uint16 num);

void indy_read_sensorcode();

void indy_readrssi();

void indy_readtemp();

void index_send(char id);

char index_r_check(char id);

// ���Ӷ�����������Ƶ��Ϊ875MHZ
void indyInitCh875();	

/*
	channel : ͨ�����
	state : �򿪻�رգ� 0 �رգ� 1 ��
	fre : ���õ�Ƶ��
*/
void indySetFre(unsigned char channel, unsigned char state, unsigned int fre);
void indySetPower(unsigned int power);

void indy_readall();


// ���ñ궨��Ϣ
void indySetBD(uint16 code1, uint16 temp1, uint16 code2, uint16 temp2, uint16 vl);
	
// ��ȡ�궨��Ϣ
void indyGetBD();

#endif
