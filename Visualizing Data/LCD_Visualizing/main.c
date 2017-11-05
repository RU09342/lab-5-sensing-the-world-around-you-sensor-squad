#include <msp430.h> 
#include <LCDDriver.h>

void LCD_Init();                //Function to initialize the LCD
void DisplayNumbers(unsigned long num);          //Function to take a long as an input and use the showChar function to display that number.

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;       //Disable high impedance mode.

	LCD_Init();
    LCDCCTL0 |= LCDON;          //Turn on the LCD
	
	return 0;
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

void LCD_Init()                 //Function to initialize the LCD.
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
