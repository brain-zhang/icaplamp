/*12864应用指令*/
#define CLEAR_SCREEN	0x01		    //清屏指令：清屏且AC值为00H
#define AC_INIT		0x02		    //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD	0x06		    //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE	0x30		    //工作模式：8位基本指令集
#define DISPLAY_ON	0x0c		    //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF	0x08		    //显示关
#define CURSE_DIR	0x14		    //游标向右移动:AC=AC+1
#define SET_CG_AC	0x40		    //设置AC，范围为：00H~3FH
#define SET_DD_AC	0x80

#define P50 0
#define P51 1
#define P55 5
#define P56 6
#define P57 7

#define RS_CLR	P5OUT &= ~(1 << P55)        //RS置低
#define RS_SET	P5OUT |= (1 << P55)         //RS置高

#define RW_CLR	P5OUT &= ~(1 << P56)        //RW置低
#define RW_SET	P5OUT |= (1 << P56)         //RW置高

#define EN_CLR	P5OUT &= ~(1 << P57)        //E置低
#define EN_SET	P5OUT |= (1 << P57)         //E置高

#define PSB_CLR	P5OUT &= ~(1 << P50)        //PSB置低，串口方式
#define PSB_SET	P5OUT |= (1 << P50)         //PSB置高，并口方式

#define RST_CLR	P5OUT &= ~(1 << P51)        //RST置低
#define RST_SET	P5OUT |= (1 << P51)         //RST置高

#define DataPort      P4OUT                 //P4口为数据口

#define LCD_BUFFER_SIZE 512
#define LCD_SCREEN_BUFFER_SIZE 16

#include <msp430x24x.h>
#include <assert.h>
#include <string.h>
#include "hardware.h"

//消息缓存
static char lcd_buffer[LCD_BUFFER_SIZE];
//当前显示到哪里
static char *buffer_ptr;
//是否开屏
static uchar is_show_lcd_screen;
//缓冲区内是否有消息
static uchar is_lcd_message;

static void lcd_write_instruction(unsigned char LCD_instruction);
static void lcd_setGuiBuf(char *src, int total_size);
static void lcd_get_screen_guiBuf(char *dest, int total_size);

//*************************************************************************
//                              写入缓冲区
//*************************************************************************
static void lcd_setGuiBuf(char *src, int total_size)
{
    assert(total_size <= LCD_BUFFER_SIZE);
    
    //先清空缓冲区
    memset(lcd_buffer, 0, sizeof(lcd_buffer));
    memcpy(lcd_buffer, src, total_size);
    
    //将指针指向起始地址
    buffer_ptr = lcd_buffer;
    is_lcd_message = 1;
}


//*************************************************************************
//          读取缓冲区，一次只读取一屏的数据，即小于16个字节
//*************************************************************************
static void lcd_get_screen_guiBuf(char *dest, int total_size)
{
  assert(total_size > LCD_SCREEN_BUFFER_SIZE);
  
  //如果当前记录没有显示完毕
  if((buffer_ptr < lcd_buffer + strlen(lcd_buffer)))
  {
    //如果剩余字数还能够显示一屏
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
//                            检测LCD状态，看它是不是还在忙呢
//*************************************************************************
void LCD_check_busy(void)   
{
}

//*************************************************************************
//                             LCD光标定位到x处
//*************************************************************************
void LCD_set_position(unsigned char lcdpage,unsigned char lcdline)            
{
}

//*************************************************************************
//                          输出一个字符到LCD
//*************************************************************************
void LCD_printc(unsigned char lcd_data)          
{
}

//*************************************************************************
//                          输出一个字符串到LCD
//*************************************************************************
void LCD_prints(unsigned char *lcd_string)       
{
}

//*************************************************************************
//                             LCD光标归位
//*************************************************************************
/*
void LCD_go_home(void)                         
{
lcd_write_instruction(LCD_GO_HOME);
}
*/

//===============以上部分均为空接口，留作不同型号的LCD扩展=============

//*************************************************************************
//                             LCD清屏
//*************************************************************************
void LCD_cls()          
{
    lcd_write_instruction(0x01);
    delay_ms(5);  
}

//*************************************************************************
//                            关闭LCD屏
//*************************************************************************
void LCD_turn_off()
{
  is_show_lcd_screen = 0;
}

//*************************************************************************
//                            打开LCD屏
//*************************************************************************
void LCD_turn_on()
{
  is_show_lcd_screen = 1;
}

//*************************************************************************
//                            写指令到LCD
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
//                            输出一个字节数据到LCD
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
//                             显示一行数据
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
//                            显示CGROM里的汉字
//*************************************************************************
/*
void LCD_displayCgrom(char *hz)
{
  //先分行，两行两行显示
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
      //显示完一屏，先delay，后清屏
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
// 显示一屏
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
//   debug函数，一个LCD_displayCgrom()的wrapper函数
//*************************************************************************
void LCD_debug_displayCgrom(char *hz)
{
  LCD_displayCgromLine(0, hz);
}

//*************************************************************************
//                         初始化LCD
//*************************************************************************
void LCD_initial()                        
{
    PSB_SET;		  //液晶并口方式
    RST_SET;		  //复位脚RST置高
    
    lcd_write_instruction(FUN_MODE);			//显示模式设置
    delay_ms(5);
    lcd_write_instruction(DISPLAY_ON);			//显示开
    delay_ms(5);
    lcd_write_instruction(CLEAR_SCREEN);		//清屏
    delay_ms(5);
    lcd_write_instruction(CURSE_ADD);
    delay_ms(5);
    
    //配置时钟
    TACTL |= TASSEL_1 + ID_0 + MC_1;
    TACCR0 = 100000;
    CCTL0 = CCIE;
    
    buffer_ptr = lcd_buffer;
    is_show_lcd_screen = 1;
    is_lcd_message = 0;
}

//*************************************************************************
//中断处理，周期显示内容
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
  LCD_displayCgrom("中国共产党万岁");
  delay_ms(10000);
  LCD_displayCgrom("hello");
  //LCD_displayCgrom_screen();
}