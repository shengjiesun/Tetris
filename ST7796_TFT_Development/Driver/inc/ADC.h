/* Atmega328p ADC library
*  Created on : 24.06.2018
*  Created by : Max Sun
*/



#ifndef ADC_H
#define ADC_H
#endif



#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

void ADC_init(void);
uint16_t ADC_read(uint8_t pin);
void ADC_delay(void);
