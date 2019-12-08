/*
 * BackLight.h
 *
 * Created: 12/1/2019 1:43:05 AM
 *  Author: sunsh
 */ 


#ifndef BACKLIGHT_H_
#define BACKLIGHT_H_

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define MICROSECONDS_PER_TIMER0_OVERFLOW ( ((64 * 256) * 1000L) / (F_CPU / 1000L) )

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)



 void timer0_init(void);
 void Backlight_Init(void);
 void SetBackLightDuty(uint8_t dutyCycle);

#endif /* BACKLIGHT_H_ */