#ifndef __COM_H__
#define __COM_H__

//URAT�˿ڳ�ʼ��
void UART_init();

//��COM�˿ڻ�ȡ����,һ�����ݴ�С���ܳ���total_size�����뵽dest��
void COM_getComBuf(char *dest, uchar total_size);

//��������
void COM_sendByte(uchar data);

#endif