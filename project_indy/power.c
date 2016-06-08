#include "power.h"

#define STDVCC 4.2		// ��׼��ѹ4.2v
#define STABLEV 1.28	// �ȶ���ѹֵ1.27v

unsigned char getBatteryTimes = 10;

void batteryInit(void)
{
	 //��ʼ��  u1.8   u1.6  u1.7  Ϊǿ���ģʽ��
	 P6M1 &= ~0x0e;
	 P6M0 |= 0x0e;
	 //u1.5 Ϊ�ڲ�������������ģʽ��
	 P6M1 &= ~0x01;
	 P6M0 &= ~0x01;
	 //��u1.8Ϊ�͵�ƽ��u1.6Ϊ�ߵ�ƽ�� u1.7Ϊ�ߵ�ƽ��

	 //SendString4("Sys = 1 \n ");
	 System_Power_Vcc_ON_OFF = 0;
	 VSEL = 1;
	 BYP = 1;
	 
	 PG = 1;	// ��Դģ��״̬��־
	 
	 //SendString4("Sys = 0 \n ");
	//Delay999ms();
	 P4M1 = 0x00;
	 P4M0 = 0x01;
	 death_ctron = 0;	// �����͸�λ��
	 System_Power_Vcc_ON_OFF = 1; // ���ܵ�Դ
	 
	// SendString4("Sys = 1 \n ");
	 
	 Delay100ms();
	 death_ctron = 1;	// �رո�λ��
}


/*
	���� ��ȡ��ѹ
	�ֱ���1024����׼��ѹ1.27v
	�ɼ�10�Σ�ȡ�м�3��ƽ��
*/ 
double batteryScan(void)
{	
	unsigned int batteryValue = 0;
	unsigned int batteryValuetmp = 0;
	unsigned char i = 0;
	double realBatteryValue = 0;
	unsigned int left = 0;
		
	//SendString4(" batteryScan in \n ");
	// ��ѭ��һ�βɼ�һ��
//	batteryValue[getBatteryTimes++] = read_adc1024(4);
//	
//	if(getBatteryTimes == 10){	// �ɼ�10�� ���һ��
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
//	// ����
//	for(; i < 10; i++)
//		batteryValue[i] = 0;
//	getBatteryTimes = 0;
//
//	return realBatteryValue;
//	}
//	else
//		return 0;

	
	// �Լ��ɼ�10��
	for(; i < 10; i++){
		batteryValuetmp = read_adc1024(4);
		if(batteryValuetmp > batteryValue)
			batteryValue = batteryValuetmp;
		Delay3us();
	}

	
	realBatteryValue = STABLEV * 1024 / batteryValue;

	left = realBatteryValue * 100 - (unsigned int)(realBatteryValue) * 100;

//	SendData4(0xff);
//	SendData4(batteryValue);

//	SendData4(0xaa);
//	SendData4((unsigned char) realBatteryValue);
//	SendData4(left);		


	return realBatteryValue;

}


unsigned char getBatteryPercent()
{
//	char output[20] = {0};
	unsigned char batteryPercentage = 0;
	double batteryValue = batteryScan();

	
	if(batteryValue != 0){
		
		batteryPercentage = (batteryValue - 2.7) * 100 / (STDVCC - 2.7);

//		sprintf(output, "BAT %d\% \n ", batteryPercentage);
//		SendData4(0xbb);
//		SendData4(batteryValue);
		
//		SendData4((unsigned char)batteryPercentage);
//		SendString4(output);
//		WriteTestMessage57(output);
		return (unsigned char)batteryPercentage;
		}
	else
		return 0;
}




/*
	���� ��ȡ�¶�
	��ʽ
		V0 = (6.25 mv/C * T C) + 424mv
		V0 = Vcc * 1000 mv * tempAdcValue / 1024
*/ 

double tempScan(void)
{
	unsigned int tempAdcValue[10];	// ADC�ɼ���ֵ
	unsigned int avgTempAdcValue;	// ADCƽ���¶�
	double realTempValue = 0;	// �¶����ս��
	unsigned char left = 0;
	unsigned char i = 0;
	
	double getVcc = batteryScan();	// ��ȡ������ѹ

	for(; i < 10; i++){
		tempAdcValue[i] = read_adc1024(5);
		Delay3us();
	}
	avgTempAdcValue = (tempAdcValue[3] + tempAdcValue[6] + tempAdcValue[9]) / 3;
	
	realTempValue = ((getVcc * avgTempAdcValue * 0.9765) - 424) / 6.25;

	left = realTempValue * 100 - (unsigned int)(realTempValue) * 100;

	if(realTempValue > 85)
		System_Power_Vcc_ON_OFF = 0;
		
	//SendData4(0xEE);
	//SendData4(avgTempAdcValue >> 8);
	//SendData4(avgTempAdcValue);
	
	//SendData4(0xEF);
	//SendData4((unsigned char)realTempValue);
	//SendData4(left);
	
	return realTempValue;
}



/*
	���� �����״̬
	P07 ���� �߻�͵�ƽ
*/
unsigned char checkCharging(void)
{
	//P0M1 |= 0x80;
	//P0M0 &= ~0x80;	// ����Ϊ��������
	if(P07 == 1)
	{
		SendString4(" USB charge \n");
		//clear();
		OledWriteMessage57(" USB charge ");
		return 1;
	}
	else
	{
		SendString4(" USB  offset \n");
		//clear();
		OledWriteMessage57(" USB  offset ");
		return 0;
	}
}
