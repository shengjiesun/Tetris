/*
 * DS1307RTC.h
 *
 * Created: 11/19/2019 8:38:23 PM
 *  Author: sunsh
 */ 


#ifndef DS1307RTC_H_
#define DS1307RTC_H_

/************************************************************************/
/*DS1307 Related defines                                                */
/************************************************************************/
#define DS1307_I2C_ADDR 0x68  //I2C address
#define DS1307_Write    ((DS1307_I2C_ADDR << 1) & ~(1<<0))
#define DS1307_Read     ((DS1307_I2C_ADDR << 1) | (1<<0))

#define DS1307_SECONDS  0x00
#define DS1307_MINUTES  0x01
#define DS1307_HOURS    0x02
#define DS1307_DAY      0x03
#define DS1307_DATE     0x04
#define DS1307_MONTH    0x05
#define DS1307_YEAR     0x06
#define DS1307_CONTROL  0x07

#define DS1307_CONTROL_OUT  (1<<7)
#define DS1307_CONTROL_SQWE (1<<4)
#define DS1307_CONTROL_RS1  (1<<1)
#define DS1307_CONTROL_RS0  (1<<0)
#define DS1307_CH	(1<<7)

/************************************************************************/
/*Useful defines                                                        */
/************************************************************************/
#define  tmYearToY2k(Y)      ((Y) + 2000)
#define  y2kYearToTm(Y)      ((Y) - 2000)

#define SECS_PER_MIN         (60UL)
#define SECS_PER_HOUR        (3600UL)
#define SECS_PER_DAY         (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK        (7UL)
#define SECS_PER_WEEK        (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR        (SECS_PER_WEEK * 52UL)
#define SECS_AT_YR_2000      (946684800UL) // the time at the start of the year 2000

/************************************************************************/
/* Define types and structures                                          */
/************************************************************************/
typedef enum
{
  timeNotSet = 0,
  timeNeedsSync,
  timeSet
} timeStatus_t;

typedef enum
{
  dowInvalid = 0,
  dowSunday,
  dowMonday,
  dowTuesday,
  dowWednesday,
  dowThursday,
  dowFriday,
  dowSaturday,
  dowNumOfDays
} timeDayOfWeek_t;

typedef struct  {
  uint8_t Second;
  uint8_t Minute;
  uint8_t Hour;
  uint8_t Day;   // day of week, Sunday is day 1
  uint8_t Date;
  uint8_t Month;
  uint8_t Year;   // offset from 1970;
} tmElements_t;


/************************************************************************/
/* Public function prototype                                            */
/************************************************************************/
void DS1307_Init(void);
void DS1307_readDate(tmElements_t* date);
uint8_t DS1307_isRunning(void);
void DS1307_ClockEnable(void);
char* DS1307_GetDayString(tmElements_t* tm);

#endif /* DS1307RTC_H_ */