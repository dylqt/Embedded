#include "stc15_adc.h"
#include "iap15.h"

/*
*	参数定义
*/
#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟


/*
*	配置参数
*/
#define ADC_SPEED ADC_SPEEDLL



/*----------------------------
初始化ADC,并开启该端口的中断
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
void AdcIntInit(unsigned char ch)
{
    P1ASF = (1 << ch);                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_START | ch;
    Delay(2);                       //ADC上电并延时
    
	EADC = 1; 
	EA = 1;
}

unsigned char getKeyAdc()
{
	// 读取p1.1端口
	return read_adc256(1);
}
/*----------------------------
	ADC中断服务程序
	// 进入中断以后关闭中断，使用查询模式进行
----------------------------*/
void adc_isr_key() interrupt 5 using 1
{
	unsigned int adcValue1 = 0;
	unsigned int adcValue2 = 0;
	unsigned int keyAdcValue = 0;
	
	EADC = 0;						// 关闭adc中断
    ADC_CONTR &= !ADC_FLAG;         //清除ADC中断标志
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	Delay10ms();					// 延时10ms，消抖

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
	EADC = 1;	// 开启adc中断
}

/* 
采集一次adc的值，精度1/256, 8位
输入 ch : 端口
输出 : 直接的采集结果
*/
unsigned char read_adc256(unsigned char ch)
{
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	ADC_CONTR &= !ADC_FLAG;		  //清除ADC中断标志 --->  Close ADC

	return ADC_RES;                 //返回ADC结果		
}

/*
功能 : 将adc采集到的数字量，通过基准值计算，得到模拟值大小、
公式 : adc_result = 256 * (vin / vcc)
输入 : 
	adc_result : 数字量
	base : 基准值
输出 : 模拟量大小
*/
double dToA256(unsigned char adc_result, double base)
{
	return (base * adc_result / 256);
}



/* 
采集一次adc的值，精度1/1024, 10位
输入 ch : 端口
输出 : 直接的采集结果
*/
unsigned int read_adc1024(unsigned char ch)
{
	unsigned int output = 0;
	ADC_RES = 0;
	ADC_RESL = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    //ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	ADC_CONTR &= !ADC_FLAG;		  //清除ADC中断标志 --->  Close ADC

	output = (ADC_RES << 2);	// 没有配置CLK_DIV.5(ADRJ),默认ADC_RES存放高8位, ADC_RESL存放低2位
	output |= ADC_RESL;
	return output;                 //返回ADC结果		
}

/*	
功能 : 将adc采集到的数字量，通过基准值计算，得到模拟值大小
公式 : adc_result = 1024 * (vin / vcc)
输入 : 
	adc_result : 数字量
	base : 基准值
输出 : 模拟量大小

*/
double dToA1024(unsigned int adc_result, double base)
{
	return (base * adc_result / 1024);
}




/*----------------------------
软件延时
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



