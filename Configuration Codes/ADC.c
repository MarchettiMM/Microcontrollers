#include <msp430fr5949.h>
signed long int buffer = 0;
signed long int buffer1[100];
unsigned int qdeAmostrasADC = 20;
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
    P3OUT ^= BIT1;
    __bic_SR_register_on_exit(LPM0_bits);
}
void Configura_ADC(void)
{
    P1SEL1 |= BIT0;
    P1SEL0 |= BIT0;
    ADC12CTL0 = ADC12ON;
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL1 = ADC12SHP | ADC12BUSY;
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL0 = ADC12INCH_0 | ADC12VRSEL_1;
    ADC12IER0 |= ADC12IE0;
    while (REFCTL0 & REFGENBUSY)
        ;
    REFCTL0 = REFVSEL_1 | REFON;
    while (!(REFCTL0 & REFGENRDY))
        ;
}
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
    {
    case ADC12IV_ADC12IFG0:
        buffer += (ADC12MEM0 - 48);
        buffer1[contSomaAmostra] = (ADC12MEM0 - 48);
        if ((++contSomaAmostra) >= qdeAmostrasADC)
        {
            buffer = (buffer / contSomaAmostra);
            contSomaAmostra = 0;
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
void Configura_LEDs(void)
{
    P2DIR |= BIT0;
    P2OUT |= BIT0;
    P3DIR = 0xFF;
    P3OUT = 0xFF;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    Configura_LEDs();
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