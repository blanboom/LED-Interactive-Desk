#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define FOSC (1843200L)

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef int int8;



//////////////////////////////////////////////////////////////
//  串口

#define BAUD 9600

#define URMD 0     
// 0: 使用定时器2做为波特率发生器
// 1: 使用定时器1的模式0（16位自动重载模式）做为波特率发生器
// 2: 使用定时器1的模式2（8位自动重载模式）做为波特率发生器

extern void initUart();
extern void sendData(uint8 dat);
extern void initADC();
uint8 getADCResult(uint8 ch);
void showResult(uint8 ch);
extern void delay(uint16 n);

#endif