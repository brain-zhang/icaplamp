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
  //初始化时钟、端口、看门狗
  WDT_init(); 
  Clock_init(); 
  Port_init();
  
  //键盘控制初始化
  KEY_init();
  
  //初始化URAT控制口
  UART_init();
  
  //初始化Flash存储
  FLASH_init_ini();
  
  //wifi连接初始化
  WIFI_init();
  
  void Spark_init();
}

void InitGraphic()
{
  //初始化图形环境
  LCD_initial();
  
  //清屏
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