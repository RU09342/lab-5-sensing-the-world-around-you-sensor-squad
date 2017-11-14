# Sensors and Signal Conditioning
* Created by Ryan Hare & Jessica Wozniak
* Created on 10/31/17
* Last Updated: 11/9/17

## Overview 

## Photoresistor: Resistance
The easiest way to find the resitance of the photoresistor is by using a voltage divider. Using the ADC, Vout can be found and then be used in the voltge divider equation to find the resistance in the photoresistor. The ADC reads from the between two resistances. Referencing the equation below, R2 will be conatsnt (1000), Vin will be constant(3.3V), and based off the value of ADC (Vout), the resistance can be found for the photoresistor(R1). 


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/Voltage%20Divider.png)

### Schematic 


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/Photoresistor.PNG)
### Hardware


![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-sensor-squad/blob/master/Photos/photoresistor.jpg) 

## Phototransistor: Current 
### Schematic 
### Hardware

## Temperature Sensor: Voltage
### Schematic 
### Hardware
## Similarities in ADC10 and ADC12

## Differences in ADC10 and ADC12
### ADC10
The MSP430G2553 was used to implement ADC10.
``` C
#include <msp430.h>

#define ADC10 BIT7          //define ADC10 as BIT7
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT1            //define RXD as BIT1
#define TXD BIT2            //define TXD as BIT2

void TimerInit(void);      //Timer function
void ADC10Init(void);      //ADC10 function
void UARTInit(void);       //UART function
void ClockInit(void);      //Clock function

unsigned int in = 0;
char ADCMSB = 0;          //ADC10 Most significant bits
char ADCLSB = 0;          //ADC10 Least significant bits
```
### ADC12
The MSP430FR6989 was used to implement ADC12.


