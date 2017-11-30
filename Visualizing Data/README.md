# Visualizing Data
* Created by Ryan Hare & Jessica Wozniak
* Created on 10/31/17
* Last Updated: 11/16/17

## Overview 
Using the ADC12 code implemented in the "Sensors and Signal Conditioning" part of this lab, different types of data visualizations were possible.
## RGB LED (MSP430FR5994)
### Hardware
This software is meant to be run with a common anode RGB LED. The LED circuit is shown below.
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
## LCD Display (MSP430FR6989)
### Software
With the addition of a LCDInit() function, a DisplayNumbers() function, and including the LCDDriver.h file, it was simple to intergrate the two codes 
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

## UART (MSP430FR6989)
### Requirements
For the UART visualization, the board is connected to a computer via UART. For the MSP430 boards, the USB cable will work.

The MATLAB Serial Toolbox will be used to plot and visualize the data once it has been transferred to the computer.
### Software
The same ADC code can be used for the UART visualization, however a UART initialize function will be needed. This will enable the microprocessor to output values over UART to the connected computer.

The UART will be initialized to have a baud rate of 9600. If a USB cable is not used, the UART will use P2.1 as the Receive pin and P2.0 as the Transmit pin.
```C
void UARTInit(void){

        CSCTL0_H = CSKEY_H;                         // Unlock CS registers
        CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
        CSCTL0_H = 0;                               // Lock CS registers

        P1SEL0 |= BIT0 + BIT1;						//Set P1SEL to 01 for p1.6 and p1.7
        P1SEL1 &= ~(BIT0 + BIT1);

        // Configure USCI_A0 for UART mode
        UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
        UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
        UCA0BRW = 52;                               // 8000000/16/9600
        UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
        UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
        UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
```
In order to actually plot the data on the computer, it is necessary to use MATLAB. The SerialDataPlot.m function was based on MathWorks code ([which can be found here](https://www.mathworks.com/matlabcentral/fileexchange/25519-collect-and-plot-data-from-an-instrument-in-real-time)). This function reads the input from the serial port and graphs the data after a minute of reading.

It may be necessary to configure the serial port based on how the microprocessor is connected. To do so, change the `COM12` in the `serialPort = 'COM12'` line to correctly reflect the COM port that your microprocessor is connected to. This can be found through device manager.
