#ifndef __HARDWAR_H__
#define __HARDWAR_H__

#include <msp430x14x.h>

/*---------------------------------------------------------
                   �������ƿڶ���
---------------------------------------------------------*/
#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long


void WDT_init(void);                                   //���Ź�����
void Clock_init(void);                                 //ϵͳʱ������
void Port_init(void);                                  //ϵͳ��ʼ��������IO������

#endif