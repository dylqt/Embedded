#include "stc15_adc.h"
#include "iap15.h"

/*
*	��������
*/
#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��


/*
*	���ò���
*/
#define ADC_SPEED ADC_SPEEDLL



/*----------------------------
��ʼ��ADC,�������ö˿ڵ��ж�
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
void AdcIntInit(unsigned char ch)
{
    P1ASF = (1 << ch);                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_START | ch;
    Delay(2);                       //ADC�ϵ粢��ʱ
    
	EADC = 1; 
	EA = 1;
}

unsigned char getKeyAdc()
{
	// ��ȡp1.1�˿�
	return read_adc256(1);
}
/*----------------------------
	ADC�жϷ������
	// �����ж��Ժ�ر��жϣ�ʹ�ò�ѯģʽ����
----------------------------*/
void adc_isr_key() interrupt 5 using 1
{
	unsigned int adcValue1 = 0;
	unsigned int adcValue2 = 0;
	unsigned int keyAdcValue = 0;
	
	EADC = 0;						// �ر�adc�ж�
    ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	Delay10ms();					// ��ʱ10ms������

	adcValue1 = read_adc256(3);
	Delay10ms();
	adcValue2 = read_adc256(3);

	if(adcValue1 > adcValue2){
		if(adcValue1 - adcValue2 < 35)
			keyAdcValue = (adcValue1 + adcValue2) / 2;
		else goto KEY_END;
	}
	else if(adcValue1 < adcValue2){
		if(adcValue2 - adcValue1 < 35)
			keyAdcValue = (adcValue1 + adcValue2) / 2;
		else goto KEY_END;
	}
	else keyAdcValue = adcValue1;

	if(keyAdcValue < 18){	    // S1 : tuch	0

	}else if(keyAdcValue < 59){	// S2 : key+ 	36

	}else if(keyAdcValue < 105){ // S3 : key-	82
	
	}else if(keyAdcValue < 155){ // S4 : Mode	126
	
	}else if(keyAdcValue < 205){ // S5 : Enter	185
	
	}
KEY_END:	
	EADC = 1;	// ����adc�ж�
}

/* 
�ɼ�һ��adc��ֵ������1/256, 8λ
���� ch : �˿�
��� : ֱ�ӵĲɼ����
*/
unsigned char read_adc256(unsigned char ch)
{
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	ADC_CONTR &= !ADC_FLAG;		  //���ADC�жϱ�־ --->  Close ADC

	return ADC_RES;                 //����ADC���		
}

/*
���� : ��adc�ɼ�������������ͨ����׼ֵ���㣬�õ�ģ��ֵ��С��
��ʽ : adc_result = 256 * (vin / vcc)
���� : 
	adc_result : ������
	base : ��׼ֵ
��� : ģ������С
*/
double dToA256(unsigned char adc_result, double base)
{
	return (base * adc_result / 256);
}



/* 
�ɼ�һ��adc��ֵ������1/1024, 10λ
���� ch : �˿�
��� : ֱ�ӵĲɼ����
*/
unsigned int read_adc1024(unsigned char ch)
{
	unsigned int output = 0;
	ADC_RES = 0;
	ADC_RESL = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	ADC_CONTR &= !ADC_FLAG;		  //���ADC�жϱ�־ --->  Close ADC

	output = (ADC_RES << 2);	// û������CLK_DIV.5(ADRJ),Ĭ��ADC_RES��Ÿ�8λ, ADC_RESL��ŵ�2λ
	output |= ADC_RESL;
	return output;                 //����ADC���		
}

/*	
���� : ��adc�ɼ�������������ͨ����׼ֵ���㣬�õ�ģ��ֵ��С
��ʽ : adc_result = 1024 * (vin / vcc)
���� : 
	adc_result : ������
	base : ��׼ֵ
��� : ģ������С

*/
double dToA1024(unsigned int adc_result, double base)
{
	return (base * adc_result / 1024);
}




/*----------------------------
�����ʱ
----------------------------*/
void Delay(unsigned char n)
{
    unsigned int x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}



