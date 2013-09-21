#include "hardware.h"
#include "key.h"
#include "msg.h"
#include "lcd.h"
#include "icap_debug.h"

//���峤������TICK��,�Լ���_�������TICK�� 
#define KEY_LONG_PERIOD         (20000)     //2����Ϊ���� 32768 * 2
#define KEY_CONTINUE_PERIOD     25 

//���尴��״̬ 
#define KEY_STATE_INIT           0 
#define KEY_STATE_WOBBLE         1 
#define KEY_STATE_PRESS          2 
#define KEY_STATE_LONG           3 
#define KEY_STATE_LONG_LONG      4
#define KEY_STATE_CONTINUE       5 
#define KEY_STATE_RELEASE        6 

//�������̽���P10
#define KEY_PORT         (P1IN & 0x01)          

//**********************************************************************
//      ����ɨ���ӳ���
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
//      ��ȡ���̶����ӳ���
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
    //����
    key_state = KEY_STATE_PRESS;
    last_key_stat = KEY_STATE_WOBBLE;
    break;
    
  case KEY_STATE_PRESS:
    if(KEY_VALUE_NULL != temp_key)
    {
      //�����ֵ���Ա����ͷŰ���״̬���ؼ�ֵ
      last_key = temp_key;  
      
      //��ʱ��������
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
        
        //������
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
    //ȡ����������
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
//	����IO�˳�ʼ��
//**********************************************************************
void KEY_init()
{
  //��������
  P1SEL = 0x00;                   //P1��ͨIO����
  P1DIR = 0xFE;                   //P10~P13����ģʽ���ⲿ��·�ѽ���������
}

//**********************************************************************
//	����ɨ���ӳ��򣬲������ɨ��ķ�ʽ
//**********************************************************************
uchar KEY_scan() 
{
  return key_get_key();
}

//**********************************************************************
//���Գ���
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
      DEBUG_PRINT("����");
      continue;
    }
    
    if(KEY_LONG == key)
    {
      LCD_cls();
      DEBUG_PRINT("����");
      continue;
    }

    if(KEY_UP == key)
    {
      LCD_cls();
      DEBUG_PRINT("�ɿ�");
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