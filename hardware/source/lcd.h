#ifndef __LCD_H__
#define __LCD_H__

#include "hardware.h"

/*******************LCD对外接口****************************/
void LCD_write_data(uchar);
void LCD_set_position(uchar lcdpage,uchar lcdline);
void LCD_prints(uchar *);
void LCD_printc(uchar); 
/************以上部分均为空接口，留作不同型号的LCD扩展*****/

//硬件初始化
void LCD_initial();
//清屏
void LCD_cls();

//暂时关闭
void LCD_turn_off();

//打开
void LCD_turn_on();

//显示汉字
void LCD_displayCgrom(char *hz);
//供debug使用
void LCD_debug_displayCgrom(char *hz);
//显示单行汉字
void LCD_displayCgromLine(uchar line, char *hz);

//测试函数
void LCD_test();

#endif