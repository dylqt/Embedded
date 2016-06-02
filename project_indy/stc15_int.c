#include "stc15_int.h"

unsigned int temp_t0_int_cnt = 0;
unsigned int vcc_t0_int_cnt = 0;

//T0��չΪ�ⲿ�½����ж�
void t0int() interrupt 1            //�ж����
{
	unsigned char batTmp = 0;
	double tempTmp = 0;
	
//	batTmp = getBatteryPercent();
//	tempTmp = tempScan();
	
	temp_t0_int_cnt++;
	vcc_t0_int_cnt++;
	
	SendData((unsigned char)temp_t0_int_cnt);
	
//	if(result.battery_pct > batTmp)
//		result.battery_pct = batTmp;		// ȡ���ĵ�ص���
//	if(result.temp_power_chip > tempTmp)	
//		result.temp_power_chip = batTmp;	// ȡ��ߵ��¶�
	
	if(temp_t0_int_cnt >= 250)		// ����20ms * 250 = 5s	�����¶�
	{
//			SendData4(0xff);
//		SendString4("temp of  u14 =  X  deg \n ");
		result.temp_power_chip = tempScan();
		OledWriteAssic57(3, 0, (unsigned char)result.temp_power_chip / 10 + '0');
		OledWriteAssic57(3, 6, ((unsigned char)result.temp_power_chip % 10) + '0');
		OledWriteAssic57(3, 12, '.');
		OledWriteAssic57(3, 18, ((unsigned int)(result.temp_power_chip * 100) % 100) / 10 + '0');
		OledWriteAssic57(3, 24, (unsigned int)(result.temp_power_chip * 100) % 10 + '0');

		// ����
		result.temp_power_chip = 0;
		
		temp_t0_int_cnt = 0;
	}
	if(vcc_t0_int_cnt >= 3000 )		// ����20ms * 3000 = 60000ms; 1min	���µ���
	{
		result.battery_pct = getBatteryPercent();
		//SendString4("temp of  u14 =  X  deg \n ");
		OledWriteAssic57(4, 0, result.battery_pct / 10 + '0');
		OledWriteAssic57(4, 6, result.battery_pct % 10 + '0');

		//����
		result.battery_pct = 0;
		
		vcc_t0_int_cnt = 0;
	}
	
}

void t0IntInit(void)
{
   // AUXR |= 0x80;                    //��ʱ��0Ϊ1Tģʽ	// max 2.9ms 
    AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ	// max 35.5ms
    TMOD &= 0xf0;                    //���ö�ʱ��0Ϊ16λ�Զ���װ���ⲿ����ģʽ

   	PT0 = 0;		//���ȼ����
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x70;		//���ö�ʱ��ֵ	// ��ʱ����20ms
	TF0 = 0;		//���TF0��־
	TR0 = 1;                        //��ʱ��0��ʼ����
    ET0 = 1;                        //����ʱ��0�ж�

    EA = 1;
}





// ��ʱ��3�жϳ�ʼ�� 12T
void t3IntInit(void)
{
//	T4T3M |= 0x02;                  //��ʱ��3Ϊ1Tģʽ
  	T4T3M &= ~0x02;                 //��ʱ��3Ϊ12Tģʽ

    T3L = 0x01;                 //��ʼ����ʱֵ
    T3H = 0x00;
    
    T4T3M |= 0x08;                  //��ʱ��3��ʼ��ʱ
    IE2 |= 0x20;                    //����ʱ��3�ж�
    
    EA = 1;
	
}








//-----------------------------------------
// INT0�жϷ������
void exint0() interrupt 0       //INT0�ж����
{
   // P10 = !P10;                 //�����Կ�ȡ��
}

//-----------------------------------------------

void Int0Init()
{
    INT0 = 1;
    IT0 = 1;                    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
    EX0 = 1;                    //ʹ��INT0�ж�
    EA = 1;
}


//-----------------------------------------
// INT1�жϷ������
void exint1() interrupt 2       //INT1�ж����
{
    //P10 = !P10;                 //�����Կ�ȡ��
    //FLAG = INT1;                //����INT1�ڵ�״̬, INT1=0(�½���); INT1=1(������)
}

//-----------------------------------------------

void Int1Init()
{
    INT1 = 1;
    IT1 = 0;                    //����INT1���ж����� (1:���½��� 0:�����غ��½���)
    EX1 = 1;                    //ʹ��INT1�ж�
    EA = 1;

}


//-----------------------------------------------
// INT2�жϷ������
void exint2() interrupt 10          //INT2�ж����
{
//    P10 = !P10;                     //�����Կ�ȡ��
    
//  INT_CLKO &= 0xEF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
//  INT_CLKO |= 0x10;               //Ȼ���ٿ��жϼ���
}

void Int2Init()
{
    INT_CLKO |= 0x10;               //(EX2 = 1)ʹ��INT2�ж�
    EA = 1;

}


//-----------------------------------------------
// INT3 �жϷ������
void exint3() interrupt 11          //INT3�ж����
{
 //   P10 = !P10;                     //�����Կ�ȡ��
    
//  INT_CLKO &= 0xDF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
//  INT_CLKO |= 0x20;               //Ȼ���ٿ��жϼ���
}

void Int3Init()
{
    INT_CLKO |= 0x20;               //(EX3 = 1)ʹ��INT3�ж�
    EA = 1;
}

//-----------------------------------------------
// INT4 �жϷ������
void exint4() interrupt 16          //INT3�ж����
{
//    P10 = !P10;                     //�����Կ�ȡ��
    
//  INT_CLKO &= 0xBF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
//  INT_CLKO |= 0x40;               //Ȼ���ٿ��жϼ���
}

void Int4Init()
{
    INT_CLKO |= 0x40;               //(EX4 = 1)ʹ��INT4�ж�
    EA = 1;

}


//-----------------------------------------
// ��ѹ���(LVD)�ж�
void LVD_ISR() interrupt 6 using 1
{
//    P11 = !P11;                     //���Կ�
    PCON &= ~LVDF;                  //��PCON.5д0��LVD�ж�
}

void LvdIntInit()
{
    PCON &= ~LVDF;                  //�ϵ����Ҫ��LVD�жϱ�־λ
    ELVD = 1;                       //ʹ��LVD�ж�
    EA = 1;                         //�����жϿ���
    
}

