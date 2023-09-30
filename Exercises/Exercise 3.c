#include <msp430fr5994.h>
int cont1 = 0;
int cont2 = 0;
int cont3 = 0;
float x;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5;             // Pinos do Oscilador
    PM5CTL0 &= ~LOCKLPM5;              // Senha de acesso dos pinos
    CSCTL0_H = CSKEY >> 8;             // CSKEY password
    CSCTL1 = DCOFSEL_0;                // Configura DCO: 1MHz
    CSCTL2 = SELA_0 | SELS_3 | SELM_3; // ACLK = XT1; SMCLK e MCLK =
    DCO
        CSCTL3 = DIVA_2 | DIVS_0 | DIVM_0; // Divisão do Clock: 1 - 32
    do
    {
        CSCTL5 &= ~LFXTOFFG;   // Limpa flag de falta XT1
        SFRIFG1 &= ~OFIFG;     // Limpa
    } while (SFRIFG1 & OFIFG); // Testa flag de falta do
    oscilador
        CSCTL0_H = 0; // Trava os registradores CS
}
void Configura_TimerA(void)
{ // Pag. 444 do datasheet
    TA0CCTL0 = CCIE;
    TA0CCR0 = 61440; // Base para Contagem do
    Timer(contar a cada 1min)
        TA0CTL = TASSEL_1 + MC_2 + ID_3; // Configuração do Timer
}
void Configura_LEDs(void)
{
    P1DIR |= BIT0 | BIT1 | BIT2;    // Direção do D25 LED
    P1OUT &= ~(BIT0 | BIT1 | BIT2); // Desliga LED D25
    P1OUT |= BIT1;
}
void ConfiguraPWM(void)
{
    P1DIR |= BIT5 | BIT4;
    P1SEL0 |= BIT5 | BIT4;
    P3DIR |= BIT5 | BIT4;
    P5SEL0 |= BIT5 | BIT4;
    TB0CCR0 = 20480;
    TB0CCTL2 = OUTMOD_7;
    TB0CCR2 = 3686; // 18%
    TB0CCTL1 = OUTMOD_7;
    TB0CCR1 = 7168; // 35%
    TB0CCTL4 = OUTMOD_7;
    TB0CCR4 = 13926; // 68%
    TB0CCTL4 = OUTMOD_7;
    TB0CCR4 = x;
    TB0CTL = TBSSEL_1 | ID_0 | MC__UP;
}
void Configura_ADC(void)
{
    P1SEL1 |= BIT0; // Escolhe os pins
    P1SEL0 |= BIT0; // Escolhe os pinos
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12ON;
    ADC12CTL1 = ADC12SHP | ADC12BUSY;
    ADC12CTL2 = ADC12RES_2;                  // ADC: 12-bit
    ADC12MCTL0 = ADC12INCH_0 | ADC12VRSEL_1; // Pesquisar sobre o A do
    pino e escolher
        a fonte de referência
            ADC12IER0 |= ADC12IE0;
    while (REFCTL0 & REFGENBUSY)
        ;
    REFCTL0 = REFVSEL_1 | REFON; // Ref = 2.0V
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
        if ((++contSomaAmostra) >=
            qdeAmostrasADC)
        {
            buffer = (buffer / contSomaAmostra) *
                     (2 / 4096) * (100); // 2/2ˆ12
            (CONVERSAO de conts para
                 volts)1 /
                100(conversao de
                        volts para %)
                    contSomaAmostra = 0;
            x = buffer * (20480 / 100);
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
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 61440; // Mais x tempo
    if (cont == 2)
    {
        // Vai piscar a cada 2 min
        P1OUT ^= (BIT0); // Alterna nível
        lógico D25(Led1)
            cont1 = 0;
    }
    if (cont == 120)
    {
        P1OUT ^= BIT1
            //(led2)
            cont2 = 0;
    }
    if (cont == 2880)
    {
        P1OUT ^= BIT2
            //(led3)
            cont3 = 0;
    }
    cont1++;
    cont2++;
    cont3++;
    __bic_SR_register_on_exit(LPM0_bits); // Sai do modo LPM3
}