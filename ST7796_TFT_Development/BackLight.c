/*
 * BackLight.c
 *
 * Created: 12/1/2019 1:37:33 AM
 *  Author: sunsh
 */ 

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include "BackLight.h"

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

void Backlight_Init(void)
{
  timer0_init();
}

void SetBackLightDuty(uint8_t dutyCycle)
{
  DDRB |= (1<<PB7);  //set pin as output
  TCCR0A |= (1<<COM0A1); //enable output compare on PD6, clear OC0A on compare, set OC0A at BOTTOM
  TCCR0A |= (1<<WGM01) | (1<<WGM00); //Fast PWM mode
  
  if (dutyCycle > 100) OCR0A = 0xff;    //upper 8bit PWM boundary
  else if(dutyCycle < 0) OCR0A = 0x00;  //lower PWM boundary
  else if ((dutyCycle <= 100) && (dutyCycle >= 0)) OCR0A = dutyCycle * 255 / 100; //duty cycle percentage converted to 8 bit
}

/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
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