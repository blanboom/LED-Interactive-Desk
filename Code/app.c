#include "app.h"
#include "stc15.h"
#include "system.h"
#include <stdio.h>

uint8 pwmTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // PWM 计时
uint8 ledMode = 0;      // LED 模式
uint16 ledTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // LED 时间，0 为未启动
int8 code ledSensitivityAdjust[8] = {-20, -30, -20, -30, -25, -20, 0, -20};  //灵敏度校正
uint16 blinkCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // LED闪烁计时器
bit sensitivityMode = 0;  // 灵敏度模式， 0 - 夜晚， 1 - 白天
uint8 sensitivity = SENSITIVITY_NIGHT;   // 灵敏度

void init()
{
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
}

// 启动 LED
void lightUp(pin)
{
	uint8 result;
	P1 = 0x01 << pin;
	result = getADCResult(pin);
	if (result > (sensitivity + ledSensitivityAdjust[pin]))
	{
		ledTime[pin] = 1;
	}
}

// LED 的 PWM 控制程序
void ledPWM(uint8 pin, uint8 brightness)
{
	pwmTime[pin] = pwmTime[pin] + 1;
	if(pwmTime[pin] >= 20)
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

// LED 慢速闪烁
void ledBlink1(pin)
{
	blinkCounter[pin] = blinkCounter[pin] + 1;
	if(blinkCounter[pin] == 200)
	{
		P2 |= 0x01 << pin;
	}
	if(blinkCounter[pin] == 224)
	{
		P2 &= 0xfe << pin;
	}
	if(blinkCounter[pin] >= 400)
	{
		blinkCounter[pin] = 0;
	}
}

// LED 快速闪烁
void ledBlink2(pin)
{
	blinkCounter[pin] = blinkCounter[pin] + 1;
	if(blinkCounter[pin] == 100)
	{
		P2 |= 0x01 << pin;
	}
	if(blinkCounter[pin] == 124)
	{
		P2 &= 0xfe << pin;
	}
	if(blinkCounter[pin] >= 200)
	{
		blinkCounter[pin] = 0;
	}
}

	
// LED 处理
// 到达指定的时间调节 LED 亮度
void ledProcess(uint8 pin)
{
	switch (ledMode)
	{		
	case 0 :
			// 渐变
			if(ledTime[pin] != 0)
			{
				ledPWM(pin, (260-ledTime[pin]) / 13);
			}
			break;
			
	case 1:
			// 快速闪烁
			if(ledTime[pin] != 0)
			{
				ledBlink1(pin);
			}
			break;
			
	case 2:
			// 闪烁
			if(ledTime[pin] != 0)
			{
				ledBlink2(pin);
			}
			break;

	case 3:
			// 手放上去灯灭
		  if(ledTime[pin] == 0)
			{
				P2 |= 0x01 << pin;
			}
			else
			{
				P2 &= 0xfe << pin;
			}
			break;
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
		if(keyTime >= 1000 && keyLock == 0)
		{
			ledMode++;
			if(ledMode > LED_MODE_NUM - 1)
			{
				ledMode = 0;
				
				// 设置灵敏度
				sensitivityMode = ~sensitivityMode;
				if (sensitivityMode == 1)
				{
					sensitivity = SENSITIVITY_DAY;
				}
				else
				{
					sensitivity = SENSITIVITY_NIGHT;
				}
				
				P2 = 0x00;
			}
			keyLock = 1;
		}
	}
	if(keyTime > 2000)
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
			if(ledTime[i] >= 260)
			{
				ledTime[i] = 0;
				P2 &= 0xfe << i;
			}
		}
	}
}