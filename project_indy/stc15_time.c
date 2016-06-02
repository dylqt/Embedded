#include "stc15_time.h"
#include "type.h"

/*
����:	��ʱ��0��ʱ���ܣ������ж�
����:	time : ��ʱ��ʱ��us
*/
void Timer0Init(unsigned int time)		//100΢��@22.1184MHz
{

	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = (65536 - (FOSC / 1000000 * time));		//���ö�ʱ��ֵ
	TH0 = (65536 - (FOSC / 1000000 * time))>>8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־		�ȴ�TF0
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}
