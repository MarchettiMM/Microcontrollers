#include "io430.h"

int x = 10;
int y = 8;
void Configura_Clock(void)
{
  PJSEL0 |= BIT4 | BIT5;
  PM5CTL0 &= ~LOCKLPM5;
  CSCTL0_H = CSKEY >> 8;
  CSCTL1 = DCOFSEL_3;
  CSCTL2 = SELA_0 | SELS_3 | SELM_3;
  CSCTL3 = DIVA_0 | DIVS_0 | DIVM_0;
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);
  CSCTL0_H = 0;
}
void Configura_TimerA(void)
{
  TA0CCTL0 = CCIE;
  TA0CCR0 = 4096;
  TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
void Configura_Botao(void)
{
  P2DIR &= ~(BIT5 | BIT6);
  P2IE |= BIT5 | BIT6;
  P2IES &= ~(BIT5 | BIT6);
  P2IFG = 0x00;
  P1DIR &= ~(BIT3 | BIT4);
  P1IE |= BIT3 | BIT4;
  P1IES &= ~(BIT3 | BIT4);
  P1IFG = 0x00;
}
void Configura_PWMVentiladorLed(void)
{
  P1DIR |= BIT5;
  P1DIR |= BIT4;
  P1SEL0 |= BIT5;
  P1SEL0 |= BIT4;
  TB0CCR0 = 111;
  TB0CCTL2 = OUTMOD_7;
  TB0CCR2 = x;
  TB0CCTL1 = OUTMOD_7;
  TB0CCR1 = y;
  TB0CTL = TBSSEL__SMCLK | ID__1 | MC__UP;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  TA0CCR0 += 4096;
  __bic_SR_register_on_exit(LPM0_bits);
}
#pragma vector = PORT2_VECTOR
__interrupt void PORT2(void)
{
  if (P2IFG & BIT5)
  {
    x = x + 1;
    if (x >= 98)
    {
      x = 98;
    }
    while (!(P2IN & BIT5))
      ;
    P2IFG &= ~BIT5;
  }
  if (P2IFG & BIT6)
  {
    x = x - 1;
    if (x <= 9)
    {
      x = 9;
    }
    while (!(P2IN & BIT6))
      ;
    P2IFG &= ~BIT6;
  }
  if (P1IFG & BIT3)
  {
    y = y + 1;
    if (y >= 102)
    {
      y = 102;
    }
    while (!(P1IN & BIT3))
      ;
    P1IFG &= ~BIT3;
  }
  if (P1IFG & BIT4)
  {
    y = y - 1;
    if (y <= 7)
    {
      y = 7;
    }
    while (!(P1IN & BIT4))
      ;
    P1IFG &= ~BIT4;
  }
}
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  Configura_Clock();
  Configura_TimerA();
  Configura_Botao();
  Configura_PWMVentiladorLed();
  __bis_SR_register(GIE);
  while (1)
  {
    __bis_SR_register(LPM0_bits);
    __no_operation();
    __no_operation();
  }
}