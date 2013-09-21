//定义了一部分宏值，用于同上位机的通讯
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

//默认收到信息后返回此信号
#define SIG_RESPONSE_SEG 0x10

//对应菜单项，长按菜单项1，返回SIG_RESPONSE_SEG(1)，依次类推....
#define SIG_RESPONSE_SEG_MENU(index) (SIG_RESPONSE_SEG + MENU_MESSAGE_BASE_SEG + index)

#endif