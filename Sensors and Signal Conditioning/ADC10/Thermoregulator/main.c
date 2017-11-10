//Jessica Wozniak
//Lab 5 Sensors: ADC10 MSP430G2553- Temperature Sensor
//Created: 11/7/17
//Last updated: 11/9/17

#include <msp430.h>

#define ADC10 BIT7
#define LED1 BIT0
#define RXD BIT1
#define TXD BIT2

void TimerInit(void);
void ADC10Init(void);
void UARTInit(void);

//unsigned int tempC = 0
long tempF = 0;
long in = 0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  ADC10Init();                  //ADC10 Function
  TimerInit();                  //Timer Function
  UARTInit();                   //UART Function

  __bis_SR_register(GIE);       //interrupt enable
    while(1){
    }
}
//ADC ISR
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  ADC10CTL0 &= ~ENC;                        // ADC10 disabled
  ADC10CTL0 = 0;                            // ADC10, Vref disabled
  in = ADC10MEM;
//  tempC = in /100;
//  tempF=9*tempC/5+32;
  tempF = ((in - 630) * 761) / 1024;        //Conversion from ADC10 sample to temp F

  UCA0TXBUF = tempF;                        //send TempF to TX
}
//Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;    // from TI exmaple code
  ADC10CTL0 |= ENC;                         // ADC10 enable set
}
//ADC Initialization
void ADC10Init()
{
      ADC10CTL1 = INCH_7 + SHS_1;             // P1.7, TA1 trigger sample start
      ADC10AE0 = ADC10;                                     // P1.7 ADC10 option select
      P1DIR |= LED1;                                        // set LED1 to output
}
// Timer Initialization
void TimerInit()
{
    TACCTL0 = CCIE;                           // Enable interrupt
    TACCR0 = 4096-1;                          // PWM Period
    TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TACCR1 = 256;                             // TACCR1 PWM Duty Cycle
    TACTL = TASSEL_1 + MC_1 + ID_3;           // ACLK, UP MODE, DIV 4
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

      DCOCTL = 0;                             // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
      DCOCTL = CALDCO_1MHZ;

      UCA0CTL1 |= UCSSEL_2;                   // SMCLK
      UCA0BR0 = 104;                          // 9600 baud
      UCA0BR1 = 0;                            // 9600 baud
      UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
}