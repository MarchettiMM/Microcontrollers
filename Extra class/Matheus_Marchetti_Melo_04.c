#include "io430.h"

int Flag = 0;
int contador = 0;
void Configura_Clock(void)
{
  PJSEL0 |= BIT4 | BIT5;
  PM5CTL0 &= ~LOCKLPM5;

  CSCTL0_H = CSKEY >> 8;
  CSCTL1 = DCOFSEL_3 | DCORSEL;
  CSCTL2 = SELA_0 | SELS_3 | SELM_3;
  CSCTL3 = DIVA_0 | DIVS_0 | DIVM_0;
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);
  CSCTL0_H = 0;
}
void Configura_Timer(void)
{
  TA0CCTL0 = CCIE;
  TA0CCR0 = 4096;

  TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  TA0CCR0 += 4096;
  if (Flag == 1)
  {
    if (contador == 2)
    {
      P3OUT |= BIT0;
      P3OUT &= ~(BIT1);
      P3OUT |= BIT2;
    }
    if (contador == 8)
    {
      P3OUT &= ~(BIT0);
      P3OUT |= BIT1;
      P3OUT |= BIT2;
    }
    if (contador == 35)
    {
      P3OUT |= BIT0;
      P3OUT |= BIT1;
      P3OUT &= ~(BIT2);
      contador = 0;
    }
    contador = contador + 1;
    __bic_SR_register_on_exit(LPM0_bits);
  }
  if (Flag == 2)
  {
    if (contador == 2)
    {
      P3OUT |= BIT0;
      P3OUT &= ~(BIT1);
      P3OUT |= BIT2;
    }
    if (contador == 8)
    {
      P3OUT &= ~(BIT0);
      P3OUT |= BIT1;
      P3OUT |= BIT2;
    }
    if (contador == 35)
    {
      P3OUT |= BIT0;
      P3OUT |= BIT1;
      P3OUT &= ~(BIT2);
      contador = 0;
    }
    contador = contador + 1;
    __bic_SR_register_on_exit(LPM0_bits);
  }
}
void Configura_Led(void)
{
  P3DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
  P3OUT |= BIT0 | BIT1 | BIT3 | BIT4;
  P3OUT &= ~(BIT2 | BIT5);
}
void Configura_Botao(void)
{
  P1DIR &= ~(BIT3 | BIT4);
  P1IE |= BIT3 | BIT4;
  P1IES &= ~(BIT3 | BIT4);
  P1IFG = 0x00;
}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1(void)
{
  if (P1IFG & BIT3)
  {
    Flag = 1;
  }
  if (P1IFG & BIT4)
  {
    Flag = 2;
  }
}
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  Configura_Clock();
  Configura_Timer();
  Configura_Led();
  Configura_Botao();
  __bis_SR_register(GIE);
  while (1)
  {
    __bis_SR_register(LPM0_bits);
  }
}