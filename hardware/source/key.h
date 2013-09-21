#ifndef _KEY_H_
#define _KEY_H_

#define KEY_VALUE_1       0x01                   //按键
#define KEY_VALUE_NULL    0xFF                   //无按键

//定义按键返回值状态(按下,长按,连发,释放) 
#define KEY_DOWN                 0x80 
#define KEY_LONG                 0x40 
#define KEY_CONTINUE             0x20 
#define KEY_UP                   0x10 

//这个状态有点特殊，代表按键长按后松开
#define KEY_UP_LONG              0x11

void KEY_init();
uchar KEY_scan();
void KEY_test();

#endif