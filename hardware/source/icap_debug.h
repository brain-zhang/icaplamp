//本文件实现调试输出
//release版本发布时，将ICAP_DEBUG宏注释即可

#ifndef __ICAP_DEBUG_H__
#define __ICAP_DEBUG_H__

#define ICAP_DEBUG

#include "hardware.h"
#include "lcd.h"

#ifdef ICAP_DEBUG
#define DEBUG_PRINT(expr) LCD_debug_displayCgrom(expr); 
                            //delay_ms(5000);
#else
#define DEBUG_PRINT(expr)
#endif

#endif