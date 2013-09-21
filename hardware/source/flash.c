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
//  ��infoA����д����Ϣ�������ֽ�д��
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
//  ��infoA���ж�ȡ������Ϣ, ���ص�ǰ�洢������Ϣ���жε��׵�ַ
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

  //��󷵻ش洢������Ϣ���жε��׵�ַ
  return ini_data;
}

//*************************************************************************
//  ��infoA����д��������Ϣ, ptr����ǰ�洢������Ϣ���жε��׵�ַ
//*************************************************************************
static void flash_write_ini(ini_data_u ini_data)
{  
  int i = 0;
  
  //���Ȳ���ԭ������Ϣ
  flash_record_clear_infoA();
  
  //д����Ϣ
  for(i = 0; i < sizeof(ini_data); i++)
  {
    flash_write_ch_infoA(ini_data.elem[i], (char *)(INFOA_START_ADDR + i));
  }
}

//*************************************************************************
//                              д��flash��ָ����ַ
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
//��������flash�����β�����ÿ����512�ֽ�
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
//����info��
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

//===============================����ӿ�==================================

//*************************************************************************
//  ��infoA����������Ϣ������ǳ������������ʼ��
//*************************************************************************
void FLASH_init_ini()
{
  ini_data_u ini_data = flash_read_ini();  
  
  //���ȼ��infoA��ʼ��ַ����û�г�ʼ����־
  if(ini_data.ini_data.seg == INFOA_INIT_SEG)
  {
    //�Ѿ���ʼ�����ˣ���ʱʲô��������
    //LCD_displayCgrom("�Ѿ���ʼ������");
  }
  else
  {
    //��ʼ��
    ini_data.ini_data.seg = INFOA_INIT_SEG;
    ini_data.ini_data.lcd_ptr = ini_data.ini_data.store_ptr = (char *)FLASH_INI_START_ADDR;
    ini_data.ini_data.count = 0;
    flash_write_ini(ini_data);
    //LCD_displayCgrom("д��ʼ��");
  }
}

//*************************************************************************
//����һ�����ݶ��У������յ�һ��������ʱ����ӣ�ֱ��������ʱ��������յ������ݳ���
//���洢 FLASH_INI_BLCOK_SIZE(�ֽ�) X 10������
//��Ӻ���
//*************************************************************************
void FLASH_msg_en_queue(const char *msg, uchar size)
{
  //���ȴ���������ȡ��ǰ����ָ��
  ini_data_u ini_data = flash_read_ini();
  int i = 0;
  
  //Ȼ������˶���Ϣ
  flash_record_clear(ini_data.ini_data.store_ptr);
  
  //Ȼ��д������
  for(i = 0; i < size; i++)
  {
    flash_record_ch(msg[i], ini_data.ini_data.store_ptr + i);
  }
  flash_record_ch(0, ini_data.ini_data.store_ptr + size);
  
   //������Ϣ���ʱ��lcdҲҪ��ʾ�½��յ���Ϣ
  ini_data.ini_data.lcd_ptr = ini_data.ini_data.store_ptr; 
  
  //��ַָ������������������
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
//��ʾ��һ����¼
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
//��ʾ��һ����¼
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
//��ȡ��ǰ��¼�ĵ�ַ
//*************************************************************************
char *FLASH_lcd_get_curr_record()
{
  ini_data_u ini_data = flash_read_ini();
  return ini_data.ini_data.lcd_ptr;
}


//*************************************************************************
//��ȡ��ǰ��¼��Ŀ
//*************************************************************************
uchar FLASH_get_curr_record_count()
{
  ini_data_u ini_data = flash_read_ini();
  return ini_data.ini_data.count;
}

//*************************************************************************
//���Ժ���
//*************************************************************************
void FLASH_test()
{
  FLASH_init_ini();
  /*
  ini_data = flash_read_ini();
  if(ini_data.ini_data.store_ptr == (char *)FLASH_INI_START_ADDR)
  {
    DEBUG_PRINT("���γ�ʼ��");
    ini_data.ini_data.store_ptr = (char*)(FLASH_INI_START_ADDR + FLASH_INI_BLCOK_SIZE);
    ini_data.ini_data.lcd_ptr = (char *)FLASH_INI_START_ADDR;
    flash_write_ini(ini_data);
  }
  else 
  {
    DEBUG_PRINT("��ʼ�����");
  }
  */
  
  FLASH_lcd_on_next_record();
  FLASH_lcd_on_pre_record();
  FLASH_lcd_on_pre_record();
  FLASH_lcd_on_next_record();
  
  DEBUG_PRINT(FLASH_lcd_get_curr_record());
}