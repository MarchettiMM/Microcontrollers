#include <MSP430FR5949.h>
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
    TA0CTL = TASSEL_1 + MC_2 + ID_3;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 4096;
    P3OUT ^= BIT1;
    __bic_SR_register_on_exit(LPM0_bits);
}
void Configura_UART(void)
{
    P2SEL1 |= BIT5 | BIT6;
    P2SEL0 &= ~(BIT5 | BIT6);
    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_2;
    UCA1BR0 = 26;
    UCA1BR1 = 0x00; // BR0 e BR1 = 8MHz/(16*19200) = 26.041
    UCA1MCTLW = UCOS16;
    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE = UCRXIE;
}
int Comandos(char CD)
{
    if ((CD == 'L') || (CD == 'l'))
    {
        P1OUT |= BIT5; // Liga Ventilador
    }
    else
    {
        if ((CD == 'D') || (CD == 'd'))
        {
            P1OUT &= ~BIT5; // Desliga Ventilador
        }
        else
        {
            if ((CD == 'I') || (CD == 'i'))
            {
                P3OUT ^= BIT0; // Indicador Luminoso
            }
        }
    }
    return 0;
}
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_UART_UCRXIFG:
        UCA1TXBUF = UCA1RXBUF;
        Comandos(UCA1RXBUF);
        __no_operation();
        break;
    }
}
void Configura_LEDs(void)
{
    P3DIR = 0xFF;
    P3OUT = 0xFF;
    P1DIR |= BIT0 | BIT1;
    P1OUT |= BIT0 | BIT1;
}
void Configura_Ventilador(void)
{
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    Configura_LEDs();
    Configura_Ventilador();
    Configura_UART();
    __bis_SR_register(GIE);
    while (1)
    {
        __bis_SR_register(LPM0_bits);
        __no_operation();
        while (!(UCA1IFG & UCTXIFG))
            ;
        UCA1TXBUF = 0x40;
    }
}