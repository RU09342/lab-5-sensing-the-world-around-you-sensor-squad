//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors
//Created: 10/17/17
//Last updated: 10/21/17

#include <msp430.h>

#define RED BIT4;           //define RED LED to P1.4
#define BLUE BIT5;          //define BLUE LED to P1.5

void LEDInit(void);         //LED initialization function
void TimerBInit(void);      //TIMERB initialization function
void ADCInit(void);         //ADC initialization function

#define CALADC12_12V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C
#define CALADC12_12V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C

unsigned int temp;
volatile float tempC;
volatile float tempF;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

    LEDInit();              //initialize LED
    TimerBInit();           //initialize TIMERB
    ADCInit();              //initialize ADC

    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 + REFON;           // Enable internal 1.2V reference

    PM5CTL0 &= ~LOCKLPM5; //Disable HIGH Z mode

    while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator
                                            // to settle
    ADC12CTL0 |= ADC12ENC;

    while(1)
    {
        ADC12CTL0 |= ADC12SC;               // Sampling and conversion start

        __bis_SR_register(LPM0_bits + GIE); // LPM4 with interrupts enabled
        __no_operation();

        // Temperature in Celsius
        tempC = (float)(((float)temp - CALADC12_12V_30C) * (85 - 30)) / (CALADC12_12V_85C - CALADC12_12V_30C) + 30.0f;

        // Temperature in Fahrenheit: Tf = (9/5)*Tc + 32
        tempF = tempC * 9.0f / 5.0f + 32.0f;

        if (tempF > 70)           //if temperature is greater than 70 degrees
            {
            TB0CCR1 = 0;          //Red
            }
        else if (tempF < 50)      //if temperature is lower than 50 degrees
            {
            TB0CCR2 = 0;          //Blue
            }
            else
            {
            //purple when in between 50 and 70
            TB0CCR1 = 127;      // 1/2 red
            TB0CCR2 = 128;      // 1/2 blue
            }

        __no_operation();                   // SET BREAKPOINT HERE
    }
  }

//ADC ISR
#pragma vector=ADC12_B_VECTOR
__interrupt void ADC12ISR (void)

{
    switch(__even_in_range(ADC12IV, ADC12IV__ADC12RDYIFG))
    {
        case ADC12IV__ADC12IFG0:            // Vector 12:  ADC12MEM0 Interrupt
            temp = ADC12MEM0;               // Move results, IFG is cleared
            __bic_SR_register_on_exit(LPM4_bits); // Exit active CPU
            break;
        default: break;
    }
}
//Initialize RGB LED
void LEDInit(void) {
    //For pin 1.4, 1.5, and 3.4, P1DIR = 1, P1SEL0 = 1, P1SEL1 = 0.
    P1DIR |= RED; //Pin 1.4
    P1SEL1 &= ~RED;
    P1SEL0 |= RED;

    P1DIR |= BLUE; //Pin 1.5
    P1SEL1 &= ~BLUE;
    P1SEL0 |= BLUE;
}
//Initialize TimerB
void TimerBInit(void) {
    TB0CCTL1 = OUTMOD_3; //Set OUTMOD_3 (set/reset) for CCR1, CCR2
    TB0CCTL2 = OUTMOD_3;

    //Set initial values for CCR1, CCR2
    TB0CCR1 = 0; //Red
    TB0CCR2 = 255; //Blue

    TB0CCR0 = 255; //Set CCR0 for a ~1kHz clock.

    TB0CTL = TBSSEL_2 + MC_1; //Enable Timer B0 with SMCLK and up mode.
}
// Initialize ADC12_A
void ADCInit(void){
    ADC12CTL0 &= ~ADC12ENC;                 // Disable ADC12
    ADC12CTL0 = ADC12SHT0_8 + ADC12ON;      // Set sample time
    ADC12CTL1 = ADC12SHP;                   // Enable sample timer
    ADC12CTL3 = ADC12TCMAP;                 // Enable internal temperature sensor
    ADC12MCTL0 = ADC12VRSEL_1 + ADC12INCH_30; // ADC input ch A30 => temp sense
    ADC12IER0 = BIT0;                      // ADC_IFG upon conv result-ADCMEMO

}
