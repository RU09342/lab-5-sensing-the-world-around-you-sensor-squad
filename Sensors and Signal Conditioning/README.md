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

## Software

### ADC
For this lab ADC10 and ADC12 were implemented on seperate boards. The ADC10 was implemented on the MSP430G2553 and the ADC12 on the MSP430FR6989. To begin initilzation of the ADC, a pin must be set on which the ADC will be taken (P1.7 ADC10 & P1.4 ADC12). In the ADC10 code Then the ADC signal is sent back to the MSP430. 
###Photoresistor
```C
    in = ADC12MEM0;
    voltage = in * 0.0033;                      //converts ADC to voltage
    resistance=(3300.0/voltage) - 1000;         //Using ohms law we can find resistance
```
###Phototransistor
```C
    in = ADC10MEM;
    voltage = in * 0.0033;                      //Takes in ADC value and converts it to voltage
    current= voltage / 1000;                    //Using ohms law we can find current
```
###Temperature Sensor
To take the ADC value and output a temperature some manipulation was done. The LM35 reads every 10mV as one degree C. As stated above the reference voltage was set to 3.3 V for ADC10 and 1.2V for ADC12. To find the value of each bit for ADC, the reference voltage is divided by 2^(n); where n is the ADC resolution (for this code 10 or 12). 
```C
  in = ADC10MEM;
  voltage = in * 0.0033;                //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF=((9*(tempC))/5)+32;             //converts degrees C to degrees F
```

