//���ļ��ӿ�ʵ����Ϣ�Ĵ洢����ʾ��ԭ������:
//��Ϣ��Ϊһ�����д洢��Flash�У���󳤶�Ϊ512(�ֽ�)X10(��)��512�ֽڶ�Ӧһ��flash�洢�Σ����Է����һ���Բ���һ��
//��Ϊ��Ϣ�洢��������Ϣ:
//1������Flash�Ƿ��ʼ����(��ʼ����ָÿ���豸��������ʱ��������һ����־��flash�У���ʹ��������Ҳ�����ٳ�ʼ��)
//2����Ϣ���е�ͷָ��(һֱָ����һ��������Ϣ�洢�ռ�)
//3��lcd��ʾ��Ϣ��ָ��(ָ����Ϣ�����е�ĳ����Ϣ������"��һ��"/"��һ��"�л�)
//��Щ������Ϣ���洢��INFOA����
//���FLASH��ַ�ռ�ʹ������:
//    |----------|FLASH_INI_END_ADDR                         �ߵ�ַ
//    |          |                                              |
//    | 512 X 10 | ��Ϊ��Ϣ�洢����ÿ����Ϣ512�ֽ�              |
//    |          |                                              |
//    |----------|FLASH_INI_START_ADDR                          |
//    |----------|                                              |
//    |----------|INFOA_END_ADDR                                |
//    |----------| ��Ϊ������Ϣ�洢��                           |
//    |----------|INFOA_START_ADDR                           �͵�ַ
//
//��Ϣ��ӵĻ���:�ȴ�������Ϣ�洢���ҵ���ǰ���еĿռ�ָ��(ָ����Ϣ�洢��)->д����Ϣ�洢��->ָ��ָ����һ����ַ�ռ�
//ͬʱlcdָ��ָ����յ�����Ϣ��ַ
//���յ�����Ϣ��������ʱ��ɾ����ɵ���Ϣʹ����ռ�


#ifndef __FLASH_H__
#define __FLASH_H__

#include "hardware.h"

//���ݴ洢����ʼ��ַ
#define FLASH_INI_START_ADDR 0xE000

//���ݴ洢��������ַ
#define FLASH_INI_END_ADDR 0xF400

//ÿ�����ݴ�С
#define FLASH_INI_BLOCK_SIZE 512

//�洢������Ŀ
#define FLASH_INI_MSG_COUNT 10

//INFO����ʼ��ַ
#define INFOA_START_ADDR 0x1080

//INFO��������ַ
#define INFOA_END_ADDR 0x10FF

//�����������־�����INFO����ͷ����˱�ʶ��˵���Ѿ���ʼ����
#define INFOA_INIT_SEG 0x86

//������Ϣ���ݽṹ
typedef struct tag_ini_data_t{
  //�Ƿ��ʼ����־
  uint seg;
  
  //��¼���д洢�ռ�ָ��
  char *store_ptr;
  
  //��¼LCD��ʾ����ָ��
  char *lcd_ptr;
  
  //��¼��ǰ���ж���������
  uint count;
}ini_data_t;

//������Ϣ���ݽṹ��������������д��flashʱ�������
typedef union tag_ini_data_u{
  ini_data_t ini_data;
  
  //��Ӧini_data_t�Ĵ�С
  uchar elem[6];
}ini_data_u;

//��ʼ��
void FLASH_init_ini();

//lcd��ʾ��һ������ǰ������ô˺�������ָ֤��ָ��
void FLASH_lcd_on_next_record();

//lcd��ʾ��һ������ǰ������ô˺�������ָ֤��ָ��
void FLASH_lcd_on_pre_record();

//��ȡ��ǰ��¼�ĵ�ַ
char *FLASH_lcd_get_curr_record();

//�������
void FLASH_msg_en_queue(const char *msg, uchar size);

//��ȡ��ǰ��¼��Ŀ
uchar FLASH_get_curr_record_count();

//���Ժ���
void FLASH_test();

#endif