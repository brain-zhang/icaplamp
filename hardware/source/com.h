#ifndef __COM_H__
#define __COM_H__

//URAT端口初始化
void UART_init();

//从COM端口获取数据,一次数据大小不能超过total_size，存入到dest中
void COM_getComBuf(char *dest, uchar total_size);

//发送数据
void COM_sendByte(uchar data);

#endif