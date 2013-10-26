#include "app.h"
#include "stc15.h"
#include "system.h"
#include <stdio.h>

uint8 pwmTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // PWM 计时
uint8 ledMode = 0;      // LED 模式
uint16 ledTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // LED 时间，0 为未启动

//  LED 模式
//  数值代表亮度，范围：0 ~ 255
//  第一个数代表后面每个亮度的持续时间，单位为 20ms
uint8 ledProgram[4][256]=
{
	{8,255,0,0,0,0,0,0,0,0,0,0,0,0},  // 立即灭掉
	{100,255,0,0,0,0,0,0,0,0,0,0,0,0},  // 持续 2 秒
	{100,255,245,235,225,215,
			 205,195,185,175,165,
			 155,145,135,125,115,
			 95,85,65,35,0},  // 逐渐变暗，2 秒
	{100,255,245,235,225,215,
			 205,195,185,175,165,
			 155,145,135,125,115,
			 95, 75, 35, 255, 0},  // 变暗后闪烁一次，共 2 秒
};

void init()
{
	initUart();
	initADC();
	
	KEY = !KEY_PRESSED;
	P2 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;
	
	// 初始化 LED 计时器（定时器 0）
	AUXR |= 0x80;  // 定时器 0：1T 模式
	TMOD = 0x03;   // 定时器 0 模式 3
  TL0 = T20MS;   // 装入时间：20ms
	TH0 = T20MS >> 8;
	TR0 = 1;       // 开始计时
	ET0 = 1;       // 使能定时器 0 在模式 3 下的中断
	
	
	printf("LED Interactive Desk\nby xiao3d and Blanboom\nhttp://blanboom.org\n");
}

// 启动 LED
void lightUp(pin)
{
	uint8 result;
	P1 = 0x01 << pin;
	result = getADCResult(pin);
	if (result > 30)
	{
		ledTime[pin] = 1;
	}
}

// LED 的 PWM 控制程序
void ledPWM(uint8 pin, uint8 brightness)
{
	pwmTime[pin] = pwmTime[pin] + 1;
	if(pwmTime[pin] >= 256)
	{
		pwmTime[pin] = 0;
	}
	if(pwmTime[pin] <= brightness)
	{
		P2 |= 0x01 << pin;
	}
	if(pwmTime[pin] > brightness)
	{
		P2 &= 0xfe << pin;
	}
}

// LED 处理
// 到达指定的时间调节 LED 亮度
void ledProcess(uint8 pin)
{
	/*
	if(ledTime[pin] != 0)
	{
		ledPWM(pin, (375-ledTime[pin]) / 6);
	}
	*/
	
	
	if(((ledTime[pin] - 1) % ledProgram[ledMode][0]) == 0)
	{
		ledPWM(pin, ledProgram[ledMode][(ledTime[pin] - 1) / ledProgram[ledMode][0]]);
	}
	
}

// 按键处理
void keyProcess()
{
	static uint16 keyTime = 0;
	static bit keyLock = 0;
	if(KEY == KEY_PRESSED)
	{
		keyTime++;
		if(keyTime <= 200 && keyLock == 0)
		{
			ledMode++;
			if(ledMode > LED_MODE_NUM - 1)
			{
				ledMode = 0;
			}
			keyLock = 1;
		}
	}
	if(keyTime > 200)
	{
		if(KEY != KEY_PRESSED)
		{
			keyTime = 0;
			keyLock = 0;
		}
	}
}
	
void ledTimeUpdate() interrupt 1 using 1
{
	uint8 i;
	for(i = 0; i <= 7; i++)
	{
		if(ledTime[i] != 0)
		{
			ledTime[i] = ledTime[i] + 1;
			if(ledTime[i] == 375)
			{
				ledTime[i] = 0;
				P2 &= 0xfe << i;
			}
		}
	}
}