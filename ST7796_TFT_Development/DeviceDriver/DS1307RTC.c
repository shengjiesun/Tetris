/*
 * DS1307RTC.c
 *
 * Created: 11/19/2019 8:39:23 PM
 *  Author: sunsh
 */ 

 #include <avr/io.h>
 #include <stdbool.h>
 #include "DS1307RTC.h"
 #include "TWI.h"

 /************************************************************************/
 /* Private function prototypes                                          */
 /************************************************************************/
 static uint8_t bcd2dec(uint8_t bcd);
 static uint8_t dec2bcd(uint8_t dec);

/************************************************************************/
/* Private variables                                                    */
/************************************************************************/
static bool mInitialised = false;

static const char dayOfWeekString[dowNumOfDays][10] =
{
  "N/A",
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};


/************************************************************************/
/* Public functions                                                     */
/************************************************************************/
void DS1307_Init(void)
{
  if (mInitialised)
  {
    return;
  }

  TWI_init();
  DS1307_ClockEnable();
  mInitialised = true;
}

uint8_t DS1307_isRunning(void)
{
  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);

  TWIStart(DS1307_Read);
  return !(TWIReadNACK() & 0x80);
}

void DS1307_ClockEnable(void)
{
  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);
  TWIStart(DS1307_Read);
  uint8_t status = TWIReadNACK();
  TWIStop();

  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);
  TWIWrite(status & ~0b10000000);
  TWIStop();
}

char* DS1307_GetDayString(tmElements_t* tm)
{
  return dayOfWeekString[tm->Day];
}

void DS1307_readDate(tmElements_t* date)
{
  uint8_t sec;
  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);

  TWIStart(DS1307_Read);
  sec = TWIReadACK();
  date->Second = bcd2dec(sec & 0b01111111);
  date->Minute = bcd2dec(TWIReadACK());
  date->Hour   = bcd2dec(TWIReadACK() & 0x3f);
  date->Day    = bcd2dec(TWIReadACK());
  date->Date   = bcd2dec(TWIReadACK());
  date->Month  = bcd2dec(TWIReadACK());
  date->Year   = (bcd2dec(TWIReadNACK()));
  TWIStop();
}

void DS1307_WriteAllReg(tmElements_t* tm)
{
  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);
  TWIWrite(0x80);            //Stop oscillator

  TWIWrite(dec2bcd(tm->Minute));

  if (tm->Hour & (1<<6)) TWIWrite(tm->Hour);
  else TWIWrite(dec2bcd(tm->Hour));

  
  TWIWrite(dec2bcd(tm->Day));
  TWIWrite(dec2bcd(tm->Date));
  TWIWrite(dec2bcd(tm->Month));
  TWIWrite(dec2bcd(y2kYearToTm(tm->Year)));
  TWIStop();

  TWIStart(DS1307_Write);
  TWIWrite(DS1307_SECONDS);
  TWIWrite(tm->Second & ~0x80);
  TWIStop();
}

/************************************************************************/
/*                  Private Functions                                   */
/************************************************************************/
static uint8_t bcd2dec(uint8_t bcd)
{
  return ((bcd/16 * 10) + (bcd % 16));
}

static uint8_t dec2bcd(uint8_t dec)
{
  return ((dec/10 * 16) + (dec % 10));
}