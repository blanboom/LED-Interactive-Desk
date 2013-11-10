#ifndef _APP_H_
#define _APP_H_

#include "system.h"
#include "stc15.h"

#define T20MS (65536 - FOSC / 50)    // ��ʱ��ʱ��

#define LED_MODE_NUM 6 // LED ģʽ����

//  ����
sbit KEY = P3^2;
#define KEY_PRESSED 0

extern void init();
extern void lightUp(pin);
extern void ledProcess(pin);
extern void keyProcess();

#endif