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

/******************��Ϣ��ʽ����*******************/

/*������Ϣ��ʽ����*/
typedef struct  tagKeyMsg{
    uint  type;
    union{uchar kch[2]; int ktat;}key_value;
} KeyMsg;

/*ϵͳ��Ϣ��ʽ����*/
typedef struct  tagSysMsg{
  //��Ϣֵ�������spark��Ϣ������ʣ���������
  uchar sys_value;
  
  //��ʱʱ�䣬��Ϊ�����и������ԣ����ֵ��������������֮��ļ��ʱ��
  uchar sys_delay_value;
} SysMsg;

/*COM��Ϣ��ʽ����*/
typedef struct  tagComMsg{
    uint com_value;
} ComMsg;

/*GUI��Ϣ��ʽ����*/
typedef struct tagGuiMsg{
  //GUI�軭�ӷ��࣬��Ϊ��Ӧ���棬�˵����棬��Ϣ��ʾ����
  uchar gui_type;
  
  //��ʱ�軭����������ϣ����һ���軭��������Ļ����ʾһ��ʱ��(��ʹ��һ����Ϣ�Ѿ����յ��������)
  //��ô�軭��Ҫһֱ��ʾ�Ļ������������һ����ʱֵ(0--255) X 10ms
  uchar gui_delay_value;
} GuiMsg;

typedef union tagMsgValue{
  KeyMsg key_message;
  SysMsg sys_message;
  ComMsg com_message;
  GuiMsg gui_message;
} MsgValue_u;

typedef struct  tagMsg{
    uchar       msg_type;         /*msg_type�ֱ��ʾCOM�ڡ����̡�GUI��ϵͳ��Ϣ��������Ϣ����ֵΪ1*/
    MsgValue_u  message;   
} Msg;
/***********************�������************************/
void MSG_init();
void MSG_sendMessage(Msg *message);
Msg MSG_getMessage();
void MSG_dealMessage(const Msg *message);

#endif