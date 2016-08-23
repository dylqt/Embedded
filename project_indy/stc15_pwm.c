#include "stc15_pwm.h"
//#include "stc15_time.h"
#include "stc15_int.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

#define SYSC12 0x00					// 系统时钟 SYSclk/12
#define SYSC2  0x02					// 系统时钟 SYSclk/2
#define SYSC   0x08					// 系统时钟 SYSclk
#define SYSC4  0x0a					// 系统时钟 SYSclk/4
#define SYSC6  0x0c					// 系统时钟 SYSclk/6
#define SYSC8  0x0f					// 系统时钟 SYSclk/8
#define SYSCT0 0x04					// 系统时钟 T0溢出
#define SYSCECI 0x06 				// 系统时钟 ETI/P1.2


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

    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = SYSC2;                    //设置PCA时钟源
                                    //禁止PCA定时器溢出中断
    PCA_PWM0 = 0x00;                //PCA模块0工作于8位PWM
    CCAP0H = CCAP0L = 0x20;         //PWM0的占空比为87.5% ((100H-20H)/100H)	---> CCAPOL = 256 - 占空比 * 256
    CCAPM0 = 0x42;                  //PCA模块0为8位PWM模式

    PCA_PWM1 = 0x40;                //PCA模块1工作于7位PWM
    CCAP1H = CCAP1L = 0x20;         //PWM1的占空比为75% ((80H-20H)/80H)
    CCAPM1 = 0x42;                  //PCA模块1为7位PWM模式

    PCA_PWM2 = 0x80;                //PCA模块2工作于6位PWM
    CCAP2H = CCAP2L = 0x20;         //PWM2的占空比为50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCA模块2为6位PWM模式

    CR = 1;                         //PCA定时器开始工作
}


/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15Fxx 系列 输出任意周期和任意占空比的PWM实例-----------------*/

// 周期时间的计算 : CYCLE / Fosc
#define CYCLE 1000L
sfr PIN_SW2 =   0xba;

/*
功能: P2.2输出PWM4波
输入:
	fre : 频率
	DUTY : 占空比
	10 : 10%
*/
void GetPwm4(unsigned int fre)
{
	unsigned int ALL = 22118 / fre;
	//double left = (double)ALL / fre * 50;
	//ALL = (unsigned int)left;
    P2M1 &= ~(1 << 2);
    P2M0 &= ~(1 << 2);	// 设置端口模式为准双向口
	
	
    PIN_SW2 |= 0x80;                //使能访问XSFR
    PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
    PWMCKS = 0x00;                  //选择PWM的时钟为Fosc/(0+1)
    PWMC = ALL;                   //设置PWM周期
    PWM4T1 = 0x0000;                //设置PWM4第1次反转的PWM计数
    PWM4T2 = ALL  / 2;    //设置PWM4第2次反转的PWM计数
                                    //占空比为(PWM2T2-PWM2T1)/PWMC
    PWM4CR = 0x00;                  //选择PWM4输出到P4.4,不使能PWM4中断
	
	PWMCR |= 0x04;					//使能PWM4信号输出
    PWMCR |= 0x80;                  //使能PWM模块
    PIN_SW2 &= ~0x80;
}


/*
功能: P4.2输出PWM波
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwm5(unsigned char DUTY)
{
	P5M0 = 0x00;
    P5M1 = 0x00;	// 设置端口模式为准双向口

	PIN_SW2 |= 0x80;                //使能访问XSFR
    PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
	PWMCKS = 0x00;                  //选择PWM的时钟为Fosc/(0+1)
	PWMC = CYCLE;                   //设置PWM周期	    
	PWM5T1 = 0x0000;				//设置PWM5第1次反转的PWM计数
	PWM5T2 = CYCLE * DUTY / 100;    //设置PWM5第2次反转的PWM计数
									//占空比为(PWM2T2-PWM2T1)/PWMC
	PWM5CR = 0x08;                  //选择PWM5输出到P4.2,不使能PWM5中断

	PWMCR |= 0x08;					//使能PWM5信号输出
	PWMCR |= 0x80;                  //使能PWM模块
	PIN_SW2 &= ~0x80;
}


/*
功能: P2.0输出PWM波，通过定时器3, 频率100HZ
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwm2(unsigned char DUTY)
{
	//unsigned char allTime = 200; // 100hz 所以一个周期时间为10ms
	unsigned int upTime = 100 * DUTY;	// 10000us * DUTY /100 = 100 * DUTY
	unsigned int downTime = 10000 - upTime;

	pwm2_state = 0;
	t3IntInit();
}

//中断服务程序
void t3int() interrupt 19           //中断入口
{
  	if(pwm2_state == 0){
		   	T3L = (65536 - (FOSC / 1200000 * upTime));		//设置定时初值
			T3H = (65536 - (FOSC / 1200000 * upTime)) >> 8;		//设置定时初值
			P20 = 1;
			pwm2_state = 1;
		}
		else{
			T3L = (65536 - (FOSC / 1200000 * downTime));		//设置定时初值
			T3H = (65536 - (FOSC / 1200000 * downTime)) >> 8;		//设置定时初值
			P20 = 0;
			pwm2_state = 0;
		}
}



/*
功能: P0^6 PWM7_2输出PWM波	
输入:
	FRE  : 频率
	0	:  300HZ
	1	:  500HZ
	2	:  1KHZ
	3	:  2KHZ
	4	:  4KHZ
	其他:  2KHZ
	
	DUTY : 占空比
	10 : 10%
*/
void GetPwmBZ(unsigned char FRE, unsigned int DUTY)
{
	float cycle = 0;
	switch(FRE){		//cycle / FRE = 1 / FOSC ;cycle 最大为32767
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
    P0M1 &= ~(1 << 6);	// 设置端口模式为准双向口
    
    PIN_SW2 |= 0x80;                //使能访问XSFR
    PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
    PWMCKS = 0x03;                  //选择PWM的时钟为Fosc/(3+1)
    PWMC = cycle;                   //设置PWM周期
    PWM7T1 = 0x0000;                //设置PWM7第1次反转的PWM计数
    PWM7T2 = cycle / 100 * DUTY;    //设置PWM7第2次反转的PWM计数
                                    //占空比为(PWM2T2-PWM2T1)/PWMC
    PWM7CR = 0x08;                  //选择PWM7输出到P0.6,不使能PWM7中断

	
	PWMCR |= 0x20;					//使能PWM7信号输出
    PWMCR |= 0x80;                  //使能PWM模块
    PIN_SW2 &= ~0x80;

}

/*
功能: P2^1 PWM3输出PWM波	100HZ
输入:
	DUTY : 占空比
	10 : 10%
*/
void GetPwmBRT(unsigned char DUTY)
{
	float cycle = 27648;	// cycle = PWMC / (FOSC / ( 7 + 1 ))
	P2M1 &= ~(1 << 1);
	P2M1 &= ~(1 << 1);


	PIN_SW2 |= 0x80;                //使能访问XSFR
    PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
    PWMCKS = 0x07;                  //选择PWM的时钟为Fosc/(7+1)
    PWMC = cycle;                   //设置PWM周期		
    PWM3T1 = 0x0000;                //设置PWM3第1次反转的PWM计数
    PWM3T2 = cycle / 100 * DUTY;    //设置PWM3第2次反转的PWM计数
                                    //占空比为(PWM2T2-PWM2T1)/PWMC
									
    PWM3CR = 0x00;                  //选择PWM3输出到P2.1,不使能PWM中断
	
	PWMCR |= 0x02;					//使能PWM3信号输出
    PWMCR |= 0x80;                  //使能PWM模块
    PIN_SW2 &= ~0x80;
	
}

/*
功能: 调节亮度 增加或减少
输入:
	 way : 1 变亮， -1 变暗
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
