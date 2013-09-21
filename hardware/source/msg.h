#ifndef __MSG_H__
#define __MSG_H__

#include "hardware.h"

#define MESSAGETYPE_COM 0x01
#define MESSAGETYPE_KEY 0x02

#define MESSAGETYPE_GUI 0x11
#define MESSAGETYPE_GUI_SHOW_MESSAGE 0x12
#define MESSAGETYPE_GUI_SHOW_MENU 0x13
#define MESSAGETYPE_GUI_SHOW_RESPONSE 0x14
#define MESSAGETYPE_GUI_SHOW_WELCOME 0x15

#define MESSAGETYPE_SYS 0x21
#define MESSAGETYPE_SYS_SPARK 0x22

#define MESSAGETYPE_NONE 0x32

/******************消息格式定义*******************/

/*键盘消息格式定义*/
typedef struct  tagKeyMsg{
    uint  type;
    union{uchar kch[2]; int ktat;}key_value;
} KeyMsg;

/*系统消息格式定义*/
typedef struct  tagSysMsg{
  //消息值，如果是spark消息，代表剩余闪光次数
  uchar sys_value;
  
  //延时时间，因为闪光有个持续性，这个值定义了两次闪光之间的间隔时间
  uchar sys_delay_value;
} SysMsg;

/*COM消息格式定义*/
typedef struct  tagComMsg{
    uint com_value;
} ComMsg;

/*GUI消息格式定义*/
typedef struct tagGuiMsg{
  //GUI描画子分类，分为响应画面，菜单画面，消息显示画面
  uchar gui_type;
  
  //延时描画，假如我们希望上一个描画画面在屏幕上显示一段时间(即使下一条消息已经接收到的情况下)
  //那么描画想要一直显示的画面后，在这里设一个延时值(0--255) X 10ms
  uchar gui_delay_value;
} GuiMsg;

typedef union tagMsgValue{
  KeyMsg key_message;
  SysMsg sys_message;
  ComMsg com_message;
  GuiMsg gui_message;
} MsgValue_u;

typedef struct  tagMsg{
    uchar       msg_type;         /*msg_type分别表示COM口、键盘、GUI、系统消息，若有消息即赋值为1*/
    MsgValue_u  message;   
} Msg;
/***********************定义结束************************/
void MSG_init();
void MSG_sendMessage(Msg *message);
Msg MSG_getMessage();
void MSG_dealMessage(const Msg *message);

#endif