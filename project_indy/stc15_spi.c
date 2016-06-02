#include "stc15_spi.h"

#define MASTER	// �����Ϊ��������
sbit SPISS      =   P1^1;       //SPI�ӻ�ѡ���, ���ӵ�����MCU��SS��
                                //��SPIΪһ�����ģʽʱ,��ʹ����������ͨIO�����ӵ��ӻ���SS��


/************************************************
SPI��ʼ��
��ڲ���: ��
���ڲ���: ��
************************************************/
void InitSpi()
{
//	ACC = P_SW1;                                //�л�����һ��SPI
//	ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=0
//	P_SW1 = ACC;                                //(P1.2/SS, P1.3/MOSI, P1.4/MISO, P1.5/SCLK)

    ACC = P_SW1;                                //
    ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=1 SPI_S1=0
    ACC |= SPI_S0;                              //(P2.4/SS_2, P2.3/MOSI_2, P2.2/MISO_2, P2.1/SCLK_2)
    P_SW1 = ACC;  

//  ACC = P_SW1;                                //�л���������SPI
//  ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=1
//  ACC |= SPI_S1;                              //(P5.4/SS_3, P4.0/MOSI_3, P4.1/MISO_3, P4.3/SCLK_3)
//  P_SW1 = ACC;  

	
	SPDAT = 0;					//��ʼ��SPI����
	SPSTAT = SPIF | WCOL;		//���SPI״̬λ
#ifdef MASTER
	SPCTL = SPEN | MSTR;		//����ģʽ
#else
	SPCTL = SPEN;				//�ӻ�ģʽ
#endif

}


///////////////////////////////////////////////////////////

void spi_isr() interrupt 9 using 1     //SPI�жϷ������ 9 (004BH)
{
    SPSTAT = SPIF | WCOL;       //���SPI״̬λ
#ifdef MASTER
    SPISS = 1;                  //���ߴӻ���SS
    //SendUart(SPDAT);            //����SPI����
#else                           //���ڴӻ�(����������SPI����,ͬʱ
    SPDAT = SPDAT;              //����ǰһ��SPI���ݸ�����)
#endif
}
 
