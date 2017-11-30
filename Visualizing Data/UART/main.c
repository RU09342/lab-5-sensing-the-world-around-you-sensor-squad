#include <msp430.h> 

void TimerInit(void);      //Timer function
void ADC12Init(void);      //ADC10 function
void GPIOInit(void);       //GPIO function
void UARTInit(void);       //UART function

#define ADC12 BIT7          //define ADC12 as BIT7
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT0            //define RXD as BIT0
#define TXD BIT1            //define TXD as BIT1

unsigned int in;
float voltage;
float tempC;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;       //Disable high impedance mode.

    UARTInit();
    TimerInit();                 //Timer Init function call
    GPIOInit();                  //GPIO Init function call

    while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
      REFCTL0 |= REFVSEL_0 | REFON;             // Select internal ref = 1.2V
                                                 // Internal Reference ON
    ADC12Init();                  //ADC12 Function

    while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle
    __enable_interrupt();           //Enable interrupts

}
#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = tempC;
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;

        default:
                break;

    }
}
//ADC ISR
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    in = ADC12MEM0;
    voltage = in * 0.000293;        //converts ADC to voltage
    tempC= voltage / 0.01;           //converts voltage to Temp C
}

//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ADC12CTL0 |= ADC12SC | ADC12ENC;         //sample/ enable
}

//ADC Initialization
void ADC12Init()
{
    ADC12CTL0 = ADC12SHT0_2 + ADC12ON;
    ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
    ADC12MCTL0 |= ADC12INCH_4 | ADC12VRSEL_1; // A1 ADC input select, Vref = 1.2V
    P1OUT = LED1;                             // LED1 on
}
// Timer Initialization
void TimerInit()
{
    TA0CCTL0 = CCIE;                           // Enable interrupt
    TA0CCR0 = 2048-1;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TA0CCR1 = 256;                             // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_1 + MC_1 + ID_3;                   // ACLK, CONT MODE
}
void GPIOInit(void)
{
    P1OUT &= ~LED1;                           // Clear LED to start
    P1DIR |= LED1;                            // P1.0 output
    P8SEL1 |= ADC12;                          // P8.7 for ADC
    P8SEL0 |= ADC12;
}

//UART Initialization
void UARTInit(void)
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
