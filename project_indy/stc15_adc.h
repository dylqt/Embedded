#ifndef __STC15_ADC_H__
#define __STC15_ADC_H__

#include "delay.h"
#include "beep.h"
//extern unsigned int keyAdcValue;
/*----------------------------
初始化ADC
输入 ch :
	0 :	选择P1.0作为A/D输入来用
	1 :	选择P1.1作为A/D输入来用
	2 :	选择P1.2作为A/D输入来用
	3 :	选择P1.3作为A/D输入来用
	4 :	选择P1.4作为A/D输入来用
	5 :	选择P1.5作为A/D输入来用
	6 :	选择P1.6作为A/D输入来用
	7 :	选择P1.7作为A/D输入来用	
----------------------------*/
#define AdcInit(ch) 	(P1ASF = (1 << ch) )         //设置ch口为AD口 

//初始化ADC,并开启该端口的中断
void AdcIntInit(unsigned char ch);
void Delay(unsigned char n);

/* 
功能 : 采集一次adc的值，精度1/256, 8位
输入 : ch : 端口
输出 : 直接的采集结果
*/
unsigned char read_adc256(unsigned char ch);

/*
功能 : 将adc采集到的数字量，通过基准值计算，得到模拟值大小、
公式 : adc_result = 256 * (vin / vcc)
输入 : 
	adc_result : 数字量
	base : 基准值
输出 : 模拟量大小
*/
double dToA256(unsigned char adc_result, double base);


/* 
功能 : 采集一次adc的值，精度1/1024, 10位
输入 : ch : 端口
输出 : 直接的采集结果
*/
unsigned int read_adc1024(unsigned char ch);

/*	
功能 : 将adc采集到的数字量，通过基准值计算，得到模拟值大小
公式 : adc_result = 1024 * (vin / vcc)
输入 : 
	adc_result : 数字量
	base : 基准值
输出 : 模拟量大小

*/
double dToA1024(unsigned int adc_result, double base);




#endif
