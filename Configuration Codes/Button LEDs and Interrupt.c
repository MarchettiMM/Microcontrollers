#include <msp430fr5949.h>
// unsigned long int i = 0;
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
    TA0CCR0 = 2048;
    // TA0EX0 = TAIDEX_1;
    TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 2048;
    __bic_SR_register_on_exit(LPM3_bits);
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    __bis_SR_register(GIE);
    P3DIR |= 0xFF;
    P3OUT |= 0xFF;
    P1DIR &= ~BIT4;
    while (1)
    {
        if ((P1IN & BIT4) == 0)
        {
            P3OUT ^= BIT0;
            P3OUT |= BIT1;
            __no_operation();
        }
        else
        {
            P3OUT ^= BIT1;
            P3OUT |= BIT0;
        }
        // for(i = 0; i < 10000; i++);
        __bis_SR_register(LPM3_bits);
    }
}