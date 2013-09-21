//本文件接口实现消息的存储及显示，原理如下:
//消息作为一个队列存储在Flash中，最大长度为512(字节)X10(条)，512字节对应一个flash存储段，可以方便的一次性擦除一段
//作为消息存储的配置信息:
//1、包括Flash是否初始化过(初始化是指每个设备初次启动时，会留有一个标志在flash中，即使掉电重启也不必再初始化)
//2、消息队列的头指针(一直指向下一个空闲消息存储空间)
//3、lcd显示消息的指针(指向消息队列中的某个消息，可以"上一条"/"下一条"切换)
//这些配置信息都存储在INFOA段中
//如此FLASH地址空间使用如下:
//    |----------|FLASH_INI_END_ADDR                         高地址
//    |          |                                              |
//    | 512 X 10 | 此为消息存储区，每条消息512字节              |
//    |          |                                              |
//    |----------|FLASH_INI_START_ADDR                          |
//    |----------|                                              |
//    |----------|INFOA_END_ADDR                                |
//    |----------| 此为配置信息存储区                           |
//    |----------|INFOA_START_ADDR                           低地址
//
//消息入队的机制:先从配置信息存储区找到当前空闲的空间指针(指向消息存储区)->写入消息存储区->指针指向下一个地址空间
//同时lcd指针指向刚收到的消息地址
//当收到的消息超过限制时，删除最旧的消息使用其空间


#ifndef __FLASH_H__
#define __FLASH_H__

#include "hardware.h"

//数据存储区起始地址
#define FLASH_INI_START_ADDR 0xE000

//数据存储区结束地址
#define FLASH_INI_END_ADDR 0xF400

//每段数据大小
#define FLASH_INI_BLOCK_SIZE 512

//存储数据数目
#define FLASH_INI_MSG_COUNT 10

//INFO区起始地址
#define INFOA_START_ADDR 0x1080

//INFO区结束地址
#define INFOA_END_ADDR 0x10FF

//数据区保存标志，如果INFO区开头有如此标识，说明已经初始化过
#define INFOA_INIT_SEG 0x86

//配置信息数据结构
typedef struct tag_ini_data_t{
  //是否初始化标志
  uint seg;
  
  //记录空闲存储空间指针
  char *store_ptr;
  
  //记录LCD显示内容指针
  char *lcd_ptr;
  
  //记录当前存有多少条数据
  uint count;
}ini_data_t;

//配置信息数据结构，次联合体用于写入flash时操作简便
typedef union tag_ini_data_u{
  ini_data_t ini_data;
  
  //对应ini_data_t的大小
  uchar elem[6];
}ini_data_u;

//初始化
void FLASH_init_ini();

//lcd显示下一条数据前必须调用此函数，保证指针指向
void FLASH_lcd_on_next_record();

//lcd显示上一条数据前必须调用此函数，保证指针指向
void FLASH_lcd_on_pre_record();

//获取当前记录的地址
char *FLASH_lcd_get_curr_record();

//数据入队
void FLASH_msg_en_queue(const char *msg, uchar size);

//获取当前记录数目
uchar FLASH_get_curr_record_count();

//测试函数
void FLASH_test();

#endif