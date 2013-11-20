#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define FOSC (1843200L)

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef int int8;




extern void initADC();
uint8 getADCResult(uint8 ch);
extern void delay(uint16 n);

#endif