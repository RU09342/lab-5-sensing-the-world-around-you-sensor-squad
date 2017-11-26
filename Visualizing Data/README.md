# Visualizing Data
* Created by Ryan Hare & Jessica Wozniak
* Created on 10/31/17
* Last Updated: 11/16/17

## Overview 
Using the ADC12 code implemented in the "Sensors and Signal Conditioning" part of this lab, different types of data visualizations were possible.
## RGB LED (MSP430FR5994)
### Hardware

### Software
Some minor additions to the ADC12 code were needed, including the following in the while(1) loop in the main that changes the color of 
the LED based on the temperature sensed by the LM35. If the temperature is above 90 F, the LED will be red and if the temperature is below 30F,
 the LED will be blue. If the temperature is anywhere in between the LED will be maroon, pink, dark purple, indigo, blue, torquoise, or teal. 
```C
    if(tempF > 90)
    {
        TB0CCR1 = 0xFF; //Red
        TB0CCR2 = 0x00; //Green
        TB0CCR3 = 0x00; //Blue
    }
    else if(tempF < 90 && tempF >=80 )
    {
        //Maroon C7046C
        TB0CCR1 = 0xC7; //Red
        TB0CCR2 = 0x04; //Green
        TB0CCR3 = 0x6C; //Blue
    }
    else if(tempF < 80 && tempF >=70 )
    {
        //Pink FF17FF
        TB0CCR1 = 0xFF; //Red
        TB0CCR2 = 0x17; //Green
        TB0CCR3 = 0xFF; //Blue
    }
    else if(tempF < 70 && tempF >=60 )
    {
        //Dark Purple 630AC9
        TB0CCR1 = 0x63; //Red
        TB0CCR2 = 0x0A; //Green
        TB0CCR3 = 0xC9; //Blue
    }
    else if(tempF < 60 && tempF >=50 )
    {
        //Indigo 465ADE
        TB0CCR1 = 0x46; //Red
        TB0CCR2 = 0x5A; //Green
        TB0CCR3 = 0xDE; //Blue
    }
    else if(tempF < 50 && tempF >=40 )
    {
        //Blue 0000FF
        TB0CCR1 = 0x00; //Red
        TB0CCR2 = 0x00; //Green
        TB0CCR3 = 0xFF; //Blue
    }
    else if(tempF < 40 && tempF >=30 )
    {
        //Torquiose 0890A5
        TB0CCR1 = 0x08; //Red
        TB0CCR2 = 0x90; //Green
        TB0CCR3 = 0xA5; //Blue
    }

    else if(tempF < 30 )
    {
        //Teal 0AFAEA
        TB0CCR1 = 0x0A; //Red
        TB0CCR2 = 0xFA; //Green
        TB0CCR3 = 0xEA; //Blue
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