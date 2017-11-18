# Visualizing Data
* Created by Ryan Hare & Jessica Wozniak
* Created on 10/31/17
* Last Updated: 11/16/17

## Overview 
Using the ADC12 code implemented in the "Sensors and Signal Conditioning" part of this lab, differnt types of data visualizations were possible.
## RGB LED (MSP430FR5994)
### Hardware
### Software
Some minor additions to the ADC12 code were needed, including a function LEDChange() that changes the color of the LED based on the temperature sensed
 by the LM35. If the temperature is above 75 F, the LED will be red and if the temperature is below 50F, the LED will be blue. If the temperature is in 
 between 50 - 75 F, the LED will be purple. 
```C
void LEDChange()
{
    if (tempF > 75)           //if temperature is greater than 75 degrees F
        {
          TB0CCR1 = 0;          //Red
        }
    else if (tempF < 50)      //if temperature is lower than 50 degrees F
        {
          TB0CCR2 = 0;          //Blue
        }
    else
        {
         //purple when in between 50 and 75 F
          TB0CCR1 = 127;      // 1/2 red
          TB0CCR2 = 128;      // 1/2 blue
        }
}
```
## LCD Display
### Software
With the addition of a LCDInit() function, a DisplayNumbers() function, and including the LCDDriver.h file,  it was simple to intergrate the two codes 
together. 
```C
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
```
The DisplayNumbers() function declares 4 variables (temp, temp2, i, and n). While i < 1000000 
```C
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
```
After the the ADC returns a value for tempF, tempF is displayed on the LCD display. 
![Alt Text] (Add link to video of this working)