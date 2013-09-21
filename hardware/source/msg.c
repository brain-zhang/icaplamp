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

#define MESSAGE_WELCOME_STRING "安全生产平安回家"
#define MESSAGE_RESPONSE_STRING "已发出回应信息"

/***********要求简单，一个消息队列，如果消息个数超出限制，冲掉最旧的消息****************/
static Msg message_queue[MAX_MSG_COUNT];
static uchar message_head = 0;
static uchar message_tail = 0;
static uchar message_is_full_tag = 0;

//*************************************************************************
//                        消息入队
//*************************************************************************
static void msg_en_queue(const Msg* message)
{
  message_queue[message_tail] = *message;
  if((message_head == message_tail) && message_is_full_tag)
  {
    //此时队列满，冲掉最旧的消息
    message_tail = (message_tail + 1) % MAX_MSG_COUNT;
    message_head = (message_head + 1) % MAX_MSG_COUNT;
  }
  else
  {
    //正常情况
    message_tail = (message_tail + 1) % MAX_MSG_COUNT;
    if(message_head == message_tail)
    {
       //此时队列满
      message_is_full_tag = 1;
    }
  }
}

//*************************************************************************
//                        消息出队
//*************************************************************************
static Msg msg_de_queue()
{
  Msg r_message;
  if((message_head == message_tail) && !message_is_full_tag)
  {
    //此时队列为空
    r_message.msg_type = MESSAGETYPE_NONE;
  }
  else
  {
    r_message = message_queue[message_head];
    message_head = (message_head + 1) % MAX_MSG_COUNT;
    if(message_head == message_tail)
    {
      //此时队列为空
      message_is_full_tag = 0;
    }
  }
  return r_message;
}

//*************************************************************************
//                       设定显示消息延时时间
//*************************************************************************
static void msg_set_delay(Msg *message, uchar count)
{
  message->message.gui_message.gui_delay_value = count;
}

//=============================对外接口========================================

//*************************************************************************
//                        初始化消息模块
//*************************************************************************
void MSG_init()
{
  //发送一条描画消息，显示关机前看的最后一条记录
  //如果刚刚出厂初始化，则显示 "安全生产平安回家" 提示信息
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
//                        发送消息
//*************************************************************************
void MSG_sendMessage(Msg *message)
{
    msg_en_queue(message);
}

//*************************************************************************
//                      主循环函数，接受数据并传送消息
//*************************************************************************
Msg MSG_getMessage()
{
    //扫描按键
    Msg key_message;
    uchar key = KEY_scan();
    if(key != KEY_VALUE_NULL)
    {
      //如果有按键处理，发送消息
      key_message.msg_type = MESSAGETYPE_KEY;
      key_message.message.key_message.key_value.ktat = key;
      msg_en_queue(&key_message);
    }
    
    return msg_de_queue();
}

//*************************************************************************
//                       处理消息
//*************************************************************************
void MSG_dealMessage(const Msg *message)
{
  Msg temp_message = *message;

  switch(temp_message.msg_type)
  {
  case MESSAGETYPE_COM:
    //如果有COM数据接收消息
    //可以插入自定义消息，根据页号及消息含义来进行相应处理，消息号请在开始时宏定义，请灵活使用
    //目前是从消息中取出串口数据，再发送一条界面重绘的消息
    temp_message.msg_type = MESSAGETYPE_GUI;  
    temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
    msg_set_delay(&temp_message, 0);    
    MSG_sendMessage(&temp_message);
    
    //发送灯头闪烁的消息
    temp_message.msg_type = MESSAGETYPE_SYS_SPARK;  
    temp_message.message.sys_message.sys_value = SPARK_COUNT;
    temp_message.message.sys_message.sys_delay_value = SPARK_INTERVAL;
    MSG_sendMessage(&temp_message);
    
    break;
    
  case MESSAGETYPE_SYS_SPARK:
    //处理头灯闪烁
    if(temp_message.message.sys_message.sys_delay_value-- >1)
    {
      delay_ms(10);

      //将消息原样放回队列
      MSG_sendMessage(&temp_message);  
    }
    else
    {
      if(temp_message.message.sys_message.sys_value-- > 1)
      {
        //闪烁
        Spark_action();
        temp_message.message.sys_message.sys_delay_value = SPARK_INTERVAL;
        MSG_sendMessage(&temp_message);
      }
      else
      {
        //停止闪烁
        Spark_stop();
      }
    }
    break;
    
  case MESSAGETYPE_GUI:
    //如果是重绘界面的信息
    //应该从存储区域中取出所有重绘信息进行重绘，目前只是简单的再后面追加一个字符
    
    //检查delay时间
    if(temp_message.message.gui_message.gui_delay_value-- >1)
    {
      //空转一下
      delay_ms(10);
      
      //将消息原样放回队列
      MSG_sendMessage(&temp_message);      
      break;
    }
    
    //先清屏，再描画
    LCD_cls();
    
    switch(temp_message.message.gui_message.gui_type)
    {
    case MESSAGETYPE_GUI_SHOW_WELCOME:
      LCD_displayCgrom(MESSAGE_WELCOME_STRING);
      break;
    
    case MESSAGETYPE_GUI_SHOW_MESSAGE:
      //如果当前是在菜单界面
      if(Menu_showing())
      {
        Menu_exit();
      }
      //如果当前flash中有消息
      if(FLASH_get_curr_record_count() > 0)
      {
        LCD_displayCgrom(FLASH_lcd_get_curr_record());
      }
      else
      {
        //发送初始提示信息 
        temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_WELCOME;  
        msg_set_delay(&temp_message, 0);
        MSG_sendMessage(&temp_message);
      }
      break;
      
    case  MESSAGETYPE_GUI_SHOW_RESPONSE:
      LCD_displayCgrom(MESSAGE_RESPONSE_STRING);
      
      //设置延时时间
      msg_set_delay(&temp_message, 200);
      
      //发送普通信息描画消息  
      temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
      MSG_sendMessage(&temp_message);
      break;
      
    case  MESSAGETYPE_GUI_SHOW_MENU:
      Menu_show();
      break;      
    }
    break;
    
  case MESSAGETYPE_KEY:
    //这里有点麻烦，主要是处理有菜单的情况下的图形描绘
    switch(temp_message.message.key_message.key_value.ktat)
    {
    case KEY_UP:
      //按键松开，一般是短按显示下一条消息
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
      //按键长按，进入/退出菜单环境
      temp_message.msg_type = MESSAGETYPE_GUI;
      if(Menu_showing())
      {
        //当前正在菜单命令项中
        if(Menu_is_exiting())
        {
          //如果是退出动作
          Menu_exit();
          temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_MESSAGE;
        }
        else
        {
          //发送回应信息，退出菜单环境
          COM_sendByte(SIG_RESPONSE_SEG_MENU(Menu_get_index()));   
          Menu_exit();
          temp_message.message.gui_message.gui_type = MESSAGETYPE_GUI_SHOW_RESPONSE;
        }
      }
      else
      {
        //进入菜单环境信息
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