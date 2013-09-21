#include <msp430x14x.h>

void Spark_init()
{

}

void Spark_action()
{
  P6DIR |= BIT1;  
  P6OUT ^= 0x02;
}

void Spark_stop()
{
  P6DIR |= BIT1;  
  P6OUT = 0x00;
}