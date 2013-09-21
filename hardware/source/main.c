#include "hardware.h"
#include "lcd.h"
#include "com.h"
#include "msg.h"
#include "flash.h"
#include "key.h"
#include "wifi.h"

void OnError()
{
  LCD_debug_displayCgrom("Error,Not enough memory");
}

void Test()
{
  //FLASH_test();
  //LCD_test();
  //WIFI_test();
  KEY_test();
}

void InitSysEnv()
{
  //��ʼ��ʱ�ӡ��˿ڡ����Ź�
  WDT_init(); 
  Clock_init(); 
  Port_init();
  
  //���̿��Ƴ�ʼ��
  KEY_init();
  
  //��ʼ��URAT���ƿ�
  UART_init();
  
  //��ʼ��Flash�洢
  FLASH_init_ini();
  
  //wifi���ӳ�ʼ��
  WIFI_init();
  
  void Spark_init();
}

void InitGraphic()
{
  //��ʼ��ͼ�λ���
  LCD_initial();
  
  //����
  LCD_cls();
}

void main(void)
{
  InitSysEnv();
  
  InitGraphic();
  
  MSG_init();
  
  //Test();
  
  Msg message;
  
  while(1)
  {
    message = MSG_getMessage();
    MSG_dealMessage(&message);
  }
}