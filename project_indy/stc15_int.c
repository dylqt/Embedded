#include "stc15_int.h"

unsigned int temp_t0_int_cnt = 0;
unsigned int vcc_t0_int_cnt = 0;

//T0扩展为外部下降沿中断
void t0int() interrupt 1            //中断入口
{
	unsigned char batTmp = 0;
	double tempTmp = 0;
	
//	batTmp = getBatteryPercent();
//	tempTmp = tempScan();
	
	temp_t0_int_cnt++;
	vcc_t0_int_cnt++;
	
	SendData((unsigned char)temp_t0_int_cnt);
	
//	if(result.battery_pct > batTmp)
//		result.battery_pct = batTmp;		// 取最大的电池电量
//	if(result.temp_power_chip > tempTmp)	
//		result.temp_power_chip = batTmp;	// 取最高的温度
	
	if(temp_t0_int_cnt >= 250)		// 基数20ms * 250 = 5s	更新温度
	{
//			SendData4(0xff);
//		SendString4("temp of  u14 =  X  deg \n ");
		result.temp_power_chip = tempScan();
		OledWriteAssic57(3, 0, (unsigned char)result.temp_power_chip / 10 + '0');
		OledWriteAssic57(3, 6, ((unsigned char)result.temp_power_chip % 10) + '0');
		OledWriteAssic57(3, 12, '.');
		OledWriteAssic57(3, 18, ((unsigned int)(result.temp_power_chip * 100) % 100) / 10 + '0');
		OledWriteAssic57(3, 24, (unsigned int)(result.temp_power_chip * 100) % 10 + '0');

		// 清零
		result.temp_power_chip = 0;
		
		temp_t0_int_cnt = 0;
	}
	if(vcc_t0_int_cnt >= 3000 )		// 基数20ms * 3000 = 60000ms; 1min	更新电量
	{
		result.battery_pct = getBatteryPercent();
		//SendString4("temp of  u14 =  X  deg \n ");
		OledWriteAssic57(4, 0, result.battery_pct / 10 + '0');
		OledWriteAssic57(4, 6, result.battery_pct % 10 + '0');

		//清零
		result.battery_pct = 0;
		
		vcc_t0_int_cnt = 0;
	}
	
}

void t0IntInit(void)
{
   // AUXR |= 0x80;                    //定时器0为1T模式	// max 2.9ms 
    AUXR &= 0x7f;                   //定时器0为12T模式	// max 35.5ms
    TMOD &= 0xf0;                    //设置定时器0为16位自动重装载外部记数模式

   	PT0 = 0;		//优先级最低
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x70;		//设置定时初值	// 定时基数20ms
	TF0 = 0;		//清除TF0标志
	TR0 = 1;                        //定时器0开始工作
    ET0 = 1;                        //开定时器0中断

    EA = 1;
}





// 定时器3中断初始化 12T
void t3IntInit(void)
{
//	T4T3M |= 0x02;                  //定时器3为1T模式
  	T4T3M &= ~0x02;                 //定时器3为12T模式

    T3L = 0x01;                 //初始化计时值
    T3H = 0x00;
    
    T4T3M |= 0x08;                  //定时器3开始计时
    IE2 |= 0x20;                    //开定时器3中断
    
    EA = 1;
	
}








//-----------------------------------------
// INT0中断服务程序
void exint0() interrupt 0       //INT0中断入口
{
   // P10 = !P10;                 //将测试口取反
}

//-----------------------------------------------

void Int0Init()
{
    INT0 = 1;
    IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX0 = 1;                    //使能INT0中断
    EA = 1;
}


//-----------------------------------------
// INT1中断服务程序
void exint1() interrupt 2       //INT1中断入口
{
    //P10 = !P10;                 //将测试口取反
    //FLAG = INT1;                //保存INT1口的状态, INT1=0(下降沿); INT1=1(上升沿)
}

//-----------------------------------------------

void Int1Init()
{
    INT1 = 1;
    IT1 = 0;                    //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX1 = 1;                    //使能INT1中断
    EA = 1;

}


//-----------------------------------------------
// INT2中断服务程序
void exint2() interrupt 10          //INT2中断入口
{
//    P10 = !P10;                     //将测试口取反
    
//  INT_CLKO &= 0xEF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
//  INT_CLKO |= 0x10;               //然后再开中断即可
}

void Int2Init()
{
    INT_CLKO |= 0x10;               //(EX2 = 1)使能INT2中断
    EA = 1;

}


//-----------------------------------------------
// INT3 中断服务程序
void exint3() interrupt 11          //INT3中断入口
{
 //   P10 = !P10;                     //将测试口取反
    
//  INT_CLKO &= 0xDF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
//  INT_CLKO |= 0x20;               //然后再开中断即可
}

void Int3Init()
{
    INT_CLKO |= 0x20;               //(EX3 = 1)使能INT3中断
    EA = 1;
}

//-----------------------------------------------
// INT4 中断服务程序
void exint4() interrupt 16          //INT3中断入口
{
//    P10 = !P10;                     //将测试口取反
    
//  INT_CLKO &= 0xBF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
//  INT_CLKO |= 0x40;               //然后再开中断即可
}

void Int4Init()
{
    INT_CLKO |= 0x40;               //(EX4 = 1)使能INT4中断
    EA = 1;

}


//-----------------------------------------
// 低压检测(LVD)中断
void LVD_ISR() interrupt 6 using 1
{
//    P11 = !P11;                     //测试口
    PCON &= ~LVDF;                  //向PCON.5写0清LVD中断
}

void LvdIntInit()
{
    PCON &= ~LVDF;                  //上电后需要清LVD中断标志位
    ELVD = 1;                       //使能LVD中断
    EA = 1;                         //打开总中断开关
    
}

