#ifndef __STD15_UART_H__
#define __STD15_UART_H__

#include "iap15.h"
#include "type.h"


/////////////////////////////////////////////////////////////
// Uart1 (for indy)	//115200bps@22.1184MHz

/*Define UART parity mode*/
#define   NONE_PARITY   0   //None parity
#define   ODD_PARITY   1   //Odd parity
#define   EVEN_PARITY   2   //Even parity
#define   MARK_PARITY  3   //Mark parity
#define   SPACE_PARITY  4   //Space parity
#define   PARITYBIT   NONE_PARITY   //Testing even parity
//sbit   bit9 = P2^2;    //P2.2 show UART data bit9

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

void SendData(unsigned char dat);
void SendString(char *s);
void UartInit(void);

//////////////////////////////////////////////////////////////
// Uart2 (for bt)	//112500bps@22.1184MHz

#define NONE_PARITY2     0       //无校验
#define ODD_PARITY2      1       //奇校验
#define EVEN_PARITY2     2       //偶校验
#define MARK_PARITY2     3       //标记校验
#define SPACE_PARITY2    4       //空白校验

#define PARITYBIT2 NONE_PARITY //Testing even parity

#define   S2RI   0x01    //S2CON.0
#define   S2TI   0x02    //S2CON.1
#define   S2RB8   0x04    //S2CON.2
#define   S2TB8   0x08    //S2CON.3

#define S2_S0 0x01 
 
extern bit busy,busy2;
extern xdata uint8 rec_data[500];
extern uint16 rec_num;
extern xdata   uint8   uart2_rec_data[201];
extern uint8   uart2_rec_cnt;

void SendData2(unsigned char dat);
void SendString2(char *s);
void Uart2Init(void);		//9600bps@22.1184MHz
void uart2_sendMes(void);
void clear_rec_data();	  //清空接收数据
void clear_uart2_data();

/////////////////////////////////////////////////////////
// Uart3

#define NONE_PARITY3     0       //无校验
#define ODD_PARITY3      1       //奇校验
#define EVEN_PARITY3     2       //偶校验
#define MARK_PARITY3     3       //标记校验
#define SPACE_PARITY3    4       //空白校验

#define PARITYBIT3 NONE_PARITY   //定义校验位


#define S3RI  0x01              //S3CON.0
#define S3TI  0x02              //S3CON.1
#define S3RB8 0x04              //S3CON.2
#define S3TB8 0x08              //S3CON.3

#define S3_S0 0x02              //P_SW2.1

extern bit busy3;


void Uart3Init(void); 	//115200bps@22.1184MHz
void SendData3(BYTE dat);
void SendString3(char *s);


//////////////////////////////////////////////////////////////
// Uart4 (for debug) //9600bps@22.1184MHz

#define NONE_PARITY4     0       //无校验
#define ODD_PARITY4      1       //奇校验
#define EVEN_PARITY4     2       //偶校验
#define MARK_PARITY4     3       //标记校验
#define SPACE_PARITY4    4       //空白校验

#define PARITYBIT4 NONE_PARITY   //定义校验位

#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define S4RB8 0x04              //S4CON.2
#define S4TB8 0x08              //S4CON.3

#define S4_S0 0x04              //P_SW2.2

extern bit busy4;

void Uart4Init(void);
void SendData4(BYTE dat);
void SendString4(char *s);

#endif
