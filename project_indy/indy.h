#ifndef __INDY_H__
#define __INDY_H__

#include "stc15_uart.h"
#include "delay.h"
#include "iap15.h"
#include "hostifregs.h"
#include "maccmds.h"
#include "stc15_oled.h"
//#include "RFID.h"

#define TAG_READ_ERR    0
#define EPC_CODE_NUM    12
#define VAL_DATA_NUM    2
#define UART2_SEND_NUM  32
#define RESERVED_DATA   0
#define READ_TAG        1
#define READ_NO_TAG     0

#define UART2_RECV_NUM   8

typedef struct  uart2_body
{
    uint8   cmd_type;
    uint8   epc_code[12];
    uint8   val_data[2];
    char    res[16];
    uint8   check_sum;
}UART2_BODY;

extern xdata   UART2_BODY  uart2_body;

void write_mac_register(uint16 addr,uint16 w_data,uint16 w_data_l);

void read_mac_register(uint16 addr);

void uart2_sendMes(void);

void clear_uart2_mes();

void process_packets();

void indy_read_sensorcode();

void indy_readrssi();

void indy_readtemp();

int uart2_recv_proc();

void index_send(char id);

char index_r_check(char id);

#endif