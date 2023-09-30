#include <msp430fr5949.h>
int contador1 = 0;
int contador2 = 0;
int contador3 = 0;
void Configura_Leds(void)
{
    P3DIR |= BIT0;  // LED1
    P3DIR &= ~BIT0; // LED1
    P3DIR |= BIT1;  // LED2
    P3DIR &= ~BIT1; // LED2
    P3DIR |= BIT2;  // LED3
    P3DIR &= ~BIT2; // LED3
}
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5; // Importante(Muda em cada ocasião)
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Importante(Muda em cada ocasião)
    8MHZ CSCTL2 = SELA_0 | SELS_3 | SELM_3; // Importante(Saber o jeito
certo)
CSCTL3 = DIVA_0 | DIVS_0 | DIVM_0;//Importante(Muda em cada
ocasião)
do
{
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
}
while (SFRIFG1 & OFIFG)
    ;
CSCTL0_H = 0;
}
void Configura_TimerA(void)
{
TA0CCTL0 = CCIE;
TA0CCR0 = 4096; // Limite de 65536 e também não pode ser valor
decimal
    TA0CTL = TASSEL_2 + MC_2 + ID_3; // Importante(_1(Auxiliar)
_2(DCO))
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
TA0CCR0 += 4096; //(1segundo)
if (contador1 == 60)
{ // (1 min - 60 segundos)
    P3OUT ^= BIT0;
    contador1 = 0;
}
if (contador2 == 600)
{ // 10 min - 600 segundos
    P3OUT ^= BIT1;
    contador2 = 0;
}
if (contador2 == 3600)
{ // 10 min - 600 segundos
    P3OUT ^= BIT2;
    contador3 = 0;
}
contador1++;
contador2++;
contador3++;
__bic_SR_register_on_exit(LPM0_bits);
}
void ConfiguraPWM(void)
{
P1DIR |= BIT5 | BIT4 | BIT6 | BIT7; // Importante(Muda em cada
ocasião)
P1SEL0 |= BIT5 | BIT4 | BIT6 | BIT7; //Importante(Muda em cada
ocasião)
TB0CCR0 = 160; // Limite de 65536 e também não pode ser valor
decimal
    TB0CCTL2 = OUTMOD_7; //_7 trabalha com o valor direto da
porcentagem e _3
    trabalha com o resto(1 - x)
        TB0CCR2 = 142;
TB0CCTL1 = OUTMOD_7;
TB0CCR1 = 80;
TB0CCTL3 = OUTMOD_7;
TB0CCR3 = 48;
TB0CCTL4 = OUTMOD_7;
TB0CCR4 = x;
TB0CTL = TBSSEL_2 | ID_0 | MC__UP;
}
void Configura_ADC(void)
{
P1SEL1 |= BIT1; // Pino
P1SEL0 |= BIT1; // Pino
ADC12CTL0 = ADC12ON;
ADC12CTL0 &= ~ADC12ENC;
ADC12CTL1 = ADC12SHP | ADC12BUSY;
ADC12CTL2 = ADC12RES_1;                  // Numero de bits(10BITS)
ADC12MCTL0 = ADC12INCH_1 | ADC12VRSEL_1; // Caixa dàgua e o
aX(Escolhe
       pra onde buscar o limite da
           tesão)
    ADC12IER0 |= ADC12IE0;
while (REFCTL0 & REFGENBUSY)
    ;
REFCTL0 = REFVSEL_0 | REFON; // Valor de referencia do limite de
tensão(caixa dagua) 1.2 while (!(REFCTL0 & REFGENRDY));
}
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
{
case ADC12IV_ADC12IFG0:
    buffer += (ADC12MEM0);
    buffer1[contSomaAmostra] =
        (ADC12MEM0);
    if ((++contSomaAmostra) >=
        qdeAmostrasADC)
    {
        buffer = (buffer / contSomaAmostra) *
                 (1.2 / 1024); // Para converter a unidade conts para
        volts tenq fazer - vezes o(valor maximo de
                                       tensão escolhida(Caixa dagua)) /
                               (valor
                                    maximo de conts(2 ^ numero de bits que
                                                            escolhemos))
                                   x = buffer * (160); //
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
int main(void)
{
WDTCTL = WDTPW | WDTHOLD;
Configura_Clock();
Configura_TimerA();
Configura_LEDs();
ConfiguraPWM();
Configura_ADC();
__bis_SR_register(GIE);
while (1)
{
    __bis_SR_register(LPM0_bits);
    ADC12CTL0 |= ADC12ENC + ADC12SC;
    __no_operation();
    __no_operation();
}
}