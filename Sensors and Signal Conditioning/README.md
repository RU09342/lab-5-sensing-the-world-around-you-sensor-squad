# Sensors and Signal Conditioning
* Created by Ryan Hare & Jessica Wozniak
* Created on 10/31/17
* Last Updated: 11/15/17

## Overview 

## Photoresistor: Resistance
The easiest way to find the resitance of the photoresistor is by using a voltage divider. Using the ADC, Vout can be found and then be used in the 
voltge divider equation to find the resistance in the photoresistor. The ADC reads from the between two resistances. Referencing the equation below, 
R2 will be conatsnt (1000), Vin will be constant(3.3V), and based off the value of ADC (Vout), the resistance can be found for the photoresistor(R1). 
When there is more light the resistance is lower and when there is less light the resistance is higher.  


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/Voltage%20Divider.png)

### Schematic 


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/Photoresistor.PNG)
### Hardware


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/photoresistor.jpg) 

## Phototransistor: Current 
The Phototransistor used was an OP805SL. In reference to the schematic below, the reason the ADC value changes with light is because the phototransistor lets more current 
pass when more light shines on it, or less current pass with less light. Also the use of a smaller resistor is less sensitive to light, whereas 
if a higher value resistor is used, the photoresistor is more sensitive to light.

### Schematic 
![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/Phototransistor.PNG)
### Hardware

## Temperature Sensor: Voltage
The temperature sensor that was used was a LM35. The LM35 reads every 10mV as one degree C. The ADC was read directly at the output of the LM35. 
### Schematic 
![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/LM35_Schematic.PNG)
### Hardware

## Software

### ADC
For this lab ADC10 and ADC12 were implemented on seperate boards. The ADC10 was implemented on the MSP430G2553 and the ADC12 on the MSP430FR6989. 
To begin initilzation of the ADC, a pin must be set on which the ADC will be taken (P1.7 ADC10 & P1.4 ADC12).
```C
    P1SEL1 |= ADC12;                          // P1.4 for ADC
    P1SEL0 |= ADC12;
```
Other initializations that were needed for code to run properly were Timer Init, UART Init, and Clock Init.


In the ADC10 initilzation, control register 0-2 were set. CTL0 was set to to turn on/ enable ADC12, CTL1 was set to sample/ hold pulse mode, and 
CTL2 was set to set ADC to 12 bit resolution. Then the ADC interrupt was enabled and the input select was chosen to bit 4.
```C
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;	       	
    ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12IER0 |= ADC12IE0;                    // Enable ADC conversion complete interrupt
    ADC12MCTL0 |= ADC12INCH_4;                // A1 ADC input select
```
Within the Timer interrupt, CTL0 was then set to enable and sample ADC.
```C
	ADC12CTL0 |= ADC12SC | ADC12ENC;		  //ADC sample and enable
```
Then the ADC value was sent back to the MSP430, where it underwent calcuations to output wanted information. Within the ADC interrupt, 
the below calcuations were computed. 

As stated above the reference voltage was set to 3.3 V for ADC10 and 1.2V for ADC12. To find the value of each bit for ADC, 
the reference voltage is divided by 2^(n); where n is the ADC resolution (for this code 10 or 12). So to find the voltage, the value in the 
ADC memory register is multiplied by the either 0.0033 (ADC10) or 0.000293(ADC12). 

### Photoresistor
After voltage is found, Ohm's law can be used to find the resistance of the photoresistor.
```C
    in = ADC12MEM0;
    voltage = in * 0.000293;                      //converts ADC to voltage
    resistance=(3300.0/voltage) - 1000;           //Using ohms law we can find resistance
```
### Phototransistor
After voltage is found, Ohm's law can be used to find the current.
```C
    in = ADC12MEM0;
    voltage = in * 0.000293;                      //Takes in ADC value and converts it to voltage
    current= voltage / 1000;                      //Using ohms law we can find current
```
### Temperature Sensor
To take the ADC value and output a temperature some manipulation was done. The LM35 reads every 10mV as one degree C. After the volatge is found, it 
is then divided by 0.01 to find Temp C. After Temp C is found, a simple conversion is used to find Temp F. 
```C
  in = ADC12MEM0;
  voltage = in * 0.000293;              //Converts ADC to voltage. (Vref/2^12) = 0.000293 * ADC = voltage
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF=((9*(tempC))/5)+32;             //converts degrees C to degrees F
```
After calculations were complete, the final results were then send to UART (also in ADC interrupt).
```C
    while(!(UCA0IFG&UCTXIFG));        //wait for TX to clear
    UCA0TXBUF = current;              //send to TX
```