//���ļ�ʵ�ֵ������
//release�汾����ʱ����ICAP_DEBUG��ע�ͼ���

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