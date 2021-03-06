#ifndef _APP_H_
#define _APP_H_

#include "system.h"
#include "stc15.h"

#define T20MS (65536 - FOSC / 50)    // 定时器时间

#define LED_MODE_NUM 4 // LED 模式数量

#define SENSITIVITY_NIGHT 60  // 夜晚灵敏度
#define SENSITIVITY_DAY 100   // 白天灵敏度

//  按键
sbit KEY = P3^2;
#define KEY_PRESSED 0

extern void init();
extern void lightUp(pin);
extern void ledProcess(pin);
extern void keyProcess();

#endif