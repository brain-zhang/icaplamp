//������һ���ֺ�ֵ������ͬ��λ����ͨѶ
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

//Ĭ���յ���Ϣ�󷵻ش��ź�
#define SIG_RESPONSE_SEG 0x10

//��Ӧ�˵�������˵���1������SIG_RESPONSE_SEG(1)����������....
#define SIG_RESPONSE_SEG_MENU(index) (SIG_RESPONSE_SEG + MENU_MESSAGE_BASE_SEG + index)

#endif