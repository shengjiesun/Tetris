/* Atmega328p ADC library ADC.c
*  Created on : 24.06.2018
*  Created by : Max Sun
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include "ADC.h"
#include <avr/delay.h>

//Initialize these two variable as 255 since no ADC pin can be 255
uint8_t pin_new = 0xff;
uint8_t pin_old = 0xff;

void ADC_init(void)
{
	ADMUX = (1<<REFS0); //use AVcc with external capacitor at AREF pin
	ADCSRA = (1<<ADEN) | (1<<ADATE) | (7<<ADPS0); //enable ADC, single conversion, prescaler 128 -> (16Mhz->125khz)
	ADCSRB = 0x00; //free running mode
	ADCSRA |= (1 << ADSC); //Start conversion
}


uint16_t ADC_read(uint8_t pin)
{
	pin_new = pin;
	//Check if the ADC channel is changed, if sample channel is changed, 
	//need at least 250us delay for Multiplexer to settle;
	if (pin_new != pin_old) 
	{
		switch (pin)
		{
			case 0: ADMUX = 0x40; //ADMUX = 0b01000000 -> ADC0
			DIDR0 |= (1<<ADC0D); //turn off input buffer, to reduce power consumption
			break;
			case 1: ADMUX = 0x41; //ADMUX = 0b01000001 -> ADC1
			DIDR0 |= (1<<ADC1D); //turn off input buffer, to reduce power consumption
			break;
			case 2: ADMUX = 0x42; //ADMUX = 0b010000010 -> ADC2
			DIDR0 |= (1<<ADC1D); //turn off input buffer, to reduce power consumption
			break;
			case 3: ADMUX = 0x43; //ADMUX = 0b010000011 -> ADC3
			DIDR0 |= (1<<ADC2D); //turn off input buffer, to reduce power consumption
			break;
			case 4: ADMUX = 0x44; //ADMUX = 0b01000100 -> ADC4
			DIDR0 |= (1<<ADC3D); //turn off input buffer, to reduce power consumption
			break;
			case 5: ADMUX = 0x45; //ADMUX = 0b010000101 -> ADC5
			DIDR0 |= (1<<ADC4D); //turn off input buffer, to reduce power consumption
			break;
			default: break;
		}
		_delay_us(300);
		pin_old = pin_new;
	}
	
	uint16_t ADC_value = ADCL ;
	ADC_value += (ADCH<<8);
	return ADC_value;
}

//need to use delay when switching sampling channel, need to allow 
//enough time to discharge the circuitry to obtain accurate value
void ADC_delay(void)
{
	_delay_us(550);
}



