#include "hardware.h"
#include "key.h"
#include "msg.h"
#include "lcd.h"
#include "icap_debug.h"

//定义长按键的TICK数,以及连_发间隔的TICK数 
#define KEY_LONG_PERIOD         (20000)     //2秒钟为长按 32768 * 2
#define KEY_CONTINUE_PERIOD     25 

//定义按键状态 
#define KEY_STATE_INIT           0 
#define KEY_STATE_WOBBLE         1 
#define KEY_STATE_PRESS          2 
#define KEY_STATE_LONG           3 
#define KEY_STATE_LONG_LONG      4
#define KEY_STATE_CONTINUE       5 
#define KEY_STATE_RELEASE        6 

//独立键盘接在P10
#define KEY_PORT         (P1IN & 0x01)          

//**********************************************************************
//      键盘扫描子程序
//**********************************************************************
static uchar key_scan()
{
  uchar key = KEY_VALUE_NULL;
  if(KEY_PORT == 0x00)   
  {
    key = KEY_VALUE_1;
  }
  return key;
}

//**********************************************************************
//      获取键盘动作子程序
//**********************************************************************
static uchar key_get_key()
{
  static uchar key_state  = KEY_STATE_INIT;
  static uint key_time_count = 0;
  static uchar last_key = KEY_VALUE_NULL;
  static uchar last_key_stat = KEY_STATE_INIT;
  
  uchar temp_key = KEY_VALUE_NULL;
  
  temp_key = key_scan();
  
  switch(key_state)
  {
  case KEY_STATE_INIT:
    if(KEY_VALUE_NULL != temp_key)
    {
      key_state = KEY_STATE_WOBBLE;
      last_key_stat = KEY_STATE_INIT;
    }
    break;
  
  case KEY_STATE_WOBBLE:  
    //消抖
    key_state = KEY_STATE_PRESS;
    last_key_stat = KEY_STATE_WOBBLE;
    break;
    
  case KEY_STATE_PRESS:
    if(KEY_VALUE_NULL != temp_key)
    {
      //保存键值，以便在释放按键状态返回键值
      last_key = temp_key;  
      
      //此时按键按下
      temp_key = KEY_DOWN;
      key_state = KEY_STATE_LONG;
    }
    else
    {
      key_state = KEY_STATE_INIT;
    }
    last_key_stat = KEY_STATE_PRESS;
    break;
    
  case KEY_STATE_LONG:
    if(KEY_VALUE_NULL != temp_key)
    {
      if(++key_time_count > KEY_LONG_PERIOD)
      {
        key_time_count = 0;
        
        //长按键
        temp_key = KEY_LONG;
        key_state = KEY_STATE_LONG;
        last_key_stat = KEY_STATE_LONG_LONG;
      }
    }
    else
    {
      key_state = KEY_STATE_RELEASE;
    }
    break;
  /*
    //取消连按设置
  case KEY_STATE_CONTINUE:
    if(KEY_VALUE_NULL != temp_key)
    {
      if(++key_time_count > KEY_CONTINUE_PERIOD)
      {
        key_time_count = 0;
        temp_key = KEY_CONTINUE;
      }
      else
      {
        key_state = KEY_STATE_RELEASE;
      }
    }
    break;
    */
  case KEY_STATE_RELEASE:
    if(last_key_stat == KEY_STATE_LONG_LONG)
    {
      last_key = KEY_UP_LONG;
    }
    else
    {
      last_key = KEY_UP;
    }
    temp_key = last_key;
    key_state = KEY_STATE_INIT;
    last_key_stat = KEY_STATE_RELEASE;
    break;
    
  default:
    break;
  }
  return temp_key;
}

//**********************************************************************
//	键盘IO端初始化
//**********************************************************************
void KEY_init()
{
  //按键处理
  P1SEL = 0x00;                   //P1普通IO功能
  P1DIR = 0xFE;                   //P10~P13输入模式，外部电路已接上拉电阻
}

//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************
uchar KEY_scan() 
{
  return key_get_key();
}

//**********************************************************************
//测试程序
//**********************************************************************
void KEY_test()
{
  uchar key = KEY_VALUE_NULL;  
  while(1)
  {
    key = key_get_key();
    if(KEY_DOWN == key)
    {
      LCD_cls();
      DEBUG_PRINT("按下");
      continue;
    }
    
    if(KEY_LONG == key)
    {
      LCD_cls();
      DEBUG_PRINT("长按");
      continue;
    }

    if(KEY_UP == key)
    {
      LCD_cls();
      DEBUG_PRINT("松开");
      continue;
    }
    /*
    key = key_scan();
    if(key != KEY_VALUE_NULL)
    {
      DEBUG_PRINT("OK");
    }
    else
    {
      LCD_cls();
    }
    */
  }
}