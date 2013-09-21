#ifndef _KEY_H_
#define _KEY_H_

#define KEY_VALUE_1       0x01                   //����
#define KEY_VALUE_NULL    0xFF                   //�ް���

//���尴������ֵ״̬(����,����,����,�ͷ�) 
#define KEY_DOWN                 0x80 
#define KEY_LONG                 0x40 
#define KEY_CONTINUE             0x20 
#define KEY_UP                   0x10 

//���״̬�е����⣬�������������ɿ�
#define KEY_UP_LONG              0x11

void KEY_init();
uchar KEY_scan();
void KEY_test();

#endif