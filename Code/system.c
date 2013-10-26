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
//  ����

// ��ʼ������
void initUart()
{
	SCON = 0x5a;       // ���ô���Ϊ 8 λ�ɱ䲨����
  #if URMD == 0
  T2L = 0xd8;        // ���ò�������װֵ
	T2H = 0xff;        // 115200 bps(65536-1843200/4/115200)
	AUXR |= 0x14;       // T2 Ϊ 1T ģʽ����������ʱ�� 2
	AUXR |= 0x01;      // ѡ��ʱ�� 2 Ϊ���� 1 �Ĳ����ʷ�����
	#elif URMD == 1
	AUXR = 0x40;       // ��ʱ�� 1 Ϊ 1T ģʽ
	TMOD = 0x00;       // ��ʱ�� 1 Ϊ ģʽ 0 ��16 λ�Զ����أ�
	TL1 = 0xd8;        // ���ò�������װֵ
	TH1 = 0xff;        // 115200 bps(65536-18432000/4/115200)
	TR1 = 1;           // ��ʱ�� 1 ��ʼ����
	#else
	TMOD = 0x20;       // ���ö�ʱ�� 1 Ϊ 8 λ�Զ���װģʽ
	AUXR = 0x40;       // ��ʱ�� 1 Ϊ 1T ģʽ
	TH1 = TL1 = 0xfb;  // 115200 bps(256-1843200/32/115200)
	#endif
}
	
// ���ʹ�������
void sendData(uint8 dat)
{
	while(!TI);        // �ȴ�ǰһ�����ݷ������
	TI = 0;            // ������ͱ�־
	SBUF = dat;        // ���͵�ǰ����
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