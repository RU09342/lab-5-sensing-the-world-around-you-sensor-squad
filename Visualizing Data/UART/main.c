//Jessica Wozniak & Ryan Hare
//Lab 5: UART Visualization
//Created: 11/21/17
//Last updated: 11/22/17

#include <msp430.h> 

void TimerInit(void);      //Timer initialize
void ADC12Init(void);      //ADC10 initialize
void GPIOInit(void);       //GPIO initialize
void UARTInit(void);       //UART initialize

#define ADC12 BIT4          //define ADC12 as BIT4
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT0            //define RXD as BIT0
#define TXD BIT1            //define TXD as BIT1

unsigned int in, time;
float voltage;
float tempC;
float tempF;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;       //Disable high impedance mode.

    UARTInit();
    TimerInit();                 //Timer Init function call
    GPIOInit();                  //GPIO Init function call

    while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
      REFCTL0 |= REFVSEL_0 | REFON;             // Select internal ref = 2.5V
                                                 // Internal Reference ON
      ADC12Init();                  //ADC10 Function

      while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle


    while(1){
    }
}
//ADC ISR
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    in = ADC12MEM0;
    voltage = in * 0.000293;        //converts ADC to voltage
    tempC= voltage/ 0.01;           //converts voltage to Temp C
    tempF=((9*tempC)/5)+32;         //Temp C to Temp F
}

//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ADC12CTL0 |= ADC12SC | ADC12ENC;        //sample/ enable
    time = (time + 1) % 8;                  //Cycle the 'time' variable up to 8
    if (time == 0)                          //Whenever 'time' cycles to zero...
        UCA0TXBUF=tempF;                    //...output 'tempF' over UART
}

//ADC Initialization
void ADC12Init()
{
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
    ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
    ADC12MCTL0 |= ADC12INCH_4 | ADC12VRSEL_0; // A1 ADC input select, Vref = 2.5V
    P1OUT = LED1;                             // LED1 on
}

// Timer Initialization
void TimerInit()
{
    TA0CCTL0 = CCIE;                           // Enable interrupt
    TA0CCR0 = 4096-1;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TA0CCR1 = 256;                             // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_1 + MC_1 + ID_3;                   // ACLK, CONT MODE
}

//GPIO Initialization
void GPIOInit(void)
{
    P1OUT &= ~LED1;                           // Clear LED to start
    P1DIR |= LED1;                            // P1.0 output
    P1SEL1 |= ADC12;                          // P1.4 for ADC
    P1SEL0 |= ADC12;
}

//UART Initialization
void UARTInit(void){

        CSCTL0_H = CSKEY_H;                         // Unlock CS registers
        CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
        CSCTL0_H = 0;                               // Lock CS registers

        P2SEL0 &= ~(BIT0 | BIT1);
        P2SEL1 |= BIT0+BIT1;

        // Configure USCI_A0 for UART mode
        UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
        UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
        UCA0BRW = 52;                               // 8000000/16/9600
        UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
        UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
        UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
