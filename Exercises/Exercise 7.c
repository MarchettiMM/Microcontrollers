#include <msp430fr5949.h>
int x = 10;
int y = 8;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5;
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_3; // Sabemos que esta em 4MHZ
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
void Configura_Botao(void)
{
    // S3- P2.5
    // S4 - P1.3
    // S5 - P1.4
    // S7 - P2.6
    P2DIR &= ~(BIT5 | BIT6);
    P2IE |= BIT5 | BIT6;
    P2IES &= ~(BIT5 | BIT6);
    P2IFG = 0x00;
    P1DIR &= ~(BIT3 | BIT4);
    P1IE |= BIT3 | BIT4;
    P1IES &= ~(BIT3 | BIT4);
    P1IFG = 0x00;
}
void ConfiguraPWMVentiladorLed(void)
{
    // p1.5 ventilador
    // p1.4 led
    P1DIR |= BIT5; // Parte de escolher o pino para o pwm(Escolher
sempre os que tem TB0 como função(Olhar o mapa
de pinos do datasheet)) Ventilador
P1DIR |= BIT4; // Parte de escolher o pino para o
pwm(Escolher sempre os que tem TB0 como
        função(Olhar o mapa de pinos do datasheet)) Led
    // O P1SEL1 Não esta pq o valor
    dele nesse caso é igual
        a 0 P1SEL0 |= BIT5; // Olhamos no datasheet e colocamos os valores
pedidos na função tb0
    P1SEL0 |= BIT4; // Olhamos no datasheet e colocamos os
valores pedidos na função tb0
    TB0CCR0 = 111; // Calculos(igual do timer) para determinar a
frequência do PWM
    TB0CCTL2 = OUTMOD_7; // 7 é porcentagem direta e 3 porcentagem
complementar
    TB0CCR2 = x; // Escolhe a porcentagem que vamos operar
// Valor minimo do x
para o ventilador = 9
    // Valor máximo do x para o
    ventilador = 98 TB0CCTL1 = OUTMOD_7; // 7 é porcentagem direta e 3 porcentagem
complementar
    TB0CCR1 = y; // Escolhe a porcentagem que vamos operar
// Valor minimo do x
para o led = 7
    // Valor máximo do x para o led
    = 102 TB0CTL = TBSSEL__SMCLK | ID__1 | MC__UP; // Escolher o clock que
vai utilizar | Escolher
por quanto vamos
dividir | Não mexer (Tomar
cuidado com a quantidade
de _, se for só 1 (_)
olhar no datasheet, se for
2(__) se for número divide
pelo próprio numero)
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
TA0CCR0 += 4096;
__bic_SR_register_on_exit(LPM0_bits);
}
#pragma vector = PORT2_VECTOR
__interrupt void PORT2(void)
{
if (P2IFG & BIT5)
{
    // Serve para aumentar a potência do ventilador
    x = x + 1; // A potencia ira aumentar de 1 em 1(não é
porcento)
if(x == 98)
{
    x = 98;
}
while (!(P2IN & BIT5))
    ;
P2IFG &= ~BIT5;
}
if (P2IFG & BIT6)
{
// Serve para diminuir a potência do ventilador
x = x - 1;
if (x == 9)
{
    x = 9;
}
while (!(P2IN & BIT6))
    ;
P2IFG &= ~BIT6;
}
if (P1IFG & BIT3)
{
// Serve para aumentar a potência do ventilador
y = y + 1; // A potencia ira aumentar de 1 em 1(não é
porcento)
if(y == 102)
{ // Aqui é o máximo
    y = 102;
}
while (!(P1IN & BIT3))
    ;
P1IFG &= ~BIT3;
}
if (P1IFG & BIT4)
{
// Serve para diminuir a potência do ventilador
y = y - 1;
if (y == 7)
{
    y = 7;
}
while (!(P1IN & BIT4))
    ;
P1IFG &= ~BIT4;
}
}
int main(void)
{
WDTCTL = WDTPW | WDTHOLD;
Configura_Clock();
Configura_TimerA();
Configura_Botao();
Configura_PWMVentiladorLed();
__bis_SR_register(GIE);
while (1)
{
__bis_SR_register(LPM0_bits);
ConfiguraPWM();
__no_operation();
__no_operation();
}
}