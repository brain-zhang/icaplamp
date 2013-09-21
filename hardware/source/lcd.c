/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		    //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		    //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		    //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		    //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0c		    //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		    //��ʾ��
#define CURSE_DIR	0x14		    //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		    //����AC����ΧΪ��00H~3FH
#define SET_DD_AC	0x80

#define P50 0
#define P51 1
#define P55 5
#define P56 6
#define P57 7

#define RS_CLR	P5OUT &= ~(1 << P55)        //RS�õ�
#define RS_SET	P5OUT |= (1 << P55)         //RS�ø�

#define RW_CLR	P5OUT &= ~(1 << P56)        //RW�õ�
#define RW_SET	P5OUT |= (1 << P56)         //RW�ø�

#define EN_CLR	P5OUT &= ~(1 << P57)        //E�õ�
#define EN_SET	P5OUT |= (1 << P57)         //E�ø�

#define PSB_CLR	P5OUT &= ~(1 << P50)        //PSB�õͣ����ڷ�ʽ
#define PSB_SET	P5OUT |= (1 << P50)         //PSB�øߣ����ڷ�ʽ

#define RST_CLR	P5OUT &= ~(1 << P51)        //RST�õ�
#define RST_SET	P5OUT |= (1 << P51)         //RST�ø�

#define DataPort      P4OUT                 //P4��Ϊ���ݿ�

#define LCD_BUFFER_SIZE 512
#define LCD_SCREEN_BUFFER_SIZE 16

#include <msp430x24x.h>
#include <assert.h>
#include <string.h>
#include "hardware.h"

//��Ϣ����
static char lcd_buffer[LCD_BUFFER_SIZE];
//��ǰ��ʾ������
static char *buffer_ptr;
//�Ƿ���
static uchar is_show_lcd_screen;
//���������Ƿ�����Ϣ
static uchar is_lcd_message;

static void lcd_write_instruction(unsigned char LCD_instruction);
static void lcd_setGuiBuf(char *src, int total_size);
static void lcd_get_screen_guiBuf(char *dest, int total_size);

//*************************************************************************
//                              д�뻺����
//*************************************************************************
static void lcd_setGuiBuf(char *src, int total_size)
{
    assert(total_size <= LCD_BUFFER_SIZE);
    
    //����ջ�����
    memset(lcd_buffer, 0, sizeof(lcd_buffer));
    memcpy(lcd_buffer, src, total_size);
    
    //��ָ��ָ����ʼ��ַ
    buffer_ptr = lcd_buffer;
    is_lcd_message = 1;
}


//*************************************************************************
//          ��ȡ��������һ��ֻ��ȡһ�������ݣ���С��16���ֽ�
//*************************************************************************
static void lcd_get_screen_guiBuf(char *dest, int total_size)
{
  assert(total_size > LCD_SCREEN_BUFFER_SIZE);
  
  //�����ǰ��¼û����ʾ���
  if((buffer_ptr < lcd_buffer + strlen(lcd_buffer)))
  {
    //���ʣ���������ܹ���ʾһ��
    if((lcd_buffer + strlen(lcd_buffer) - buffer_ptr) > LCD_SCREEN_BUFFER_SIZE)
    {
      memcpy(dest, buffer_ptr, LCD_SCREEN_BUFFER_SIZE);
      buffer_ptr += LCD_SCREEN_BUFFER_SIZE;
    }
    else
    {
      memcpy(dest, buffer_ptr, lcd_buffer + strlen(lcd_buffer) - buffer_ptr);
      buffer_ptr = lcd_buffer;
    }
  }
  else
  {
    memcpy(dest, 0, total_size);
    buffer_ptr = lcd_buffer;
  }
}


//*************************************************************************
//                            ���LCD״̬�������ǲ��ǻ���æ��
//*************************************************************************
void LCD_check_busy(void)   
{
}

//*************************************************************************
//                             LCD��궨λ��x��
//*************************************************************************
void LCD_set_position(unsigned char lcdpage,unsigned char lcdline)            
{
}

//*************************************************************************
//                          ���һ���ַ���LCD
//*************************************************************************
void LCD_printc(unsigned char lcd_data)          
{
}

//*************************************************************************
//                          ���һ���ַ�����LCD
//*************************************************************************
void LCD_prints(unsigned char *lcd_string)       
{
}

//*************************************************************************
//                             LCD����λ
//*************************************************************************
/*
void LCD_go_home(void)                         
{
lcd_write_instruction(LCD_GO_HOME);
}
*/

//===============���ϲ��־�Ϊ�սӿڣ�������ͬ�ͺŵ�LCD��չ=============

//*************************************************************************
//                             LCD����
//*************************************************************************
void LCD_cls()          
{
    lcd_write_instruction(0x01);
    delay_ms(5);  
}

//*************************************************************************
//                            �ر�LCD��
//*************************************************************************
void LCD_turn_off()
{
  is_show_lcd_screen = 0;
}

//*************************************************************************
//                            ��LCD��
//*************************************************************************
void LCD_turn_on()
{
  is_show_lcd_screen = 1;
}

//*************************************************************************
//                            дָ�LCD
//*************************************************************************
static void lcd_write_instruction(unsigned char LCD_instruction)  
{
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort = LCD_instruction;
    delay_ms(5);
    EN_CLR;
}

//*************************************************************************
//                            ���һ���ֽ����ݵ�LCD
//*************************************************************************
void LCD_write_data(unsigned char LCD_data)      
{
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort = LCD_data;
    delay_ms(5);
    EN_CLR;
}

//*************************************************************************
//                             ��ʾһ������
//*************************************************************************
void LCD_displayCgromLine(uchar line, char *hz)
{
    uchar address = 0x80;
    assert(line < 2);
    switch(line)
    {
    case 0:
      address = 0x80;
      break;
    case 1:
      address = 0x90;
      break; 
    }
    lcd_write_instruction(address);
    delay_ms(5);
    while(*hz != '\0')
    {
      LCD_write_data(*hz);
      hz++;
      delay_ms(5);
    }
}

//*************************************************************************
//                            ��ʾCGROM��ĺ���
//*************************************************************************
/*
void LCD_displayCgrom(char *hz)
{
  //�ȷ��У�����������ʾ
  uchar i = 0;
  uchar lines = strlen(hz) / 2 / 4;
  char content[10] = {0};
  if((strlen(hz) / 2 % 2) != 0 && lines > 1)
  {
    lines++;
  }
  
  for(i = 0; i <= lines; i++)
  {
    if(strlen(hz) > 8)
    {
      strncpy(content, hz, 8);
      content[8] = 0;
      hz += 8;
    }
    else
    {
      strcpy(content, hz);
    }
    
    LCD_displayCgromLine(i % 2, content);
    
    if(((i % 2) - 1 == 0) & i > 2)
    {
      //��ʾ��һ������delay��������
      delay_ms(5000);
      LCD_cls();
    }
  }
} 
*/

void LCD_displayCgrom(char *hz)
{
  lcd_setGuiBuf(hz, strlen(hz) + 1);
}

//*************************************************************************
// ��ʾһ��
//*************************************************************************
void LCD_displayCgrom_screen()
{
  char temp[18] = {0};
  char temp1[9] = {0};
  char temp2[9] = {0};

  lcd_get_screen_guiBuf(temp, 18);
  memcpy(temp1, temp, 8);
  memcpy(temp2, temp + 8, 8);
  if(temp1[0])
    LCD_displayCgromLine(0 , temp1);
  if(temp2[0])
    LCD_displayCgromLine(1 , temp2);
}

//*************************************************************************
//   debug������һ��LCD_displayCgrom()��wrapper����
//*************************************************************************
void LCD_debug_displayCgrom(char *hz)
{
  LCD_displayCgromLine(0, hz);
}

//*************************************************************************
//                         ��ʼ��LCD
//*************************************************************************
void LCD_initial()                        
{
    PSB_SET;		  //Һ�����ڷ�ʽ
    RST_SET;		  //��λ��RST�ø�
    
    lcd_write_instruction(FUN_MODE);			//��ʾģʽ����
    delay_ms(5);
    lcd_write_instruction(DISPLAY_ON);			//��ʾ��
    delay_ms(5);
    lcd_write_instruction(CLEAR_SCREEN);		//����
    delay_ms(5);
    lcd_write_instruction(CURSE_ADD);
    delay_ms(5);
    
    //����ʱ��
    TACTL |= TASSEL_1 + ID_0 + MC_1;
    TACCR0 = 100000;
    CCTL0 = CCIE;
    
    buffer_ptr = lcd_buffer;
    is_show_lcd_screen = 1;
    is_lcd_message = 0;
}

//*************************************************************************
//�жϴ���������ʾ����
//*************************************************************************
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  if(is_show_lcd_screen && is_lcd_message)
  {
    LCD_cls();
    LCD_displayCgrom_screen();
  }
}

void LCD_test()
{
  LCD_displayCgrom("�й�����������");
  delay_ms(10000);
  LCD_displayCgrom("hello");
  //LCD_displayCgrom_screen();
}