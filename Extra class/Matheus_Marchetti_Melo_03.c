#include "io430.h"

int Flag = 0;
int contA = 0;
int contB = 1;
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
void Configura_TimerA(void)
{
  TA0CCTL0 = CCIE;
  TA0CCR0 = 1024;
  TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
void Configura_Led(void)
{
  P2DIR |= BIT0;
  P2OUT &= ~BIT0;
  P3DIR |= BIT7;
  P3OUT &= ~BIT7;
  P3DIR |= BIT6;
  P3OUT &= ~BIT6;
  P3DIR |= BIT5;
  P3DIR |= BIT4;
  P3OUT &= ~BIT4;
  P3DIR |= BIT3;
  P3OUT &= ~BIT3;
  P3DIR |= BIT2;
  P3OUT &= ~BIT2;
  P3DIR |= BIT1;
  P3OUT &= ~BIT1;
  P3DIR |= BIT0;
  P3OUT &= ~BIT0;
}
void Configura_Botao(void)
{
  P1DIR &= ~(BIT3);
  P1IE |= BIT3;
  P1IES &= ~(BIT3);
  P1IFG = 0x00;
}
void Configura_Ventilador(void)
{
  P1DIR |= BIT5;
  P1OUT &= ~BIT5;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  TA0CCR0 += 1024;
  if (Flag == 1)
  {
    P2OUT ^= BIT0;
    if (contA % 2 == 0)
    {
      P3OUT ^= BIT7;
    }
    if (contA % 4 == 0)
    {
      P3OUT ^= BIT6;
    }
    if (contA % 8 == 0)
    {
      P3OUT ^= BIT5;
    }
    if (contA % 16 == 0)
    {
      P3OUT ^= BIT4;
    }
    if (contA % 32 == 0)
    {
      P3OUT ^= BIT3;
    }
    if (contA % 64 == 0)
    {
      P3OUT ^= BIT2;
    }
    if (contA % 128 == 0)
    {
      P3OUT ^= BIT1;
    }
    if (contA % 256 == 0)
    {
      P3OUT ^= BIT0;
    }
    if (contA % 512 == 0)
    {
      P1OUT |= BIT5;
      contA = 0;
    }
    if (contA == 224)
    {
      P1OUT &= ~BIT5;
    }
    contA = contA + 1;
  }
  __bic_SR_register_on_exit(LPM0_bits);
}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1(void)
{
  if (P1IFG & BIT3)
  {
    Flag = 1;
    if (contB == 2)
    {
      Flag = 0;
      contB = 0;
    }
    contB = contB + 1;
    while (!(P1IN & BIT3))
      ;
    P1IFG &= ~BIT3;
  }
}
int main()
{
  WDTCTL = WDTPW | WDTHOLD;
  Configura_Clock();
  Configura_TimerA();
  Configura_Led();
  Configura_Botao();
  __bis_SR_register(GIE);
  while (1)
  {
    __bis_SR_register(LPM0_bits);
    __no_operation();
    __no_operation();
  }
}