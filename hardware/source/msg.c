#include "hardware.h"
#include "msg.h"
#include "com.h"
#include "lcd.h"
#include "flash.h"
#include "key.h"
#include "menu.h"
#include "signal.h"
#include "spark.h"

#define MAX_MSG_COUNT 5
#define MESSAGE_RESPONSE_SEG SIG_RESPONSE_SEG

#define MESSAGE_WELCOME_STRING "��ȫ����ƽ���ؼ�"
#define MESSAGE_RESPONSE_STRING "�ѷ�����Ӧ��Ϣ"

/***********Ҫ��򵥣�һ����Ϣ���У������Ϣ�����������ƣ������ɵ���Ϣ****************/
static Msg message_queue[MAX_MSG_COUNT];
static uchar message_head = 0;
static uchar message_tail = 0;
static uchar message_is_full_tag = 0;

//*************************************************************************
//                        ��Ϣ���
//*************************************************************************
static void msg_en_queue(const Msg* message)
{
  message_queue[message_tail] = *message;
  if((message_head == message_tail) && message_is_full_tag)
  {
    //��ʱ�������������ɵ���Ϣ
    message_tail = (message_tail + 1) % MAX_MSG_COUNT;
    message_head = (message_head + 1) % MAX_MSG_COUNT;
  }
  else
  {
    //�������
    message_tail = (message_tail + 1) % MAX_MSG_COUNT;
    if(message_head == message_tail)
    {
       //��ʱ������
      message_is_full_tag = 1;
    }
  }
}

//*************************************************************************
//                        ��Ϣ����
//*************************************************************************
static Msg msg_de_queue()
{
  Msg r_message;
  if((message_head == message_tail) && !message_is_full_tag)
  {
    //��ʱ����Ϊ��
    r_message.msg_type = MESSAGETYPE_NONE;
  }
  else
  {
    r_message = message_queue[message_head];
    message_head = (message_head + 1) % MAX_MSG_COUNT;
    if(message_head == message_tail)
    {
      //��ʱ����Ϊ��
      message_is_full_tag = 0;
    }
  }
  return r_message;
}

//*************************************************************************
//                       �趨��ʾ��Ϣ��ʱʱ��
//*************************************************************************
static void msg_set_delay(Msg *message, uchar count)
{
  message->message.gui_message.gui_delay_value = count;
}

//=============================����ӿ�========================================

//*************************************************************************
//                        ��ʼ����Ϣģ��
//*************************************************************************
void MSG_init()
{
  //����һ���軭��Ϣ����ʾ�ػ�ǰ�������һ����¼
  //����ոճ�����ʼ��������ʾ "��ȫ����ƽ���ؼ�" ��ʾ��Ϣ
  Msg temp_message;
  temp_message.msg_type = MESSAGETYPE_GUI;  
  if(FLASH_get_curr_record_count() > 0)
  {
    temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
  }
  else
  {
    temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_WELCOME;
  }
  msg_set_delay(&temp_message, 0);
  MSG_sendMessage(&temp_message);
}

//*************************************************************************
//                        ������Ϣ
//*************************************************************************
void MSG_sendMessage(Msg *message)
{
    msg_en_queue(message);
}

//*************************************************************************
//                      ��ѭ���������������ݲ�������Ϣ
//*************************************************************************
Msg MSG_getMessage()
{
    //ɨ�谴��
    Msg key_message;
    uchar key = KEY_scan();
    if(key != KEY_VALUE_NULL)
    {
      //����а�������������Ϣ
      key_message.msg_type = MESSAGETYPE_KEY;
      key_message.message.key_message.key_value.ktat = key;
      msg_en_queue(&key_message);
    }
    
    return msg_de_queue();
}

//*************************************************************************
//                       ������Ϣ
//*************************************************************************
void MSG_dealMessage(const Msg *message)
{
  Msg temp_message = *message;

  switch(temp_message.msg_type)
  {
  case MESSAGETYPE_COM:
    //�����COM���ݽ�����Ϣ
    //���Բ����Զ�����Ϣ������ҳ�ż���Ϣ������������Ӧ������Ϣ�����ڿ�ʼʱ�궨�壬�����ʹ��
    //Ŀǰ�Ǵ���Ϣ��ȡ���������ݣ��ٷ���һ�������ػ����Ϣ
    temp_message.msg_type = MESSAGETYPE_GUI;  
    temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
    msg_set_delay(&temp_message, 0);    
    MSG_sendMessage(&temp_message);
    
    //���͵�ͷ��˸����Ϣ
    temp_message.msg_type = MESSAGETYPE_SYS_SPARK;  
    temp_message.message.sys_message.sys_value = SPARK_COUNT;
    temp_message.message.sys_message.sys_delay_value = SPARK_INTERVAL;
    MSG_sendMessage(&temp_message);
    
    break;
    
  case MESSAGETYPE_SYS_SPARK:
    //����ͷ����˸
    if(temp_message.message.sys_message.sys_delay_value-- >1)
    {
      delay_ms(10);

      //����Ϣԭ���Żض���
      MSG_sendMessage(&temp_message);  
    }
    else
    {
      if(temp_message.message.sys_message.sys_value-- > 1)
      {
        //��˸
        Spark_action();
        temp_message.message.sys_message.sys_delay_value = SPARK_INTERVAL;
        MSG_sendMessage(&temp_message);
      }
      else
      {
        //ֹͣ��˸
        Spark_stop();
      }
    }
    break;
    
  case MESSAGETYPE_GUI:
    //������ػ�������Ϣ
    //Ӧ�ôӴ洢������ȡ�������ػ���Ϣ�����ػ棬Ŀǰֻ�Ǽ򵥵��ٺ���׷��һ���ַ�
    
    //���delayʱ��
    if(temp_message.message.gui_message.gui_delay_value-- >1)
    {
      //��תһ��
      delay_ms(10);
      
      //����Ϣԭ���Żض���
      MSG_sendMessage(&temp_message);      
      break;
    }
    
    //�����������軭
    LCD_cls();
    
    switch(temp_message.message.gui_message.gui_type)
    {
    case MESSAGETYPE_GUI_SHOW_WELCOME:
      LCD_displayCgrom(MESSAGE_WELCOME_STRING);
      break;
    
    case MESSAGETYPE_GUI_SHOW_MESSAGE:
      //�����ǰ���ڲ˵�����
      if(Menu_showing())
      {
        Menu_exit();
      }
      //�����ǰflash������Ϣ
      if(FLASH_get_curr_record_count() > 0)
      {
        LCD_displayCgrom(FLASH_lcd_get_curr_record());
      }
      else
      {
        //���ͳ�ʼ��ʾ��Ϣ 
        temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_WELCOME;  
        msg_set_delay(&temp_message, 0);
        MSG_sendMessage(&temp_message);
      }
      break;
      
    case  MESSAGETYPE_GUI_SHOW_RESPONSE:
      LCD_displayCgrom(MESSAGE_RESPONSE_STRING);
      
      //������ʱʱ��
      msg_set_delay(&temp_message, 200);
      
      //������ͨ��Ϣ�軭��Ϣ  
      temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
      MSG_sendMessage(&temp_message);
      break;
      
    case  MESSAGETYPE_GUI_SHOW_MENU:
      Menu_show();
      break;      
    }
    break;
    
  case MESSAGETYPE_KEY:
    //�����е��鷳����Ҫ�Ǵ����в˵�������µ�ͼ�����
    switch(temp_message.message.key_message.key_value.ktat)
    {
    case KEY_UP:
      //�����ɿ���һ���Ƕ̰���ʾ��һ����Ϣ
      temp_message.msg_type = MESSAGETYPE_GUI; 
      if(Menu_showing())
      {
        Menu_next();        
        temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MENU;
      }
      else
      {
        FLASH_lcd_on_next_record();
        temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
      }
      msg_set_delay(&temp_message, 0);
      MSG_sendMessage(&temp_message);
      break;
      
    case KEY_LONG:
      //��������������/�˳��˵�����
      temp_message.msg_type = MESSAGETYPE_GUI;
      if(Menu_showing())
      {
        //��ǰ���ڲ˵���������
        if(Menu_is_exiting())
        {
          //������˳�����
          Menu_exit();
          temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
        }
        else
        {
          //���ͻ�Ӧ��Ϣ���˳��˵�����
          COM_sendByte(SIG_RESPONSE_SEG_MENU(Menu_get_index()));   
          Menu_exit();
          temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_RESPONSE;
        }
      }
      else
      {
        //����˵�������Ϣ
        Menu_entry();
        temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MENU;
      }
        msg_set_delay(&temp_message, 0);
        MSG_sendMessage(&temp_message);
      break;
        
    default:
      break;
    }
    break;
    
  case MESSAGETYPE_NONE:
    break;
    
  default:
    break;
  }
}