#include "stc15_pwm.h"
//#include "stc15_time.h"
#include "stc15_int.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

#define SYSC12 0x00					// ϵͳʱ�� SYSclk/12
#define SYSC2  0x02					// ϵͳʱ�� SYSclk/2
#define SYSC   0x08					// ϵͳʱ�� SYSclk
#define SYSC4  0x0a					// ϵͳʱ�� SYSclk/4
#define SYSC6  0x0c					// ϵͳʱ�� SYSclk/6
#define SYSC8  0x0f					// ϵͳʱ�� SYSclk/8
#define SYSCT0 0x04					// ϵͳʱ�� T0���
#define SYSCECI 0x06 				// ϵͳʱ�� ETI/P1.2


unsigned char upTime;
unsigned char downTime;
unsigned char pwm2_state;
unsigned char key_state;
unsigned char OLED_BRT_DUTY = 50;
void GetPwm()
{
	ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
//  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
//  P_SW1 = ACC;  

    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = SYSC2;                    //����PCAʱ��Դ
                                    //��ֹPCA��ʱ������ж�
    PCA_PWM0 = 0x00;                //PCAģ��0������8λPWM
    CCAP0H = CCAP0L = 0x20;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)	---> CCAPOL = 256 - ռ�ձ� * 256
    CCAPM0 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ

    PCA_PWM1 = 0x40;                //PCAģ��1������7λPWM
    CCAP1H = CCAP1L = 0x20;         //PWM1��ռ�ձ�Ϊ75% ((80H-20H)/80H)
    CCAPM1 = 0x42;                  //PCAģ��1Ϊ7λPWMģʽ

    PCA_PWM2 = 0x80;                //PCAģ��2������6λPWM
    CCAP2H = CCAP2L = 0x20;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCAģ��2Ϊ6λPWMģʽ

    CR = 1;                         //PCA��ʱ����ʼ����
}


/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15Fxx ϵ�� ����������ں�����ռ�ձȵ�PWMʵ��-----------------*/

// ����ʱ��ļ��� : CYCLE / Fosc
#define CYCLE 1000L
sfr PIN_SW2 =   0xba;

/*
����: P2.2���PWM4��
����:
	fre : Ƶ��
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm4(unsigned int fre)
{
	unsigned int ALL = 22118 / fre;
	//double left = (double)ALL / fre * 50;
	//ALL = (unsigned int)left;
    P2M1 &= ~(1 << 2);
    P2M0 &= ~(1 << 2);	// ���ö˿�ģʽΪ׼˫���
	
	
    PIN_SW2 |= 0x80;                //ʹ�ܷ���XSFR
    PWMCFG = 0x00;                  //����PWM�������ʼ��ƽΪ�͵�ƽ
    PWMCKS = 0x00;                  //ѡ��PWM��ʱ��ΪFosc/(0+1)
    PWMC = ALL;                   //����PWM����
    PWM4T1 = 0x0000;                //����PWM4��1�η�ת��PWM����
    PWM4T2 = ALL  / 2;    //����PWM4��2�η�ת��PWM����
                                    //ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
    PWM4CR = 0x00;                  //ѡ��PWM4�����P4.4,��ʹ��PWM4�ж�
	
	PWMCR |= 0x04;					//ʹ��PWM4�ź����
    PWMCR |= 0x80;                  //ʹ��PWMģ��
    PIN_SW2 &= ~0x80;
}


/*
����: P4.2���PWM��
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm5(unsigned char DUTY)
{
	P5M0 = 0x00;
    P5M1 = 0x00;	// ���ö˿�ģʽΪ׼˫���

	PIN_SW2 |= 0x80;                //ʹ�ܷ���XSFR
    PWMCFG = 0x00;                  //����PWM�������ʼ��ƽΪ�͵�ƽ
	PWMCKS = 0x00;                  //ѡ��PWM��ʱ��ΪFosc/(0+1)
	PWMC = CYCLE;                   //����PWM����	    
	PWM5T1 = 0x0000;				//����PWM5��1�η�ת��PWM����
	PWM5T2 = CYCLE * DUTY / 100;    //����PWM5��2�η�ת��PWM����
									//ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
	PWM5CR = 0x08;                  //ѡ��PWM5�����P4.2,��ʹ��PWM5�ж�

	PWMCR |= 0x08;					//ʹ��PWM5�ź����
	PWMCR |= 0x80;                  //ʹ��PWMģ��
	PIN_SW2 &= ~0x80;
}


/*
����: P2.0���PWM����ͨ����ʱ��3, Ƶ��100HZ
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwm2(unsigned char DUTY)
{
	//unsigned char allTime = 200; // 100hz ����һ������ʱ��Ϊ10ms
	unsigned int upTime = 100 * DUTY;	// 10000us * DUTY /100 = 100 * DUTY
	unsigned int downTime = 10000 - upTime;

	pwm2_state = 0;
	t3IntInit();
}

//�жϷ������
void t3int() interrupt 19           //�ж����
{
  	if(pwm2_state == 0){
		   	T3L = (65536 - (FOSC / 1200000 * upTime));		//���ö�ʱ��ֵ
			T3H = (65536 - (FOSC / 1200000 * upTime)) >> 8;		//���ö�ʱ��ֵ
			P20 = 1;
			pwm2_state = 1;
		}
		else{
			T3L = (65536 - (FOSC / 1200000 * downTime));		//���ö�ʱ��ֵ
			T3H = (65536 - (FOSC / 1200000 * downTime)) >> 8;		//���ö�ʱ��ֵ
			P20 = 0;
			pwm2_state = 0;
		}
}



/*
����: P0^6 PWM7_2���PWM��	
����:
	FRE  : Ƶ��
	0	:  300HZ
	1	:  500HZ
	2	:  1KHZ
	3	:  2KHZ
	4	:  4KHZ
	����:  2KHZ
	
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwmBZ(unsigned char FRE, unsigned int DUTY)
{
	float cycle = 0;
	switch(FRE){		//cycle / FRE = 1 / FOSC ;cycle ���Ϊ32767
		case 0:			// 300HZ
			cycle = 18432;
			break;
		case 1:			// 500HZ
			cycle = 11059;
			break;
		case 2:			// 1KHZ
			cycle = 5529;
			break;
		case 3:			// 2KHZ
			cycle = 2765;
			break;
		case 4:			// 4KHZ
			cycle = 1382;
			break;
		default:
			cycle = 2765;
			break;
	}
    P0M0 &= ~(1 << 6);
    P0M1 &= ~(1 << 6);	// ���ö˿�ģʽΪ׼˫���
    
    PIN_SW2 |= 0x80;                //ʹ�ܷ���XSFR
    PWMCFG = 0x00;                  //����PWM�������ʼ��ƽΪ�͵�ƽ
    PWMCKS = 0x03;                  //ѡ��PWM��ʱ��ΪFosc/(3+1)
    PWMC = cycle;                   //����PWM����
    PWM7T1 = 0x0000;                //����PWM7��1�η�ת��PWM����
    PWM7T2 = cycle / 100 * DUTY;    //����PWM7��2�η�ת��PWM����
                                    //ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
    PWM7CR = 0x08;                  //ѡ��PWM7�����P0.6,��ʹ��PWM7�ж�

	
	PWMCR |= 0x20;					//ʹ��PWM7�ź����
    PWMCR |= 0x80;                  //ʹ��PWMģ��
    PIN_SW2 &= ~0x80;

}

/*
����: P2^1 PWM3���PWM��	100HZ
����:
	DUTY : ռ�ձ�
	10 : 10%
*/
void GetPwmBRT(unsigned char DUTY)
{
	float cycle = 27648;	// cycle = PWMC / (FOSC / ( 7 + 1 ))
	P2M1 &= ~(1 << 1);
	P2M1 &= ~(1 << 1);


	PIN_SW2 |= 0x80;                //ʹ�ܷ���XSFR
    PWMCFG = 0x00;                  //����PWM�������ʼ��ƽΪ�͵�ƽ
    PWMCKS = 0x07;                  //ѡ��PWM��ʱ��ΪFosc/(7+1)
    PWMC = cycle;                   //����PWM����		
    PWM3T1 = 0x0000;                //����PWM3��1�η�ת��PWM����
    PWM3T2 = cycle / 100 * DUTY;    //����PWM3��2�η�ת��PWM����
                                    //ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
									
    PWM3CR = 0x00;                  //ѡ��PWM3�����P2.1,��ʹ��PWM�ж�
	
	PWMCR |= 0x02;					//ʹ��PWM3�ź����
    PWMCR |= 0x80;                  //ʹ��PWMģ��
    PIN_SW2 &= ~0x80;
	
}

/*
����: �������� ���ӻ����
����:
	 way : 1 ������ -1 �䰵
	 5%  10%  15%   20%  50%  90%
*/
void OledBrtAdjust(signed char way)
{
	switch(OLED_BRT_DUTY){
		case 5:
			if(way == 1){
				OLED_BRT_DUTY = 10;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		case 10:
			if(way == 1){
				OLED_BRT_DUTY = 15;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else if(way == -1){
				OLED_BRT_DUTY = 5;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		case 15:
			if(way == 1){
				OLED_BRT_DUTY = 20;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else if(way == -1){
				OLED_BRT_DUTY = 10;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		case 20:
			if(way == 1){
				OLED_BRT_DUTY = 50;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else if(way == -1){
				OLED_BRT_DUTY = 15;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		case 50 :
			if(way == 1){
				OLED_BRT_DUTY = 90;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else if(way == -1){
				OLED_BRT_DUTY = 20;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		case 90:
			if(way == -1){
				OLED_BRT_DUTY = 50;
				GetPwmBRT(OLED_BRT_DUTY);
				break;
				}
			else 
				break;
		default:
			OLED_BRT_DUTY = 50;
			GetPwmBRT(OLED_BRT_DUTY);
			break;
	}
}
