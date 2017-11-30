//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors
//Created: 10/17/17
//Last updated: 10/21/17

#include <msp430.h>

void ADCInit(void);         //ADC initialization function
void UARTInit(void);

unsigned int temp;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; //Disable HIGH Z mode
    TB0CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
    TB0CTL = TBSSEL_2 + MC_2 + ID_3; //Set the Clock_A control to:
                                    //1. TASSEL_2 which selects SMCLK, the internal 1MHz clock.
                                    //2. MC_2 which selects the continuous counting mode.
                                    //3. ID_3 which sets an internal 8x clock divider.

    P4SEL0 |= BIT2;          //Configure pin 4.2 for ADC channel 10.
    P4SEL1 |= BIT2;

    ADCInit();              //initialize ADC
    UARTInit();             //Initialize UART

    __enable_interrupt();           //Enable interrupts.

    while (1)
    {
        ADC12CTL0 |= ADC12SC;   // Start conversion
        __bis_SR_register(LPM0_bits + GIE);     //Enter LPM0 with interrupts
    }

  }

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void) {
    UCA0TXBUF = ADC12MEM0;                  //Write ADC value to UART.
}

//ADC ISR
#pragma vector=ADC12_B_VECTOR
__interrupt void ADC12ISR (void)
{

}

// Initialize ADC12_A
void ADCInit(void){
    REFCTL0 |= REFON + REFTCOFF + REFVSEL_0;  //Enable reference voltage at 1.2v
    ADC12CTL0 = ADC12SHT0_4 + ADC12ON;      //Initialize ADC12CTL0 with a 64 cycle sample time.
    ADC12MCTL0 = ADC12INCH_10 + ADC12VRSEL0_H;      //Set input channel 10 and use 1.2 volt reference.
    ADC12CTL1 = ADC12SHP;                   //Initialize ADC12CTL1 with sample and hold mode.
    ADC12IER0 = 0x01;                       //Enable ADC interrupts.
    ADC12CTL0 |= ADC12ENC;                  //Enable conversion.

}

void UARTInit(void){

    CSCTL0_H = CSKEY_H;                         // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
    CSCTL0_H = 0;                               // Lock CS registers

    P2SEL0 &= ~(BIT0 | BIT1);                   //Configure pin 2.0 to RXD
    P2SEL1 |= BIT0+BIT1;                        //Configure pin 2.1 to TXD

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
    UCA0BRW = 52;                               // 8000000/16/9600
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
}
