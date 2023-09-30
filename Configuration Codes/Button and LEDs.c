#include <msp430fr5949.h>
unsigned long int i = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;
    P3DIR |= 0xFF;
    P3OUT |= 0xFF;
    P1DIR &= ~BIT3;
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