#include "app.h"
#include "stc15.h"
#include "system.h"
#include <stdio.h>

uint8 pwmTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // PWM ��ʱ
uint8 ledMode = 0;      // LED ģʽ
uint16 ledTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // LED ʱ�䣬0 Ϊδ����
int8 code ledSensitivityAdjust[8] = {-20, -30, -20, -30, -25, -20, 0, -20};  //������У��
uint16 blinkCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // LED��˸��ʱ��
bit sensitivityMode = 0;  // ������ģʽ�� 0 - ҹ�� 1 - ����
uint8 sensitivity = SENSITIVITY_NIGHT;   // ������

void init()
{
	initADC();
	
	KEY = !KEY_PRESSED;
	P2 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;
	
	// ��ʼ�� LED ��ʱ������ʱ�� 0��
	AUXR |= 0x80;  // ��ʱ�� 0��1T ģʽ
	TMOD = 0x03;   // ��ʱ�� 0 ģʽ 3
  TL0 = T20MS;   // װ��ʱ�䣺20ms
	TH0 = T20MS >> 8;
	TR0 = 1;       // ��ʼ��ʱ
	ET0 = 1;       // ʹ�ܶ�ʱ�� 0 ��ģʽ 3 �µ��ж�
}

// ���� LED
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

// LED �� PWM ���Ƴ���
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

// LED ������˸
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

// LED ������˸
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

	
// LED ����
// ����ָ����ʱ����� LED ����
void ledProcess(uint8 pin)
{
	switch (ledMode)
	{		
	case 0 :
			// ����
			if(ledTime[pin] != 0)
			{
				ledPWM(pin, (260-ledTime[pin]) / 13);
			}
			break;
			
	case 1:
			// ������˸
			if(ledTime[pin] != 0)
			{
				ledBlink1(pin);
			}
			break;
			
	case 2:
			// ��˸
			if(ledTime[pin] != 0)
			{
				ledBlink2(pin);
			}
			break;

	case 3:
			// �ַ���ȥ����
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

// ��������
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
				
				// ����������
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