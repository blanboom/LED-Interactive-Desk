#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define FOSC (1843200L)

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef int int8;



//////////////////////////////////////////////////////////////
//  ����

#define BAUD 9600

#define URMD 0     
// 0: ʹ�ö�ʱ��2��Ϊ�����ʷ�����
// 1: ʹ�ö�ʱ��1��ģʽ0��16λ�Զ�����ģʽ����Ϊ�����ʷ�����
// 2: ʹ�ö�ʱ��1��ģʽ2��8λ�Զ�����ģʽ����Ϊ�����ʷ�����

extern void initUart();
extern void sendData(uint8 dat);
extern void initADC();
uint8 getADCResult(uint8 ch);
void showResult(uint8 ch);
extern void delay(uint16 n);

#endif