#include <msp430fr5949.h>
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
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 4096;
    P3OUT ^= BIT0;
    __bic_SR_register_on_exit(LPM0_bits);
}
void Configura_LEDs(void)
{
    P2DIR |= BIT0;
    P2OUT |= BIT0;
    P3DIR = 0xFF;
    P3OUT = 0xFF;
}
void Configura_TimerA(void)
{
    TA0CCTL0 = CCIE;
    TA0CCR0 = 4096;
    TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
void ConfiguraPWM(void)
{
    P1DIR |= BIT5;
    P1SEL0 |= BIT5;
    TB0CCR0 = 360;
    TB0CCTL2 = OUTMOD_7;
    TB0CCR2 = 60;
    TB0CTL = TBSSEL__ACLK | ID__1 | MC__UP;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    Configura_LEDs();
    ConfiguraPWM();
    __bis_SR_register(GIE);
    while (1)
    {
        __bis_SR_register(LPM0_bits);
        __no_operation();
        __no_operation();
    }
}