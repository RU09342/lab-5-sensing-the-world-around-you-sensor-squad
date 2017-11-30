#include <msp430.h> 
#include <LCDDriver.h>

void LCDInit();                //Function to initialize the LCD
void DisplayNumbers(unsigned long num);          //Function to take a long as an input and use the showChar function to display that number.
void TimerInit(void);      //Timer function
void ADC12Init(void);      //ADC10 function
void GPIOInit(void);       //GPIO function

#define ADC12 BIT7          //define ADC12 as BIT7
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT0            //define RXD as BIT0
#define TXD BIT1            //define TXD as BIT1

unsigned int in;
float voltage;
float tempC;
float tempF;
float tempAvg = 20;
int count = 1;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;       //Disable high impedance mode.

	LCDInit();                  //Led Init function call
	TimerInit();                 //Timer Init function call
	GPIOInit();                  //GPIO Init function call
    LCDCCTL0 |= LCDON;          //Turn on the LCD

    while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
      REFCTL0 |= REFVSEL_0 | REFON;             // Select internal ref = 1.2V
                                                 // Internal Reference ON
    ADC12Init();                  //ADC12 Function

    while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle
    __enable_interrupt();           //Enable interrupts
	
	while(1){
	    tempAvg = ((tempAvg * count) + tempC)/(count + 1); //Take an average value to allow the displayed value to stabilize.
	    count++;
		if (count == 100)
			count = 10;
	    DisplayNumbers(tempAvg);      //displays temperature on LCD
	}
}
void DisplayNumbers(unsigned long num)      //Function takes in an integer of up to 6 characters and displays it on the LCD.
{
    long temp = 0;
    char temp2 = '0';
    long i = 10;
    int n = 6;
    while(i <= 1000000)
    {
        temp = num % i;         //Modulus the input with i to isolate the least significant nonzero digit
        num = num - temp;       //Subtract the lsd from the remaining input number
        temp = temp/(i/10);     //Divide the least significant nonzero digit by i/10 to remove all the zeroes
        temp2 = '0' + temp;     //Set the char temp2 to '0' plus the value of temp, which sets it to the char that represents the number in temp
        showChar(temp2, n);     //Show that char in position n
        n--;                    //Decrement n
        i = i * 10;             //Multiply i by 10
    }
}

void LCDInit()                 //Function to initialize the LCD.
{
    LCDCPCTL0 = 0xFFFF;         //Initialize the LCD segments.
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;
                                //Initialize LCD_C with ACLK, a divider of 1, a pre-divider of 16, and a 4-pin MUX.

    LCDCVCTL = VLCD_2 | VLCDREF_0 | LCDCPEN;
                                //Set the internal LCD reference voltage. Select internal reference and enable charge pump.

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enable
    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
}
//ADC ISR
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    in = ADC12MEM0;
    voltage = in * 0.000293;        //converts ADC to voltage
    tempC= voltage / 0.01;           //converts voltage to Temp C
    tempF=((9*tempC)/5)+32;             //Temp C to Temp F
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
    TA0CCR0 = 4096-1;                           // PWM Period
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

