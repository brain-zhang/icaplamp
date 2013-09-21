//���ļ��ӿ�ʵ�ַ��ͻ�Ӧ��Ϣʱ�Ĵ���
//�����˳������ͼ�ʱ�������һ�����÷��Ͷ���˵�������ͨ���̰���ѡ���͵Ķ���
//�ٳ��������ͳ�ȥ��֮��ϵͳ��ʾ���ͳɹ����ص���Ϣ��ʾ����
//��Ϣ���壬ÿ���˵���Ϣ����һ����������ʾ��Ԫ

#include <msp430x14x.h>
#include "hardware.h"
#include "lcd.h"

//ÿ���˵���Ϣ����󳤶�
#define MAX_MENU_MESS_SIZE 64
#define MENU_MESS_COUNT (sizeof(menus) / MAX_MENU_MESS_SIZE)

//������Ϣ��ѡ
static char menus[][MAX_MENU_MESS_SIZE] = {
  "1>�Ѿ��յ�",
  "2>��æ",
  "3>�����Ͼ���",
  "4>�����¾���",
  "5>��������",
  "6>�˳�"
};

//��ǰ�Ƿ����˵�״̬
static uchar is_menu_state = 0;

//��ǰ��Ϣ�α�
static uchar current_menu_no = 0;

//��ʾ�˵�
void Menu_show()
{
  LCD_displayCgrom(menus[current_menu_no]);
}

//��һ���˵���
void Menu_next()
{
  current_menu_no = (current_menu_no + 1) % MENU_MESS_COUNT;
}

//����˵�
void Menu_entry()
{
  is_menu_state = 1;
}

//�˳��˵�
void Menu_exit()
{
  current_menu_no = 0;
  is_menu_state = 0;
}

//�Ƿ�Ҫ�˳��˵�
uchar Menu_is_exiting()
{
  return current_menu_no == MENU_MESS_COUNT - 1;
}

//��ʾ��ǰ�˵���
uchar Menu_showing()
{
  return is_menu_state;
}

//���ص�ǰ�˵����ţ���1��ʼ����
uchar Menu_get_index()
{
  return current_menu_no + 1;
}

//����
void Menu_test()
{
  Menu_show();
}