/*��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��*/
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ

#define COM_BUFFER_SIZE  128
#define COM_BUFFER_AREA  1

//CRC��
#define COM_CRC_SEG   1
//������־�ֶ�
#define COM_END_SEG   2
//��չ�ֶΣ�����CRCУ��ֵ��������־
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
//               MSP430���ڳ�ʼ��
//*************************************************************************
void UART_init()
{
  U0CTL |= SWRST;               //��λSWRST
  U0CTL |= CHAR;                //8λ����ģʽ 
  U0TCTL |= SSEL1;              //SMCLKΪ����ʱ��
  U0BR1 = baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0 = baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL = 0x00;                //΢���Ĵ���Ϊ0��������9600bps
  ME1 |= UTXE0;                 //UART1����ʹ��
  ME1 |= URXE0;                 //UART1����ʹ��
  
  U0CTL &= ~SWRST;
  IE1 |= URXIE0;                //�����ж�ʹ��λ
  
  P3SEL |= BIT4;               //����IO��Ϊ��ͨI/Oģʽ
  P3DIR |= BIT4;               //����IO�ڷ���Ϊ���
  P3SEL |= BIT5;
  
  P3DIR &= ~BIT0; 
  
  //���ж�
  _EINT();
  
  pbuffer = com_buffer;
}

//*************************************************************************
//              ����0�������ݺ���
//*************************************************************************

void COM_sendByte(uchar data)
{
  //���ͼĴ����յ�ʱ��������
    while((IFG1 & UTXIFG0) == 0);          
      U0TXBUF = data;
}

void COM_getComBuf(char *dest, uchar total_size)
{
     assert(total_size >= sizeof(com_buffer));
     
     //ȷ�ϻ����������ݣ������Ϊ�ж�ԭ���¶������ݣ�ֻȡ���һ��
     memcpy(dest, com_buffer, strlen((char *)com_buffer) - 1);
     //���㣬��λ
     memset(com_buffer, 0, COM_BUFFER_AREA * sizeof(com_buffer));
     pbuffer = com_buffer;
}

//*************************************************************************
//               �������Դ��� 0 �Ľ����ж�
//*************************************************************************
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
   //���յ������ݴ�����
  *pbuffer++ = U0RXBUF;                      
  if(((pbuffer - com_buffer) > 1) && (*(pbuffer - 1) == 0xFF) &&(*(pbuffer - 2) == 0xFF))
  {
    if(CRC_is_ok(com_buffer, strlen((char *)com_buffer) - COM_END_SEG))
    {
      //������Ϣ������������
      Msg message;
      message.msg_type = MESSAGETYPE_COM;
      FLASH_msg_en_queue((char *)com_buffer, strlen((char*)com_buffer) - COM_EXT_SEG);
      MSG_sendMessage(&message);
      
      //����λ�����ͻ�Ӧ��Ϣ
      COM_sendByte(SIG_RESPONSE_SEG);
      
      //LCD_displayCgrom("GET OK");
    }
    //��յ�ǰ������
    memset(com_buffer, 0, COM_BUFFER_AREA * sizeof(com_buffer));
    pbuffer = com_buffer;
  }
}

//*************************************************************************
//            �������Դ��� 0 �ķ����жϣ�Ԥ��
//*************************************************************************

#pragma vector=UART0TX_VECTOR
__interrupt void UART0_TX_ISR(void)
{
  
}
