#include "stc15_uart.h"
#define DEBUG


bit   busy, busy2, busy3, busy4;
xdata uint8 rec_data[500];
uint16 rec_num = 0;
xdata   uint8   uart2_rec_data[201];
uint8   uart2_rec_cnt = 0;

xdata   uint8   uart3_rec_data[20];
uint8   uart3_rec_cnt = 0;

xdata   uint8   uart4_rec_data[200];
uint8   uart4_rec_cnt = 0;


//115200bps@22.1184MHz
void UartInit(void)		 
{
	
	P3M1 &= ~0xC0; 
	P3M0 |= 0xC0;

	ACC = P_SW1;
  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
	P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)

//	ACC = P_SW1;
//    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
//    ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
//    P_SW1 = ACC;  
  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	
	PS = 1;		// 拉高串口1的中断等级
	
    ES = 1;        //使能串口中断
    EA = 1;

}


/*----------------------------Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(unsigned char dat)
{
	while (busy);    //Wait for the completion of the previous data is sent
	ACC = dat;    //Calculate the even parity bit P (PSW.0)
//	if (P)    //Set the parity bit according to P
//	{
//		#if (PARITYBIT == ODD_PARITY)
//		TB8 = 0;    //Set parity bit to 0
//		#elif (PARITYBIT == EVEN_PARITY)
//		TB8 = 1;    //Set parity bit to 1
//		#endif
//	}
//	else
//	{
//		#if (PARITYBIT == ODD_PARITY)
//		TB8 = 1;    //Set parity bit to 1
//		#elif (PARITYBIT == EVEN_PARITY)
//		TB8 = 0;    //Set parity bit to 0
//		#endif
//	}
	busy = 1;
	SBUF = ACC;    //Send data to UART buffer
}
   
/*----------------------------Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/

void SendString(char *s)
{
	while (*s)    //Check the end of the string
	{
		SendData(*s++);    //Send current char and increment string ptr
	}
}

/*----------------------------UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4 using 1
{
	if (RI)
	{
		RI = 0;    //Clear receive interrupt flag
		rec_data[rec_num++] = SBUF;    //P0 show UART data
		//bit9 = RB8;    //P2.2 show parity bit

	}
	if (TI)
	{
		TI = 0;    //Clear transmit interrupt flag
		busy = 0;    //Clear transmit busy flag
	}
}


/*----------------------------UART2 interrupt service routine
----------------------------*/
void Uart2() interrupt 8 using 1
{
	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;    //Clear receive interrupt flag
        uart2_rec_data[uart2_rec_cnt++] = S2BUF;

	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;    //Clear transmit interrupt flag
		busy2 = 0;    //Clear transmit busy flag
	}
}
/*----------------------------Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData2(BYTE dat)
{
	while (busy2);    //Wait for the completion of the previous data is sent
	ACC = dat;    //Calculate the even parity bit P (PSW.0)
//	if (P)    //Set the parity bit according to P
//	{
//    	#if (PARITYBIT2 == ODD_PARITY2)
//    	S2CON &= ~S2TB8;    //Set parity bit to 0
//    	#elif (PARITYBIT2 == EVEN_PARITY2)
//    	S2CON |= S2TB8;    //Set parity bit to 1
//    	#endif
//    }
//    else
//    {
//    	#if (PARITYBIT2 == ODD_PARITY2)
//    	S2CON |= S2TB8;    //Set parity bit to 1
//    	#elif (PARITYBIT2 == EVEN_PARITY2)
//    	S2CON &= ~S2TB8;    //Set parity bit to 0
//    	#endif
//    }
    busy2 = 1;
    S2BUF = ACC;    //Send data to UART2 buffer
}


/*----------------------------Send a string to UART2
Input: s (address of string)
Output:None
----------------------------*/

void SendString2(char *s)
{
	while (*s)    //Check the end of the string
	{
		SendData2(*s++);    //Send current char and increment string ptr
	}
}


void Uart2Init()	//9600bps@22.1184MHz
{
	P1M1 &= ~0x03;
	P1M0 |= 0x03;
	
    P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
//  P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)


	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xC0;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2

    IE2 = 0x01;                 //使能串口2中断
    EA = 1;
}


void clear_rec_data()	  //清空接收数据
{
	uint16 i;
	for(i=0;i<500;i++)
	 	  rec_data[i] = 0;
	rec_num = 0;
}

void clear_uart2_data()
{
    char    i;
  
    for(i = 0;i < 100;i++)
    {
        uart2_rec_data[i] = 0;
    }
    uart2_rec_cnt = 0 ;
}

/////////////////////////////////////////////////////////////
// Uart3
void Uart3Init()	//115200bps@22.1184MHz
{
	//P_SW2 &= ~S3_S0; 		   //S3_S0=0 (P0.0/RxD3, P0.1/TxD3)
		P_SW2 |= S3_S0; 			//S3_S0=1 (P5.0/RxD3_2, P5.1/TxD3_2)
	
//#if (PARITYBIT3 == NONE_PARITY3)
//		S3CON = 0x50;				//8位可变波特率
//#elif (PARITYBIT3 == ODD_PARITY3) || (PARITYBIT3 == EVEN_PARITY3) || (PARITYBIT3 == MARK_PARITY3)
//		S3CON = 0xda;				//9位可变波特率,校验位初始为1
//#elif (PARITYBIT3 == SPACE_PARITY3)
//		S3CON = 0xd2;				//9位可变波特率,校验位初始为0
//#endif
	
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为Fosc,即1T
	T3L = 0xD0;		//设定定时初值
	T3H = 0xFF;		//设定定时初值
	T4T3M |= 0x08;		//启动定时器3

	IE2 |= 0x08; 				//使能串口3中断
	EA = 1;

}



/*----------------------------
UART3 中断服务程序
-----------------------------*/
void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI;         //清除S3RI位
        P0 = S3BUF;             //P0显示串口数据
       // P2 = (S3CON & S3RB8);   //P2.2显示校验位
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI;         //清除S3TI位
        busy3 = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData3(BYTE dat)
{
    while (busy3);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
//    if (P)                      //根据P来设置校验位
//    {
//#if (PARITYBIT3 == ODD_PARITY3)
//        S3CON &= ~S3TB8;        //设置校验位为0
//#elif (PARITYBIT3 == EVEN_PARITY3)
//        S3CON |= S3TB8;         //设置校验位为1
//#endif
//    }
//    else
//    {
//#if (PARITYBIT3 == ODD_PARITY3)
//        S3CON |= S3TB8;         //设置校验位为1
//#elif (PARITYBIT3 == EVEN_PARITY3)
//        S3CON &= ~S3TB8;        //设置校验位为0
//#endif
//    }
    busy3 = 1;
    S3BUF = ACC;                //写数据到UART3数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString3(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData3(*s++);         //发送当前字符
    }
}


/////////////////////////////////////////////////////////////
//Uart4 //112500bps@22.1184MHz

void Uart4Init(void)		
{
	
//	P_SW2 &= ~S4_S0;			//S4_S0=0 (P0.2/RxD4, P0.3/TxD4)
	P_SW2 |= S4_S0; 			//S4_S0=1 (P5.2/RxD4_2, P5.3/TxD4_2)
	
//#if (PARITYBIT4 == NONE_PARITY4)
//		S4CON = 0x50;				//8位可变波特率
//#elif (PARITYBIT4 == ODD_PARITY4) || (PARITYBIT4 == EVEN_PARITY4) || (PARITYBIT4 == MARK_PARITY4)
//		S4CON = 0xda;				//9位可变波特率,校验位初始为1
//#elif (PARITYBIT44 == SPACE_PARITY4)
//		S4CON = 0xd2;				//9位可变波特率,校验位初始为0
//#endif
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
	T4L = 0xD0;		//设定定时初值
	T4H = 0xFF;		//设定定时初值
	T4T3M |= 0x80;		//启动定时器4

	IE2 |= 0x10;                 //使能串口4中断
    EA = 1;
}

/*----------------------------
UART4 中断服务程序
-----------------------------*/
void Uart4() interrupt 18 using 1
{
    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI;         //清除S4RI位
        uart4_rec_data[uart4_rec_cnt++] = S4BUF;             //P0显示串口数据
      //  P2 = (S4CON & S4RB8);   //P2.2显示校验位
    }
    if (S4CON & S4TI)
    {
        S4CON &= ~S4TI;         //清除S4TI位
        busy4 = 0;              //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData4(BYTE dat)
{
    while (busy4);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
//    if (P)                      //根据P来设置校验位
//    {
//#if (PARITYBIT4 == ODD_PARITY4)
//        S4CON &= ~S4TB8;        //设置校验位为0
//#elif (PARITYBIT4 == EVEN_PARITY4)
//        S4CON |= S4TB8;         //设置校验位为1
//#endif
//    }
//    else
//    {
//#if (PARITYBIT4 == ODD_PARITY4)
//        S4CON |= S4TB8;         //设置校验位为1
//#elif (PARITYBIT4 == EVEN_PARITY4)
//        S4CON &= ~S4TB8;        //设置校验位为0
//#endif
//    }
    busy4 = 1;
    S4BUF = ACC;                //写数据到UART4数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString4(char *s)
{
#ifdef DEBUG
    while (*s)                  //检测字符串结束标志
    {
        SendData4(*s++);         //发送当前字符
    }
#endif
}


