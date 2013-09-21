#ifndef __HARDWAR_H__
#define __HARDWAR_H__

#include <msp430x14x.h>

/*---------------------------------------------------------
                   公共控制口定义
---------------------------------------------------------*/
#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long


void WDT_init(void);                                   //看门狗设置
void Clock_init(void);                                 //系统时钟设置
void Port_init(void);                                  //系统初始化，设置IO口属性

#endif