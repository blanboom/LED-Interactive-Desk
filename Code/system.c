#include "system.h"
#include "stc15.h"
#include <intrins.h>

// 软件延时
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
//  串口

// 初始化串口
void initUart()
{
	SCON = 0x5a;       // 设置串口为 8 位可变波特率
  #if URMD == 0
  T2L = 0xd8;        // 设置波特率重装值
	T2H = 0xff;        // 115200 bps(65536-1843200/4/115200)
	AUXR |= 0x14;       // T2 为 1T 模式，并启动定时器 2
	AUXR |= 0x01;      // 选择定时器 2 为串口 1 的波特率发生器
	#elif URMD == 1
	AUXR = 0x40;       // 定时器 1 为 1T 模式
	TMOD = 0x00;       // 定时器 1 为 模式 0 （16 位自动重载）
	TL1 = 0xd8;        // 设置波特率重装值
	TH1 = 0xff;        // 115200 bps(65536-18432000/4/115200)
	TR1 = 1;           // 定时器 1 开始启动
	#else
	TMOD = 0x20;       // 设置定时器 1 为 8 位自动重装模式
	AUXR = 0x40;       // 定时器 1 为 1T 模式
	TH1 = TL1 = 0xfb;  // 115200 bps(256-1843200/32/115200)
	#endif
}
	
// 发送串口数据
void sendData(uint8 dat)
{
	while(!TI);        // 等待前一个数据发送完成
	TI = 0;            // 清除发送标志
	SBUF = dat;        // 发送当前数据
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

// 初始化 ADC
void initADC()
{
	P1ASF = 0xff;      // 设置 P1 口为 AD 口
	ADC_RES = 0;       // 清除结果寄存器
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	delay(2);          // ADC 上电并延时
}

// 读取 ADC 结果
uint8 getADCResult(uint8 ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR & ADC_FLAG));   // 等待 ADC 转换完成
	ADC_CONTR &= ~ADC_FLAG;           // 关闭 ADC
	
	return ADC_RES;                   // 返回 ADC 结果
}