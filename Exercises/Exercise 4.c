int Flag = 0; // Declarei a flag, declarei para marcar quando que
apertou o botao int Contador = 0;
int contadorB = 1;
void Configura_Clock(void)
{
    PJSEL0 |= BIT4 | BIT5; // Essa linha mostra os pinos do
    microcontrolador que
        esta sendo direcionado pro clock
            PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_3 | DCORSEL;      // 8 MHz
    CSCTL2 = SELA_0 | SELS_3 | SELM_3; // Sempre SELA 0 e SELS 3 para
    o cristal e o
        dco funcionar
            CSCTL3 = DIVA_0 | DIVS_0 | DIVM_0; // Cristal sempre vai
    ser 32768 e esses
        underlines escolhem quanto esse valor vai ser dividio do
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
    TA0CCR0 = 1024; // Linha que vamos utilizar para a divisao da
    frequencia
        TA0CTL = TASSEL_1 + MC_2 + ID_3; // TASSEL ESCOLHE SE A GENTE VAI
    USAR O 32768(ACLK)ou o DCO(Secundario / Oq a gente
                                                configurou la no clock)
}
void ConfiguraLed(void)
{
    // D25 - P2BIT0
    // D15 - P3BIT7
    // D14 - P3BIT6
    // D13 - P3BIT5
    // D12 - P3BIT4
    // D11 - P3BIT3
    // D10 - P3BIT2
    // D9 - P3BIT1
    // D8 - P3BIT0
    P2DIR |= BIT0;  // Direção DIR
    P2OUT &= ~BIT0; // Nivel logico(Acesso ou apagado) essas duas
    linhas são pra uma
        lampada
            P3DIR |= BIT7; // Direção DIR
    P3OUT &= ~BIT7;
    P3DIR |= BIT6; // Direção DIR
    P3OUT &= ~BIT6;
    P3DIR |= BIT5; // Direção DIR
    P3OUT &= ~BIT5;
    P3DIR |= BIT4; // Direção DIR
    P3OUT &= ~BIT4;
    P3DIR |= BIT3; // Direção DIR
    P3OUT &= ~BIT3;
    P3DIR |= BIT2; // Direção DIR
    P3OUT &= ~BIT2;
    P3DIR |= BIT1; // Direção DIR
    P3OUT &= ~BIT1;
    P3DIR |= BIT0; // Direção DIR
    P3OUT &= ~BIT0;
}
void Configura_Botao(void)
{
    // P1BIT3 é o nosso botao
    P1DIR &= ~(BIT3); // Entrada
    P1IE |= BIT3;
    P1IES &= ~(BIT3);
    P1IFG = 0x00;
}
void ConfiguraVentilador(void)
{
    // P1BIT5 - é o ventilador
    P1DIR |= BIT5;  // vai ser uma saida(1)
    P1OUT &= ~BIT5; // O ventilador comecara desligado
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{ // A cada, no caso, 0,25 segundos o
    programa
        le esse trecho
            TA0CCR0 += 1024; // Mudar junto com o do configura timer
    if (Flag == 1)
    {
        P2OUT ^= BIT0; // O led D25 vai piscar a cada 0,25
        if (contador % 2 == 0)
        { // Verifica se é 500 ms
            P3OUT ^= BIT7;
        }
        if (contador % 4 == 0)
        { // Verifica se é 1000 ms
            P3OUT ^= BIT6;
        }
        if (contador % 8 == 0)
        { // Verifica se é 2000 ms
            P3OUT ^= BIT5;
        }
        if (contador % 16 == 0)
        {
            P3OUT ^= BIT4;
        }
        if (contador % 32 == 0)
        {
            P3OUT ^= BIT3;
        }
        if (contador % 64 == 0)
        {
            P3OUT ^= BIT2;
        }
        if (contador % 128 == 0)
        {
            P3OUT ^= BIT1;
        }
        if (contador % 256 == 0)
        {
            P3OUT ^= BIT0;
        }
        if (contador % 512 == 0)
        {
            P1OUT |= BIT5;
            contador = 0;
        }
        if (contador == 224)
        { // Verifica se o contador passou 224
            vezes(passar 224 vezes significa que passou 56 000 ms(224 * 250) ai desliga
                      o ventilador)
                P1OUT &= ~BIT5;
        }
        contador = contador + 1;
    }
    __bic_SR_register_on_exit(LPM0_bits);
}
#pragma vector = PORT1_VECTOR // Alterar de acordo com o numero do P do
botao
    __interrupt void
    PORT1(void)
{
    if (P1IFG & BIT3)
    { // Verifica se o botao estiver sendo apertado,
        se ele
            // estiver apertado entrara aqui
            Flag = 1;
        if (contadorB == 2)
        {
            Flag = 0 contador2 = 0;
        }
        contadorB = contadorB + 1;
        while (!(P1IN & BIT3))
            ;
        P1IFG &= ~BIT3;
    }
    int main()
    {
        WDTCTL = WDTPW | WDTHOLD;
        Configura_Clock();
        Configura_TimerA();
        ConfiguraLed();
        ConfiguraBuzzer();
        ConfiguraBotao();
        __bis_SR_register(GIE); // Habilita a Interrupção(A contagem do
timer)
while(1)
{
    __bis_SR_register(LPM0_bits); // Faz o programa "dormir"
    "portalzinho"(faz ir pro pragma)
        __no_operation();
    __no_operation();
}
    }