/*
 * Buttons.c
 *
 * Created: 12/9/2019 11:10:12 PM
 *  Author: sunsh
 */ 

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include "ADC.h"

void Button_init(void)
{
  ADC_init();
  DDRF &= ~(1<<PF2);
  PORTF |= (1 << PF2);
}

uint16_t readButtonX(void)
{
  return ADC_read(0);
}

uint16_t readButtonY(void)
{
  return ADC_read(1);
}

uint8_t readButtonSwitch(void)
{
  if (PINF & (1<<PF2)) return 1;
  else return 0;
}