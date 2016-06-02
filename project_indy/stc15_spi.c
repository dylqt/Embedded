#include "stc15_spi.h"

#define MASTER	// 如果作为主机则定义
sbit SPISS      =   P1^1;       //SPI从机选择口, 连接到其它MCU的SS口
                                //当SPI为一主多从模式时,请使用主机的普通IO口连接到从机的SS口


/************************************************
SPI初始化
入口参数: 无
出口参数: 无
************************************************/
void InitSpi()
{
//	ACC = P_SW1;                                //切换到第一组SPI
//	ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=0
//	P_SW1 = ACC;                                //(P1.2/SS, P1.3/MOSI, P1.4/MISO, P1.5/SCLK)

    ACC = P_SW1;                                //
    ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=1 SPI_S1=0
    ACC |= SPI_S0;                              //(P2.4/SS_2, P2.3/MOSI_2, P2.2/MISO_2, P2.1/SCLK_2)
    P_SW1 = ACC;  

//  ACC = P_SW1;                                //切换到第三组SPI
//  ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=1
//  ACC |= SPI_S1;                              //(P5.4/SS_3, P4.0/MOSI_3, P4.1/MISO_3, P4.3/SCLK_3)
//  P_SW1 = ACC;  

	
	SPDAT = 0;					//初始化SPI数据
	SPSTAT = SPIF | WCOL;		//清除SPI状态位
#ifdef MASTER
	SPCTL = SPEN | MSTR;		//主机模式
#else
	SPCTL = SPEN;				//从机模式
#endif

}


///////////////////////////////////////////////////////////

void spi_isr() interrupt 9 using 1     //SPI中断服务程序 9 (004BH)
{
    SPSTAT = SPIF | WCOL;       //清除SPI状态位
#ifdef MASTER
    SPISS = 1;                  //拉高从机的SS
    //SendUart(SPDAT);            //返回SPI数据
#else                           //对于从机(从主机接收SPI数据,同时
    SPDAT = SPDAT;              //发送前一个SPI数据给主机)
#endif
}
 
