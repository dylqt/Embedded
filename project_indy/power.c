#include "power.h"

#define STDVCC 4.2		// 标准电压4.2v
#define STABLEV 2.459	// 稳定电压值2.459v

unsigned char getBatteryTimes = 10;

unsigned char System_Power_Vcc_State = 0;
unsigned char VSEL_State = 0;
unsigned char BYP_State = 0;

sbit batteryChargeState = P0^7;

struct state workState; 

// 刷新电池的状态
void freshBatteryState()		
{
	
	if(workState.charge_state || workState.battery_state != 5){	// 如果在充电状态或者非低电压的状态下时，每一分钟检测一次电压
		if( workState.vcc_t0_int_cnt == 3000)	{	// 在非低电压模式下 1min加测一次
			workState.vcc_t0_int_cnt = 0;
			workState.battery_pct = getBatteryPercent();
			if(workState.battery_pct >= 80){
				if(workState.battery_state != 0){	// 如果上一次的状态和这次的相同就不刷新
					workState.battery_state = 0;
					OledBatteryState(0);
				}
			}
			else if(workState.battery_pct >= 60){
				if(workState.battery_state != 1){
					workState.battery_state = 1;
					OledBatteryState(1);
				}
			}
			else if(workState.battery_pct >= 40){
				if(workState.battery_state != 2){
					workState.battery_state = 2;
					OledBatteryState(2);
				}
			}
			else if(workState.battery_pct >= 20){
				if(workState.battery_state != 3){
					workState.battery_state = 3;
					OledBatteryState(3);
				}
			}
			else if(workState.battery_pct > 5){
				if(workState.battery_state != 4){
					workState.battery_state = 4;
					OledBatteryState(4);
				}
			}
			else{			// 进入低电压模式
				if(workState.battery_state != 5){
					workState.battery_state = 5;
					OledBatteryState(5);
				}
			}
		}
	}
	else{	// 低电压模式且没有充电则0.5s闪烁
		
		if(workState.vcc_t0_int_cnt % 25 == 0){	// 0.5s闪烁一次
			if((workState.vcc_t0_int_cnt / 25 & 1) == 0)	// 闪烁
				OledBatteryState(6);
			else
				OledBatteryState(5);
		}
		if(workState.vcc_t0_int_cnt == 3000){	// 1min蜂鸣器叫一次
			workState.vcc_t0_int_cnt = 0;
			lowBattery50msBZ();		
		}
	}	
}


void batteryInit(void)
{
	 //初始化  u1.8   u1.6  u1.7  为强输出模式；
	 P6M1 &= ~0x0e;
	 P6M0 |= 0x0e;
	 //u1.5 为内部电阻上拉输入模式；
	 P6M1 &= ~0x01;
	 P6M0 &= ~0x01;
	 //且u1.8为低电平；u1.6为高电平； u1.7为高电平；
	 
	 System_Power_Vcc_ON_OFF = 0;
	 VSEL = 1;
	 BYP = 1;
	 PG = 1;	// 电源模块状态标志

	 P4M1 &= ~0x02;
	 P4M0 |= 0x02;
	 death_ctron = 0;	// 先拉低复位线
	 System_Power_Vcc_ON_OFF = 1; // 开总电源
	 
	 Delay100ms();
	 death_ctron = 1;	// 关闭复位线
}


/*
	功能 读取电压
	分辨率1024，标准电压1.27v
*/ 
double batteryScan(void)
{	
	unsigned int batteryValue = 0;
	unsigned int batteryValuetmp = 0;
	unsigned char i = 0;
	double realBatteryValue = 0;
	unsigned int left = 0;
		
	//SendString4(" batteryScan in \n ");
	// 主循环一次采集一次
//	batteryValue[getBatteryTimes++] = read_adc1024(4);
//	
//	if(getBatteryTimes == 10){	// 采集10次 输出一次
//	
//	realBatteryValue = STABLEV * 1024 / (batteryValue[3] + batteryValue[6] + batteryValue[9]) * 3;
//
//	left = realBatteryValue * 100 - (unsigned int)(realBatteryValue) * 100;
//
//	SendData4(0xff);
//	SendData4(batteryValue);
//	
//	SendData4((unsigned char) realBatteryValue);
//	SendData4(left);		
//
//	// 清零
//	for(; i < 10; i++)
//		batteryValue[i] = 0;
//	getBatteryTimes = 0;
//
//	return realBatteryValue;
//	}
//	else
//		return 0;

	
	// 自己采集10次
	/*for(; i < 10; i++){
		batteryValuetmp = read_adc256(4);
		if(batteryValuetmp < batteryValue)
			batteryValue = batteryValuetmp;
		Delay3us();
	}*/

	batteryValue = read_adc256(4);
	
	realBatteryValue = STABLEV * 256 / batteryValue;

//	left = realBatteryValue * 100 - (unsigned int)(realBatteryValue) * 100;

//	SendData4(0xff);
//	SendData4(batteryValue);

//	SendData4(0xaa);
//	SendData4((unsigned char) realBatteryValue);
//	SendData4(left);		


	return realBatteryValue;	// 补偿0.13v

}


unsigned char getBatteryPercent()
{
//	char output[20] = {0};
	unsigned char batteryPercentage = 0;
	double batteryValue = batteryScan();
	int tmp = 0;
	
	if(batteryValue != 0){
		
		batteryPercentage = (batteryValue - 2.6) * 100.0 / (STDVCC - 2.6);

		//SendData4(0xbb);
		//tmp = batteryValue * 100;
		//SendData4((tmp & 0xff00) >> 8);
		//SendData4(tmp & 0xff);
		//SendData4((unsigned char)batteryPercentage);


		return (unsigned char)batteryPercentage;
		}
	else
		return 0;
}




/*
	功能 读取温度
	公式
		V0 = (6.25 mv/C * T C) + 424mv
		V0 = Vcc * 1000 mv * tempAdcValue / 1024
*/ 

double tempScan(void)
{
	unsigned int tempAdcValue[3];	// ADC采集的值
	unsigned int avgTempAdcValue;	// ADC平均温度
	double realTempValue = 0;	// 温度最终结果
	unsigned char left = 0;
	unsigned char i = 0;
	
	double getVcc = batteryScan();	// 获取工作电压

	for(; i < 3; i++){
		tempAdcValue[i] = read_adc1024(5);
		Delay3us();
	}
	avgTempAdcValue = (tempAdcValue[0] + tempAdcValue[1] + tempAdcValue[2]) / 3;
	OledWriteDouble(2,0,avgTempAdcValue);
	realTempValue = ((getVcc * avgTempAdcValue * 0.9765) - 424) / 6.25;

	// left = realTempValue * 100 - (unsigned int)(realTempValue) * 100;

	workState.temp_power_chip = realTempValue;
	
	if(realTempValue > 85)
		System_Power_Vcc_ON_OFF = 0;
	else if(System_Power_Vcc_ON_OFF == 0)
		System_Power_Vcc_ON_OFF = 1;
	//SendData4(0xEE);
	//SendData4(avgTempAdcValue >> 8);
	//SendData4(avgTempAdcValue);
	
	//SendData4(0xEF);
	//SendData4((unsigned char)realTempValue);
	//SendData4(left);
	
	return realTempValue;
}



/*
	功能 检测充电状态
	P07 读到 高或低电平
*/
unsigned char checkCharging(void)
{
	//P0M1 |= 0x80;
	//P0M0 &= ~0x80;	// 本身为高阻输入
	if(P07 == 1)
	{
		//SendString4(" USB charge \n");
		//clear();
		//OledWriteMessage57(" USB charge ");
		OledChargingState(1);
		workState.charge_state = 1;
		return 1;
	}
	else
	{
		//SendString4(" USB  offset \n");
		//clear();
		//OledWriteMessage57(" USB  offset ");
		OledChargingState(0);
		workState.charge_state = 0;
		return 0;
	}
}
