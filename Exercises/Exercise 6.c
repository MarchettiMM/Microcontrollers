#include <msp430fr5949.h>
int TempIdeal = 35;
signed long int buffer = 0;
signed long int buffer1[100];
unsigned int qdeAmostrasADC = 10;
unsigned int contSomaAmostra = 0;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5;
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_3; // 4MHZ
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
    P5DIR &= ~(BIT6 | BIT5);
    P5IE |= BIT6 | BIT5;
    P5IES &= ~(BIT6 | BIT5);
    P5IFG = 0x00;
}
#pragma vector = PORT5_VECTOR
__interrupt void PORT5(void)
{
    if (P5IFG & BIT5)
    { // Botão Aumentar Temp Ideal
        TempIdeal = TempIdeal + 1;
        while (!(P5IN & BIT5))
            ;
        P5IFG &= ~BIT5;
    }
    if (P5IFG & BIT6)
    { // Botão Diminuir Temp Ideal
        TempIdeal = TempIdeal - 1;
        while (!(P5IN & BIT6))
            ;
        P5IFG &= ~BIT6;
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
    TB0CCR0 = 228; // 17593 HZ
    TB0CCTL2 = OUTMOD_7;
    TB0CCR2 = x;
    TB0CTL = TBSSEL_1 | ID__1 | MC__UP; // ACLK
}
void Configura_ADC(void)
{
    P1SEL1 |= BIT0; // Pino
    P1SEL0 |= BIT0; // Pino
    ADC12CTL0 = ADC12ON;
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL1 = ADC12SHP | ADC12BUSY;
    ADC12CTL2 = ADC12RES_1;                  // Numero de bits(10BITS)
    ADC12MCTL0 = ADC12INCH_0 | ADC12VRSEL_1; // Caixa dàgua e o aX/ Escolha do
    AX(De acordo
           com o pino que foi
               escolhido /
       Escolhe a fonte de referência)
    ADC12IER0 |= ADC12IE0;
    while (REFCTL0 & REFGENBUSY)
        ;
    REFCTL0 = REFVSEL_1 | REFON; // Valor de referencia(2.0V limite)
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
            buffer = (buffer / contSomaAmostra); // Não
            fizemos a
            conversão(então
                          o nosso valor
                              esta conts)
                contSomaAmostra = 0;
            if (buffer == TempIdeal)
            {
                x = 0;
            }
            __bic_SR_register_on_exit(LPM0_bits);
        }
        else
        {
            ADC12CTL0 |= ADC12ENC + ADC12SC; // Colocar
            essa
                linha para quando fazermos uma leitura
        }
        break;
    default:
        break;
    }
}