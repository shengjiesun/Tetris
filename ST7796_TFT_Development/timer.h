/* Timer.h  Timer0, Timer1 and Timer2 library
*  Created on : 25.06.2018
*  Created by : Max Sun
*
*/

#ifndef _TIMER_H_
#define _TIMER_H_

#define F_CPU 16000000UL

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define MICROSECONDS_PER_TIMER0_OVERFLOW ( ((64 * 256) * 1000L) / (F_CPU / 1000L) )

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

//Port B physical pin out on chip
#define PORTB_0 14
#define PORTB_1 15
#define PORTB_2 16
#define PORTB_3 17
#define PORTB_4 18
#define PORTB_5 19
//#define PORTB_6 9
//#define PORTB_7 10

//Port C physical pin out on chip
#define PORTC_0 23
#define PORTC_1 24
#define PORTC_2 25
#define PORTC_3 26
#define PORTC_4 27
#define PORTC_5 28
//#define PORTC_6 1
//#define PORTC_7 -> reserved

//Port D physical pin out on chip
#define PORTD_0 2
#define PORTD_1 3
#define PORTD_2 4
#define PORTD_3 5
#define PORTD_4 6
#define PORTD_5 11
#define PORTD_6 12
#define PORTD_7 13


//Function prototypes
void timer0_init(void);
void timer1_init(void);
void timer2_init(void);
void timer_init(void);
unsigned long millis(void);


//PWM hardware output control
void pwmSetDuty(uint8_t pin, int16_t dutyCycle);
























#endif