#include <msp430fr5949.h>
unsigned long int i = 0;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5;
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_6;
    CSCTL2 = SELA_0 | SELS_3 | SELM_3;
    CSCTL3 = DIVA_4 | DIVS_2 | DIVM_1;
    do
    {
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);
    CSCTL0_H = 0;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;
    P3DIR |= 0xFF;
    P3OUT |= 0xFF;
    P1DIR &= ~BIT3;
    Configura_Clock();
    while (1)
    {
        for (i = 0; i < 10000; i++)
            ;
        if ((P1IN & BIT3) == 0)
        {
            P3OUT ^= BIT7;
            P3OUT |= BIT6;
            __no_operation();
        }
        else
        {
            P3OUT ^= BIT6;
            P3OUT |= BIT7;
        }
    }
}