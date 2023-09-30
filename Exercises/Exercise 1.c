int Flag1 = 0; // Oq é isso? Serve para avisar se o botao foi apertado ou nao
int Flag2 = 0;
int Flag3 = 0;
int Flag4 = 0;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5; // Pinos do Oscilador
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_3 | DCORSEL;      // Configura DCO: 1MHz - 24 MHz
    CSCTL2 = SELA_0 | SELS_3 | SELM_3; // ACLK = XT1; SMCLK e MCLK = DCO
    CSCTL3 = DIVA_0 | DIVS_0 | DIVM_3; // Divisão do Clock: 1 - 32
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
    TA0CCR0 = 4096; // Escolher valor para o tempo
    // TA0EX0 = TAIDEX_1;
    TA0CTL = TASSEL_1 + MC_2 + ID_3; // Escolhe a fonte de clock(TASSEL) e quanto
    ele sera dividido(ID)
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCR0 += 4096; // Muda o valor dependendo com qual o professor pede
    if (((Flag1) + (Flag2) + (Flag3) + (Flag4)) % 2 == 0)
    {
        // Pra que? Verificar se dois botoes estao sendo apertados ao mesmo tempo
        //  Como? Se a soma for par, quer dizer que no minimo dois botoes estao
sendo apertados, se for impar apenas 1
if( (Flag1) + (Flag2) + (Flag3) + (Flag4) ) != 0 )
{
    // Verifica se nenhum botao foi apertado
    P4OUT |= BIT1;
}
    }
    else
    {
P4OUT &= ~BIT1;
    }
    Flag1 = 0;
    Flag2 = 0;
    Flag3 = 0;
    Flag4 = 0;
    __bic_SR_register_on_exit(LPM3_bits);
}
void ConfiguraLed(void)
{
    P2DIR |= BIT1;
    P2OUT &= ~BIT1;
    P2DIR |= BIT2;
    P2OUT &= ~BIT2;
    P2DIR |= BIT3;
    P2OUT &= ~BIT3;
    P2DIR |= BIT4;
    P2OUT &= ~BIT4;
}
void ConfiguraBotao(void)
{
    P3DIR &= ~(BIT3 | BIT4 | BIT5 | BIT6); // Só precisa escolher os pinos
    certos
        P3IE |= BIT3 | BIT4 | BIT5 | BIT6;
    P3IES &= ~(BIT3 | BIT4 | BIT5 | BIT6);
    P3IFG = 0x00;
}
#pragma vector = PORT3_VECTOR // Alterar de acordo com o numero do P do botao
__interrupt void PORT2(void)
{
    if (P3IFG & BIT3)
    {
if ((Flag1 + Flag2 + Flag3 + Flag4) == 0)
{ // Verifica algum botao foi
    pressionado
        antes Por que isso
        ? (Requisto de ex)
              P2OUT ^= BIT1; // é o led
    P2OUT &= ~BIT2;          // o resto dos leds desligados
    P2OUT &= ~BIT3;
    P2OUT &= ~BIT4;
}
Flag1 = 1; // Variavel para avisar se entrou no botao
while (!(P3IN & BIT3))
    ;
P3IFG &= ~BIT3;
    }
    if (P3IFG & BIT4)
    {
if ((Flag1 + Flag2 + Flag3 + Flag4) == 0)
{
    P2OUT &= ~BIT1; // é o led
    P2OUT ^= BIT2;  // o resto dos leds desligados
    P2OUT &= ~BIT3;
    P2OUT &= ~BIT4;
}
Flag2 = 1; // Variavel para avisar se entrou no botao
while (!(P3IN & BIT4))
    ;
P3IFG &= ~BIT4;
    }
    if (P3IFG & BIT5)
    {
if ((Flag1 + Flag2 + Flag3 + Flag4) == 0)
{
    P2OUT &= ~BIT1; // é o led
    P2OUT &= ~BIT2; // o resto dos leds desligados
    P2OUT ^= BIT3;
    P2OUT &= ~BIT4;
}
Flag3 = 1; // Variavel para avisar se entrou no botao
while (!(P3IN & BIT5))
    ;
P2IFG &= ~BIT5;
    }
    if (P3IFG & BIT6)
    {
if ((Flag1 + Flag2 + Flag3 + Flag4) == 0)
{
    P2OUT &= ~BIT1; // é o led
    P2OUT &= ~BIT2; // o resto dos leds desligados
    P2OUT &= ~BIT3;
    P2OUT ^= BIT4;
}
Flag4 = 1; // Variavel para avisar se entrou no botao
while (!(P3IN & BIT6))
    ;
P2IFG &= ~BIT5;
    }
}
void ConfiguraBuzzer(void)
{
    P4DIR |= BIT1;
    P4OUT &= ~BIT1;
}
int main()
{
    WDTCTL = WDTPW | WDTHOLD;
    Configura_Clock();
    Configura_TimerA();
    ConfiguraLed();
    ConfiguraBuzzer();
    ConfiguraBotao();
    __bis_SR_register(GIE); // Habilita a Interrupção(A contagem do timer)
    while (1)
    {
__bis_SR_register(LPM0_bits); // Faz o programa "dormir"
"portalzinho"(faz ir pro pragma)
    __no_operation();
__no_operation();
    }
}