<<<<<<< HEAD
<<<<<<< HEAD
//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors: ADC10 MSP430G2553- Temperature Sensor
//Created: 11/7/17
//Last updated: 11/15/17

#include <msp430.h>

#define ADC10 BIT7
#define LED1 BIT0
#define RXD BIT1
#define TXD BIT2

void TimerInit(void);
void ADC10Init(void);
void UARTInit(void);
void ClockInit(void);

unsigned int in = 0;
float tempC = 0;
float tempF = 0;
float voltage =0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  ADC10Init();                  //ADC10 Function call
  TimerInit();                  //Timer Function call
  UARTInit();                   //UART Function call
  ClockInit();                  //Clock Function call

  __bis_SR_register(GIE);       //interrupt enable
    while(1){
    }
}
//ADC ISR
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  in = ADC10MEM;
  voltage = in * 0.0033;                //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF=((9*(tempC))/5)+32;             //converts degrees C to degrees F

  while(!(IFG2 & UCA0TXIFG));          //waits for tx to be clear
  UCA0TXBUF = tempF;                   //send to TX
}
//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
  ADC10CTL0 = SREF_0 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
 // VR+ = AVCC+ and VR- = AVSS, 16 x ADC10CLKs, ADC10 Reference on, ADC10 On/Enable, ADC10 Interrupt Enable : from TI exmaple code
  ADC10CTL0 |= ENC;                         // ADC10 enable set
}
//ADC Initialization
void ADC10Init()
{
      ADC10CTL1 = INCH_7 + SHS_1;             // P1.7, TA1 trigger sample start
      ADC10AE0 = ADC10;                       // ADC10 on P1.7
      P1DIR |= LED1;                          // set LED1 to output
}
// Timer Initialization
void TimerInit()
{
    TACCTL0 = CCIE;                           // Enable interrupt
    TACCR0 = 400;                             // PWM Period
    TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TACCR1 = 300;                             // TACCR1 PWM Duty Cycle
    TACTL = TASSEL_1 + MC_1;                  // ACLK, UP MODE, DIV 4
}
//UART Initialization
void UARTInit()
{
    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);
      }

      P1SEL = RXD + TXD;                    // P1.1 = RXD, P1.2=TXD
      P1SEL2 = RXD + TXD;                  // P1.1 = RXD, P1.2=TXD

      UCA0CTL1 |= UCSSEL_2;                   // SMCLK
      UCA0BR0 = 104;                          // 9600 baud
      UCA0BR1 = 0;                            // 9600 baud
      UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
}
void ClockInit()
{
    DCOCTL = 0;                             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
    DCOCTL = CALDCO_1MHZ;
}
=======
//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors: ADC10 MSP430G2553- Temperature Sensor
//Created: 11/7/17
//Last updated: 11/15/17

#include <msp430.h>

#define ADC10 BIT7
#define LED1 BIT0
#define RXD BIT1
#define TXD BIT2

void TimerInit(void);
void ADC10Init(void);
void UARTInit(void);
void ClockInit(void);

unsigned int in = 0;
float tempC = 0;
float tempF = 0;
float voltage =0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  ADC10Init();                  //ADC10 Function call
  TimerInit();                  //Timer Function call
  UARTInit();                   //UART Function call
  ClockInit();                  //Clock Function call

  __bis_SR_register(GIE);       //interrupt enable
    while(1){
    }
}
//ADC ISR
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  in = ADC10MEM;
  voltage = in * 0.0033;                //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF=((9*(tempC))/5)+32;             //converts degrees C to degrees F

  while(!(IFG2 & UCA0TXIFG));          //waits for tx to be clear
  UCA0TXBUF = tempF;                   //send to TX
}
//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
  ADC10CTL0 = SREF_0 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
 // VR+ = AVCC+ and VR- = AVSS, 16 x ADC10CLKs, ADC10 Reference on, ADC10 On/Enable, ADC10 Interrupt Enable : from TI exmaple code
  ADC10CTL0 |= ENC;                         // ADC10 enable set
}
//ADC Initialization
void ADC10Init()
{
      ADC10CTL1 = INCH_7 + SHS_1;             // P1.7, TA1 trigger sample start
      ADC10AE0 = ADC10;                       // ADC10 on P1.7
      P1DIR |= LED1;                          // set LED1 to output
}
// Timer Initialization
void TimerInit()
{
    TACCTL0 = CCIE;                           // Enable interrupt
    TACCR0 = 400;                             // PWM Period
    TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TACCR1 = 300;                             // TACCR1 PWM Duty Cycle
    TACTL = TASSEL_1 + MC_1;                  // ACLK, UP MODE, DIV 4
}
//UART Initialization
void UARTInit()
{
    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);
      }

      P1SEL = RXD + TXD;                    // P1.1 = RXD, P1.2=TXD
      P1SEL2 = RXD + TXD;                  // P1.1 = RXD, P1.2=TXD

      UCA0CTL1 |= UCSSEL_2;                   // SMCLK
      UCA0BR0 = 104;                          // 9600 baud
      UCA0BR1 = 0;                            // 9600 baud
      UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
}
void ClockInit()
{
    DCOCTL = 0;                             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
    DCOCTL = CALDCO_1MHZ;
}
>>>>>>> refs/remotes/origin/master
=======
//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors: ADC10 MSP430G2553- Temperature Sensor
//Created: 11/7/17
//Last updated: 11/15/17

#include <msp430.h>

#define ADC10 BIT7
#define LED1 BIT0
#define RXD BIT1
#define TXD BIT2

void TimerInit(void);
void ADC10Init(void);
void UARTInit(void);
void ClockInit(void);

unsigned int in = 0;
float tempC = 0;
float tempF = 0;
float voltage =0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  ADC10Init();                  //ADC10 Function call
  TimerInit();                  //Timer Function call
  UARTInit();                   //UART Function call
  ClockInit();                  //Clock Function call

  __bis_SR_register(GIE);       //interrupt enable
    while(1){
    }
}
//ADC ISR
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  in = ADC10MEM;
  voltage = in * 0.0033;                //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF=((9*(tempC))/5)+32;             //converts degrees C to degrees F

  while(!(IFG2 & UCA0TXIFG));          //waits for tx to be clear
  UCA0TXBUF = tempF;                   //send to TX
}
//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
  ADC10CTL0 = SREF_0 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
 // VR+ = AVCC+ and VR- = AVSS, 16 x ADC10CLKs, ADC10 Reference on, ADC10 On/Enable, ADC10 Interrupt Enable : from TI exmaple code
  ADC10CTL0 |= ENC;                         // ADC10 enable set
}
//ADC Initialization
void ADC10Init()
{
      ADC10CTL1 = INCH_7 + SHS_1;             // P1.7, TA1 trigger sample start
      ADC10AE0 = ADC10;                       // ADC10 on P1.7
      P1DIR |= LED1;                          // set LED1 to output
}
// Timer Initialization
void TimerInit()
{
    TACCTL0 = CCIE;                           // Enable interrupt
    TACCR0 = 400;                             // PWM Period
    TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TACCR1 = 300;                             // TACCR1 PWM Duty Cycle
    TACTL = TASSEL_1 + MC_1;                  // ACLK, UP MODE, DIV 4
}
//UART Initialization
void UARTInit()
{
    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);
      }

      P1SEL = RXD + TXD;                    // P1.1 = RXD, P1.2=TXD
      P1SEL2 = RXD + TXD;                  // P1.1 = RXD, P1.2=TXD

      UCA0CTL1 |= UCSSEL_2;                   // SMCLK
      UCA0BR0 = 104;                          // 9600 baud
      UCA0BR1 = 0;                            // 9600 baud
      UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
}
void ClockInit()
{
    DCOCTL = 0;                             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
    DCOCTL = CALDCO_1MHZ;
}
>>>>>>> refs/remotes/origin/master
