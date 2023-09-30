#include <msp430fr5949.h>

int TempIdeal = 35;
int x = 60;
signed long int buffer = 0;
signed long int buffer1[100];
unsigned int qdeAmostrasADC = 10;
unsigned int contSomaAmostra = 0;

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
void Configura_Botao(void)
{
  P3DIR &= ~BIT6;
  P3DIR &= ~BIT5;
  P3IE |= BIT6 | BIT5;
  P3IES &= ~(BIT6 | BIT5);
  P3IFG = 0x00;
}
#pragma vector = PORT3_VECTOR
__interrupt void PORT5(void)
{
  if (P3IFG & BIT5)
  {
    TempIdeal = TempIdeal + 1;
    while (!(P3IN & BIT5))
      ;
    P3IFG &= ~BIT5;
  }
  if (P3IFG & BIT6)
  {
    TempIdeal = TempIdeal - 1;
    while (!(P3IN & BIT6))
      ;
    P3IFG &= ~BIT6;
  }
}
void Configura_TimerA(void)
{
  TA0CCTL0 = CCIE;
  TA0CCR0 = 4096;
  TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  TA0CCR0 += 4096;
  __bic_SR_register_on_exit(LPM0_bits);
}
void ConfiguraPWM(void)
{
  P1DIR |= BIT5;
  P1SEL0 |= BIT5;
  TB0CCR0 = 228;
  TB0CCTL2 = OUTMOD_7;
  TB0CCR2 = x;
  TB0CTL = TBSSEL_1 | ID__1 | MC__UP;
}
void Configura_ADC(void)
{
  P1SEL1 |= BIT0;
  P1SEL0 |= BIT0;
  ADC12CTL0 = ADC12ON;
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL1 = ADC12SHP | ADC12BUSY;
  ADC12CTL2 = ADC12RES_1; // Numero de bits = 10
  ADC12MCTL0 = ADC12INCH_0 | ADC12VRSEL_1;
  ADC12IER0 |= ADC12IE0;
  while (REFCTL0 & REFGENBUSY)
    ;
  REFCTL0 = REFVSEL_1 | REFON; // Valor de referencia = 2.0V
  while (!(REFCTL0 & REFGENRDY))
    ;
}
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
  case ADC12IV_ADC12IFG0:
    buffer += (ADC12MEM0);
    buffer1[contSomaAmostra] = (ADC12MEM0);
    if ((++contSomaAmostra) >= qdeAmostrasADC)
    {
      buffer = (buffer / contSomaAmostra);
      contSomaAmostra = 0;
      if (buffer == TempIdeal)
      {
        x = 0;
      }
      __bic_SR_register_on_exit(LPM0_bits);
    }
    else
    {
      ADC12CTL0 |= ADC12ENC + ADC12SC;
    }
    break;
  default:
    break;
  }
}
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  Configura_Clock();
  Configura_Botao();
  Configura_TimerA();
  ConfiguraPWM();
  Configura_ADC();
  __bis_SR_register(GIE);
  while (1)
  {
    __bis_SR_register(LPM0_bits);
    __no_operation();
    __no_operation();
    ADC12CTL0 |= ADC12ENC | ADC12SC;
  }
}