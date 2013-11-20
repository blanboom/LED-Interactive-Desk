#include "system.h"
#include "stc15.h"
#include <intrins.h>

// �����ʱ
void delay(uint16 n)
{
	uint16 x;
	
	while(n--)
	{
		x = 5000;
		while(x--);
	}
}


//////////////////////////////////////////////////////////////
//  ADC

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER 0x80 //ADC power control bit
#define ADC_FLAG 0x10 //ADC complete flag
#define ADC_START 0x08 //ADC start control bit
#define ADC_SPEEDLL 0x00 //540 clocks
#define ADC_SPEEDL 0x20 //360 clocks
#define ADC_SPEEDH 0x40 //180 clocks
#define ADC_SPEEDHH 0x60 //90 clocks

// ��ʼ�� ADC
void initADC()
{
	P1ASF = 0xff;      // ���� P1 ��Ϊ AD ��
	ADC_RES = 0;       // �������Ĵ���
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	delay(2);          // ADC �ϵ粢��ʱ
}

// ��ȡ ADC ���
uint8 getADCResult(uint8 ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR & ADC_FLAG));   // �ȴ� ADC ת�����
	ADC_CONTR &= ~ADC_FLAG;           // �ر� ADC
	
	return ADC_RES;                   // ���� ADC ���
}