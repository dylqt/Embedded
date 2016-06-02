#ifndef __STC15_ADC_H__
#define __STC15_ADC_H__

#include "delay.h"
#include "beep.h"
//extern unsigned int keyAdcValue;
/*----------------------------
��ʼ��ADC
���� ch :
	0 :	ѡ��P1.0��ΪA/D��������
	1 :	ѡ��P1.1��ΪA/D��������
	2 :	ѡ��P1.2��ΪA/D��������
	3 :	ѡ��P1.3��ΪA/D��������
	4 :	ѡ��P1.4��ΪA/D��������
	5 :	ѡ��P1.5��ΪA/D��������
	6 :	ѡ��P1.6��ΪA/D��������
	7 :	ѡ��P1.7��ΪA/D��������	
----------------------------*/
#define AdcInit(ch) 	(P1ASF = (1 << ch) )         //����ch��ΪAD�� 

//��ʼ��ADC,�������ö˿ڵ��ж�
void AdcIntInit(unsigned char ch);
void Delay(unsigned char n);

/* 
���� : �ɼ�һ��adc��ֵ������1/256, 8λ
���� : ch : �˿�
��� : ֱ�ӵĲɼ����
*/
unsigned char read_adc256(unsigned char ch);

/*
���� : ��adc�ɼ�������������ͨ����׼ֵ���㣬�õ�ģ��ֵ��С��
��ʽ : adc_result = 256 * (vin / vcc)
���� : 
	adc_result : ������
	base : ��׼ֵ
��� : ģ������С
*/
double dToA256(unsigned char adc_result, double base);


/* 
���� : �ɼ�һ��adc��ֵ������1/1024, 10λ
���� : ch : �˿�
��� : ֱ�ӵĲɼ����
*/
unsigned int read_adc1024(unsigned char ch);

/*	
���� : ��adc�ɼ�������������ͨ����׼ֵ���㣬�õ�ģ��ֵ��С
��ʽ : adc_result = 1024 * (vin / vcc)
���� : 
	adc_result : ������
	base : ��׼ֵ
��� : ģ������С

*/
double dToA1024(unsigned int adc_result, double base);




#endif
