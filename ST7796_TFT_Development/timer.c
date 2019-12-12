/* Timer.c  Timer0, Timer1 and Timer2 library
*  Created on : 25.06.2018
*  Created by : Max Sun
*  Description: timer0, timer1, timer2 setup
				PWM hardware output control
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include "timer.h"
#include <avr/interrupt.h>



volatile unsigned long milli_second;
volatile unsigned char milli_fraction;
volatile unsigned long timer0_counts;



void timer0_init(void)
{
	/*
	TCCR0A - Timer/Counter 0 Control Register A
	
	bit           7         6         5         4        3       2        1        0
	name        COM0A1    COM0A0    COM0B1    COM0B0     -       -      WGM01    WGM00
	set to        0         0         0         0        0       0        0        0
	
	COM0A1 = 0    normal port operation, OC0A disconnected
	COM0A0 = 0
	
	COM0B1 = 0    normal port operation, OC0B disconnected
	COM0B0 = 0
	
	bit 3 = 0
	bit 2 = 0
	
	WGM01 = 0     "Normal" timer/counter mode
	WGM00 = 0
	*/
	TCCR0A = 0b00000000;
	
		/*
	TCCR0B - Timer/Counter 0 Control Register B
	
	bit           7          6        5       4         3         2         1        0
	name        FOC0A      FOC0B      -       -       WGM02      CS02      CS01     CS00
	set to        0          0        0       0         0         1         0        1
	
	FOC0A = 0     don't use Force Output Compare A
	FOC0B = 0
	
	bit 5 = 0
	bit 4 = 0
	
	WGM02 = 0     "Normal" timer/counter mode, see TCCR0A
	
	CS02 = 0
	CS01 = 1      clock / 64 = 250kHz -> /256 -> 1024us per overflow
	CS00 = 1
	*/
	TCCR0B = 0b00000011;
	
		/*
	TIMSK0 - Timer/Counter 0 Interrupt Mask Register
	
	bit           7        6        5       4       3       2         1         0
	name          -        -        -       -       -     OCIE0B    OCIE0A    TOIE0
	set to        0        0        0       0       0       0         0         1
	
	bit 7 = 0     don't use Force Output Compare A
	bit 6 = 0
	bit 5 = 0
	bit 4 = 0
	bit 3 = 0
	OCIE0B = 0    don't enable Timer/Counter 0 Output Compare Match B Interrupt
	OCIE0A = 0    don't enable Timer/Counter 0 Output Compare Match A Interrupt Enable
	TOIE0 = 1     enable Timer/Counter 0 Overflow Interrupt
	*/
	TIMSK0 = 0b00000001;
	TCNT0 = 0;
	
}

void timer1_init(void)
{
	/*
	TCCR1A - Timer/Counter1 Control Register A
	
	bit           7         6         5         4        3       2        1        0
	name        COM1A1    COM1A0    COM1B1    COM1B0     -       -      WGM11    WGM10
	set to        0         0         0         0        0       0        0        0
	
	COM1A1 = 0    normal port operation, OC1A disconnected
	COM1A0 = 0
	
	COM1B1 = 0    normal port operation, OC1B disconnected
	COM1B0 = 0
	
	bit 3 = 0
	bit 2 = 0
	
	WGM11 = 1     Fast PWM 10 bit mode
	WGM10 = 1
	*/
	TCCR1A = 0b00000011;
	
	/*
	TCCR1B - Timer/Counter1 Control Register B
	
	bit           7          6        5         4          3         2         1        0
	name        ICNC1      ICES1      -       WGM13      WGM12      CS12      CS11     CS10
	set to        0          0        0         0          0         0         1        0
	
	ICNC1 = 0     don't use Input Capture Noise Canceler
	ICES1 = 0     don't use Input Capture Edge Select
	
	bit 5 = 0
	
	WGM13 = 0
	WGM12 = 1     Fast PWM mode 10 bit, see TCCR1A
	
	CS12 = 0
	CS11 = 0     clock / 1 = 16MHz
	CS10 = 1
	*/
	TCCR1B = 0b00001001;
	
	/*
	TCCR1C - Timer/Counter1 Control Register C
	
	bit           7          6        5       4       3       2       1      0
	name        FOC1A      FOC1B      -       -       -       -       -      -
	set to        0          0        0       0       0       0       0      0
	
	FOC1A = 0     don't use Force Output Compare for Channel A
	FOC1B = 0     don't use Force Output Compare for Channel B
	
	bit 5 = 0
	bit 4 = 0
	bit 3 = 0
	bit 2 = 0
	bit 1 = 0
	bit 0 = 0
	*/
	TCCR1C = 0b00000000;
	
	/*
	TIMSK1 - Timer/Counter 1 Interrupt Mask Register
	
	bit           7        6        5        4       3       2         1         0
	name          -        -      ICIE1      -       -     OCIE1B    OCIE1A    TOIE1
	set to        0        0        0        0       0       0         0         1
	
	bit 7 = 0
	bit 6 = 0
	ICIE1 = 0     don't use Input Capture Interrupt Enable
	bit 4 = 0
	bit 3 = 0
	OCIE1B = 0    don't enable Timer/Counter 1 Output Compare Match B Interrupt
	OCIE1A = 0    don't enable Timer/Counter 1 Output Compare Match A Interrupt Enable
	TOIE1 = 0     disable Timer/Counter 1 Overflow Interrupt
	*/
	TIMSK1 = 0b00000000;
	TCNT1 = 0;
}

void timer2_init(void)
{
	TCCR2A = 0b00000000;
	TCCR2B = 0b00000001;  //16MHz /1 prescalser
	TIMSK2 = 0b00000000;
	TCNT2 = 0;
}

void timer_init(void)
{
	timer0_init();
	timer1_init();
	timer2_init();
}

unsigned long millis(void)
{
	unsigned long m;
	uint8_t oldSREG = SREG;
	cli(); //disable global interrupt
	m = milli_second;
	SREG = oldSREG;
	return m;
}

ISR (TIMER0_OVF_vect)
{
	unsigned long m = milli_second;
	unsigned char f = milli_fraction;
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m +=1;
	}
	milli_second = m;
	milli_fraction = f;
	timer0_counts++;
}

/*
*  OC0A -> PD6   ...976.6Hz PWM
*  OC0B -> PD5   ...976.6Hz PWM
*  OC1A -> PB1   ...15.625kHz PWM
*  OC1B -> PB2   ...15.625kHz PWM
*  OC2A -> PB3   ...31.25kHz PWM
*  OC2B -> PD3   ...31.25kHz PWM
*/
void pwmSetDuty(uint8_t pin, int16_t dutyCycle)
{
	switch (pin) 
	{
		case  PORTD_6: //timer0, PD6, pin 12
			DDRD |= (1<<PD6);  //set pin as output
			TCCR0A |= (1<<COM0A1); //enable output compare on PD6, clear OC0A on compare, set OC0A at BOTTOM
			TCCR0A |= (1<<WGM01) | (1<<WGM00); //Fast PWM mode
					
			if (dutyCycle > 100) OCR0A = 0xff;    //upper 8bit PWM boundary
			else if(dutyCycle < 0) OCR0A = 0x00;  //lower PWM boundary
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR0A = dutyCycle * 255 / 100; //duty cycle percentage converted to 8 bit
			break;
		
		case PORTD_5 : //timer0, PD5, pin 11
			DDRD |= (1<<PD5);
			TCCR0A |= (1<<COM0B1); //enable output compare on PD5, clear OC0B on compare, set OC0B at BOTTOM
			TCCR0A |= (1<<WGM01) | (1<<WGM00);    //Fast PWM mode
			
			if (dutyCycle > 100) OCR0B = 0xff;	  //upper 8bit PWM boundary
			else if (dutyCycle < 0) OCR0B = 0x00; //lower 8bit PWM boundary
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR0B = dutyCycle * 255 / 100; //duty cycle percentage converted to 8 bit
			break;
			
		case  PORTB_1 : //timer1, PB1, pin 15
			DDRB |= (1<<PB1);
			TCCR1A |= (1<<COM0A1); //Clear OC1A on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)
			TCCR1A |= (1<<WGM11) | (1<<WGM11); //Fast PWM mode
			TCCR1B |= (1<<WGM12);
			
			if (dutyCycle > 100) OCR1A = 0x03ff;	//OCR1A is 16bit, 10 bit PWM maximum value is 0x03ff;
			else if (dutyCycle < 0) OCR1A = 0x0000; //OCR1A is 16bit, 10 bit PWM minimum value is 0x0000;
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR1A = (( dutyCycle * 1023.0 / 100)); //duty cycle percentage converted to 10 bit
			break;
		
		case PORTB_2 :	//timer1, PB2, pin 16
			DDRB |= (1<<PB2);
			TCCR1A |= (1<<COM0B1); //Clear OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)
			TCCR1A |= (1<<WGM11) | (1<<WGM11); //Fast PWM mode
			TCCR1B |= (1<<WGM12);
			
			if (dutyCycle > 100) OCR1B = 0x03ff;	//OCR2A is 16bit, 10 bit PWM maximum value is 0x03ff;
			else if (dutyCycle < 0) OCR1B = 0x0000; //OCR2B is 16bit, 10 bit PWM minimum value is 0x0000;
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR1B = (( dutyCycle * 1023.0 / 100)); //duty cycle percentage converted to 10 bit
			break;
			
		case PORTB_3 : //timer2, PB3, pin 17
			DDRB |= (1<<PB3);  //set pin as output
			TCCR2A |= (1<<COM2A1); //enable output compare on PD6, clear OC0A on compare, set OC0A at BOTTOM
			TCCR2A |= (1<<WGM01) | (1<<WGM00);         //Fast PWM mode
			
			if (dutyCycle > 100) PORTB |= (1<<PB3);    //upper PWM boundary
			else if(dutyCycle < 0) PORTB &= ~(1<<PB3); //lower PWM boundary
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR2A =  (dutyCycle * 255 / 100); //duty cycle percentage converted to 8 bit
			break;
		
		case PORTD_3 : //timer2, PD3, pin 5
			DDRD |= (1<<PD3);  //set pin as output
			TCCR2A |= (1<<COM2B1); //enable output compare on PD6, clear OC0A on compare, set OC0A at BOTTOM
			TCCR2A |= (1<<WGM01) | (1<<WGM00);   //Fast PWM mode
			
			if (dutyCycle > 100) OCR2B = 0xff;   //upper 8bit PWM boundary
			else if(dutyCycle < 0) OCR2B = 0x00; //lower 8bitPWM boundary
			else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR2B = dutyCycle * 255 / 100; //duty cycle percentage converted to 8 bit
			break;
		
		default:
			break;
	}
}





