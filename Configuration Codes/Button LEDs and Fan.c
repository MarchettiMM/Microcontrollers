#include <msp430fr5949.h>
#define Tempo 20
unsigned char contadarSegundos = 0;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5;
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_6;
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
    // TA0EX0 = TAIDEX_1;
    TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 4096;
    contadarSegundos++;
    P3OUT ^= BIT0;
    if (contadarSegundos >= Tempo)
    {
        contadarSegundos = 0;
        __bic_SR_register_on_exit(LPM3_bits);
    }
}
void Configura_Botao(void)
{
    P2DIR &= ~(BIT5 | BIT6);
}
void Configura_Ventilador(void)
{
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;
}
void Configura_LEDs(void)
{
    P3DIR |= 0xFF;
    P3OUT |= 0xFF;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    Configura_LEDs();
    Configura_Botao();
    Configura_Ventilador();
    __bis_SR_register(GIE);
    while (1)
    {
        __bis_SR_register(LPM3_bits);
        P3OUT ^= BIT1;
        if ((P2IN & BIT5) == 0)
        {
            P3OUT &= ~BIT2;
            P1OUT |= BIT5;
        }
        if ((P2IN & BIT6) == 0)
        {
            P3OUT |= BIT2;
            P1OUT &= ~BIT5;
        }
    }
}