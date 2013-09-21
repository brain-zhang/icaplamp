#include <msp430x14x.h>
#include <string.h>
#include "flash.h"
#include "lcd.h"
#include "hardware.h"
#include "icap_debug.h"

static void flash_write_ch_infoA(char ch, char *addr);
static void flash_record_clear_infoA();
static void flash_write_ini(ini_data_u ini_data);
static ini_data_u flash_read_ini();

//*************************************************************************
//  向infoA区中写入信息，单个字节写入
//*************************************************************************
static void flash_write_ch_infoA(char ch, char *addr)
{
  
  if((addr < (char *)INFOA_START_ADDR) || addr > (char *)INFOA_END_ADDR)
  {
    return;
  }
  
  FCTL1 = FWKEY + WRT;
  FCTL3 = FWKEY;
  
  _DINT();
  
  *addr = ch;
  
  _EINT();
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;
}

//*************************************************************************
//  从infoA区中读取配置信息, 返回当前存储配置信息空闲段的首地址
//*************************************************************************
static ini_data_u flash_read_ini()
{
  ini_data_u ini_data;
  int i = 0;
  char *elem = 0;
  for(i = 0; i < sizeof(ini_data); i++)
  {
    elem = (char *)(INFOA_START_ADDR + i);
    ini_data.elem[i] = *elem;
  }

  //最后返回存储配置信息空闲段的首地址
  return ini_data;
}

//*************************************************************************
//  向infoA区中写入配置信息, ptr代表当前存储配置信息空闲段的首地址
//*************************************************************************
static void flash_write_ini(ini_data_u ini_data)
{  
  int i = 0;
  
  //首先擦除原来的信息
  flash_record_clear_infoA();
  
  //写入信息
  for(i = 0; i < sizeof(ini_data); i++)
  {
    flash_write_ch_infoA(ini_data.elem[i], (char *)(INFOA_START_ADDR + i));
  }
}

//*************************************************************************
//                              写入flash到指定地址
//*************************************************************************
static char flash_record_ch(char ch, char *addr)
{
  if(addr > (char *)FLASH_INI_END_ADDR)
  {
    return 0;
  }
  
  FCTL1 = FWKEY + WRT;
  FCTL3 = FWKEY;
  
  _DINT();
  *addr = ch;
  while(FCTL3 & BUSY);
  _EINT();
  
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;
  return 1;
}

//*************************************************************************
//擦除数据flash，按段擦除，每个段512字节
//*************************************************************************
static void flash_record_clear(char *addr)
{
  if(addr > (char *)FLASH_INI_END_ADDR)
  {
    return;
  }

  FCTL1 = FWKEY + ERASE;
  FCTL3 = FWKEY;
  _DINT();
  
  *addr = 0;
  while(FCTL3 & BUSY);

  _EINT();
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;
}

//*************************************************************************
//擦除info区
//*************************************************************************
static void flash_record_clear_infoA()
{
  uchar *iinfo_ptr = (uchar *)INFOA_START_ADDR;
  FCTL1 = FWKEY  + ERASE;
  FCTL3 = FWKEY;
  
  _DINT();
  *iinfo_ptr = 0;
  while(FCTL3 & BUSY);
  _EINT();
  
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;
}

//===============================对外接口==================================

//*************************************************************************
//  从infoA区中设置信息，如果是初次启动，则初始化
//*************************************************************************
void FLASH_init_ini()
{
  ini_data_u ini_data = flash_read_ini();  
  
  //首先检测infoA开始地址处有没有初始化标志
  if(ini_data.ini_data.seg == INFOA_INIT_SEG)
  {
    //已经初始化过了，暂时什么都不用做
    //LCD_displayCgrom("已经初始化过了");
  }
  else
  {
    //初始化
    ini_data.ini_data.seg = INFOA_INIT_SEG;
    ini_data.ini_data.lcd_ptr = ini_data.ini_data.store_ptr = (char *)FLASH_INI_START_ADDR;
    ini_data.ini_data.count = 0;
    flash_write_ini(ini_data);
    //LCD_displayCgrom("写初始化");
  }
}

//*************************************************************************
//创建一个数据队列，当接收到一条新数据时，入队，直到队列满时，最早接收到的数据出队
//最多存储 FLASH_INI_BLCOK_SIZE(字节) X 10条数据
//入队函数
//*************************************************************************
void FLASH_msg_en_queue(const char *msg, uchar size)
{
  //首先从配置区读取当前队列指针
  ini_data_u ini_data = flash_read_ini();
  int i = 0;
  
  //然后擦除此段消息
  flash_record_clear(ini_data.ini_data.store_ptr);
  
  //然后写入内容
  for(i = 0; i < size; i++)
  {
    flash_record_ch(msg[i], ini_data.ini_data.store_ptr + i);
  }
  flash_record_ch(0, ini_data.ini_data.store_ptr + size);
  
   //有新消息入队时，lcd也要显示新接收的消息
  ini_data.ini_data.lcd_ptr = ini_data.ini_data.store_ptr; 
  
  //地址指针自增，存入配置区
  ini_data.ini_data.store_ptr += FLASH_INI_BLOCK_SIZE;
  if(ini_data.ini_data.store_ptr >= (char *)FLASH_INI_END_ADDR)
  {
    ini_data.ini_data.store_ptr = (char *)FLASH_INI_START_ADDR;
  }
  if(ini_data.ini_data.count < FLASH_INI_MSG_COUNT)
  {
    ini_data.ini_data.count++;
  }
  flash_write_ini(ini_data);
}

//*************************************************************************
//显示下一条记录
//*************************************************************************
void FLASH_lcd_on_next_record()
{
  ini_data_u ini_data = flash_read_ini();
  ini_data.ini_data.lcd_ptr += FLASH_INI_BLOCK_SIZE;
  if(ini_data.ini_data.lcd_ptr >= (char *)(FLASH_INI_START_ADDR + ini_data.ini_data.count * FLASH_INI_BLOCK_SIZE))
  {
    ini_data.ini_data.lcd_ptr = (char *)FLASH_INI_START_ADDR;
  }
  flash_write_ini(ini_data);
}

//*************************************************************************
//显示上一条记录
//*************************************************************************
void FLASH_lcd_on_pre_record()
{
  ini_data_u ini_data = flash_read_ini();
  ini_data.ini_data.lcd_ptr -= FLASH_INI_BLOCK_SIZE;
  if(ini_data.ini_data.lcd_ptr < (char *)FLASH_INI_START_ADDR)
  {
    ini_data.ini_data.lcd_ptr = (char *)(FLASH_INI_END_ADDR - (FLASH_INI_MSG_COUNT - ini_data.ini_data.count) * FLASH_INI_BLOCK_SIZE);
  }
  flash_write_ini(ini_data);
}

//*************************************************************************
//获取当前记录的地址
//*************************************************************************
char *FLASH_lcd_get_curr_record()
{
  ini_data_u ini_data = flash_read_ini();
  return ini_data.ini_data.lcd_ptr;
}


//*************************************************************************
//获取当前记录数目
//*************************************************************************
uchar FLASH_get_curr_record_count()
{
  ini_data_u ini_data = flash_read_ini();
  return ini_data.ini_data.count;
}

//*************************************************************************
//测试函数
//*************************************************************************
void FLASH_test()
{
  FLASH_init_ini();
  /*
  ini_data = flash_read_ini();
  if(ini_data.ini_data.store_ptr == (char *)FLASH_INI_START_ADDR)
  {
    DEBUG_PRINT("初次初始化");
    ini_data.ini_data.store_ptr = (char*)(FLASH_INI_START_ADDR + FLASH_INI_BLCOK_SIZE);
    ini_data.ini_data.lcd_ptr = (char *)FLASH_INI_START_ADDR;
    flash_write_ini(ini_data);
  }
  else 
  {
    DEBUG_PRINT("初始化完成");
  }
  */
  
  FLASH_lcd_on_next_record();
  FLASH_lcd_on_pre_record();
  FLASH_lcd_on_pre_record();
  FLASH_lcd_on_next_record();
  
  DEBUG_PRINT(FLASH_lcd_get_curr_record());
}