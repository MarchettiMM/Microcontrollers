#include "io430.h"

unsigned long int i = 0;
unsigned long int p = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  PM5CTL0 &= ~LOCKLPM5;

  P3DIR |= 0xFF;
  P3OUT |= 0xFF;
  P1DIR |= BIT5;
  P1OUT |= BIT5;
  P2DIR &= ~(BIT5 | BIT6);
  P1DIR &= ~(BIT3 | BIT4);
  while (1)
  {
    for (i = 0; i < 10000; i++)
      ;

    if ((P2IN & BIT5) == 0)
    {
      P3OUT ^= BIT0;
      p = p + 1;
    }
    else
    {
      P3OUT |= BIT0;
    }
    if ((P1IN & BIT3) == 0)
    {
      P3OUT ^= BIT1;
      p = p + 1;
    }
    else
    {
      P3OUT |= BIT1;
    }
    if ((P1IN & BIT4) == 0)
    {
      P3OUT ^= BIT2;
      p = p + 1;
    }
    else
    {
      P3OUT |= BIT2;
    }
    if ((P2IN & BIT6) == 0)
    {
      P3OUT ^= BIT3;
      p = p + 1;
    }
    else
    {
      P3OUT |= BIT3;
    }
    if ((p == 2) || (p == 4))
    {
      P1OUT &= ~BIT5;
    }
    else
    {
      P1OUT |= BIT5;
    }
    p = 0;
  }
}