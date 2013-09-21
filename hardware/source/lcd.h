#ifndef __LCD_H__
#define __LCD_H__

#include "hardware.h"

/*******************LCD����ӿ�****************************/
void LCD_write_data(uchar);
void LCD_set_position(uchar lcdpage,uchar lcdline);
void LCD_prints(uchar *);
void LCD_printc(uchar); 
/************���ϲ��־�Ϊ�սӿڣ�������ͬ�ͺŵ�LCD��չ*****/

//Ӳ����ʼ��
void LCD_initial();
//����
void LCD_cls();

//��ʱ�ر�
void LCD_turn_off();

//��
void LCD_turn_on();

//��ʾ����
void LCD_displayCgrom(char *hz);
//��debugʹ��
void LCD_debug_displayCgrom(char *hz);
//��ʾ���к���
void LCD_displayCgromLine(uchar line, char *hz);

//���Ժ���
void LCD_test();

#endif