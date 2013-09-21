//本文件接口实现发送回应信息时的处理
//当工人长按发送键时，会出现一个常用发送短语菜单，工人通过短按键选择发送的短语
//再长按键发送出去，之后系统提示发送成功，回到信息显示界面
//信息定义，每条菜单信息都是一个独立的显示单元

#include <msp430x14x.h>
#include "hardware.h"
#include "lcd.h"

//每条菜单信息的最大长度
#define MAX_MENU_MESS_SIZE 64
#define MENU_MESS_COUNT (sizeof(menus) / MAX_MENU_MESS_SIZE)

//发送消息候选
static char menus[][MAX_MENU_MESS_SIZE] = {
  "1>已经收到",
  "2>正忙",
  "3>正在上井中",
  "4>正在下井中",
  "5>立即处理",
  "6>退出"
};

//当前是否进入菜单状态
static uchar is_menu_state = 0;

//当前消息游标
static uchar current_menu_no = 0;

//显示菜单
void Menu_show()
{
  LCD_displayCgrom(menus[current_menu_no]);
}

//下一个菜单项
void Menu_next()
{
  current_menu_no = (current_menu_no + 1) % MENU_MESS_COUNT;
}

//进入菜单
void Menu_entry()
{
  is_menu_state = 1;
}

//退出菜单
void Menu_exit()
{
  current_menu_no = 0;
  is_menu_state = 0;
}

//是否要退出菜单
uchar Menu_is_exiting()
{
  return current_menu_no == MENU_MESS_COUNT - 1;
}

//显示当前菜单项
uchar Menu_showing()
{
  return is_menu_state;
}

//返回当前菜单项编号，从1开始计数
uchar Menu_get_index()
{
  return current_menu_no + 1;
}

//测试
void Menu_test()
{
  Menu_show();
}