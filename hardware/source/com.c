/*当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数*/
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位

#define COM_BUFFER_SIZE  128
#define COM_BUFFER_AREA  1

//CRC域
#define COM_CRC_SEG   1
//结束标志字段
#define COM_END_SEG   2
//扩展字段，包括CRC校验值，结束标志
#define COM_EXT_SEG   (COM_END_SEG + COM_CRC_SEG)

#include <string.h>
#include <assert.h>
#include "hardware.h"
#include "msg.h"
#include "lcd.h"
#include "flash.h"
#include "crc.h"
#include "signal.h"

static uchar com_buffer[COM_BUFFER_SIZE];
static uchar *pbuffer;

//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_init()
{
  U0CTL |= SWRST;               //复位SWRST
  U0CTL |= CHAR;                //8位数据模式 
  U0TCTL |= SSEL1;              //SMCLK为串口时钟
  U0BR1 = baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0 = baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL = 0x00;                //微调寄存器为0，波特率9600bps
  ME1 |= UTXE0;                 //UART1发送使能
  ME1 |= URXE0;                 //UART1接收使能
  
  U0CTL &= ~SWRST;
  IE1 |= URXIE0;                //接收中断使能位
  
  P3SEL |= BIT4;               //设置IO口为普通I/O模式
  P3DIR |= BIT4;               //设置IO口方向为输出
  P3SEL |= BIT5;
  
  P3DIR &= ~BIT0; 
  
  //开中断
  _EINT();
  
  pbuffer = com_buffer;
}

//*************************************************************************
//              串口0发送数据函数
//*************************************************************************

void COM_sendByte(uchar data)
{
  //发送寄存器空的时候发送数据
    while((IFG1 & UTXIFG0) == 0);          
      U0TXBUF = data;
}

void COM_getComBuf(char *dest, uchar total_size)
{
     assert(total_size >= sizeof(com_buffer));
     
     //确认缓冲区的数据，如果因为中断原因导致多条数据，只取最后一条
     memcpy(dest, com_buffer, strlen((char *)com_buffer) - 1);
     //清零，复位
     memset(com_buffer, 0, COM_BUFFER_AREA * sizeof(com_buffer));
     pbuffer = com_buffer;
}

//*************************************************************************
//               处理来自串口 0 的接收中断
//*************************************************************************
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
   //接收到的数据存起来
  *pbuffer++ = U0RXBUF;                      
  if(((pbuffer - com_buffer) > 1) && (*(pbuffer - 1) == 0xFF) &&(*(pbuffer - 2) == 0xFF))
  {
    if(CRC_is_ok(com_buffer, strlen((char *)com_buffer) - COM_END_SEG))
    {
      //发送消息，串口数据满
      Msg message;
      message.msg_type = MESSAGETYPE_COM;
      FLASH_msg_en_queue((char *)com_buffer, strlen((char*)com_buffer) - COM_EXT_SEG);
      MSG_sendMessage(&message);
      
      //向上位机发送回应信息
      COM_sendByte(SIG_RESPONSE_SEG);
      
      //LCD_displayCgrom("GET OK");
    }
    //清空当前缓冲区
    memset(com_buffer, 0, COM_BUFFER_AREA * sizeof(com_buffer));
    pbuffer = com_buffer;
  }
}

//*************************************************************************
//            处理来自串口 0 的发送中断，预留
//*************************************************************************

#pragma vector=UART0TX_VECTOR
__interrupt void UART0_TX_ISR(void)
{
  
}
