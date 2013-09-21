#include <msp430x14x.h>
#include "hardware.h"

void WIFI_init()
{
  delay_ms(3000);
  P6DIR |= BIT0;
  P6OUT |= 0x01;
}

void WIFI_test()
{
  WIFI_init();
  while(1)
  {
    P6OUT ^= 0x01;
    delay_ms(5000);
  }
}