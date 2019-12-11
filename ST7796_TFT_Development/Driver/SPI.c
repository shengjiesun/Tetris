/* SPI.c Serial Perripheral Interface library
*  Created on : 24.6.2018
*  Created by : Max Sun
*
*/
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdbool.h>
#include "SPI.h"

/************************************************************************/
/* Private function prototypes                                          */
/************************************************************************/
static void SPI_enable_interrupt(void);
static void SPI_disable_interrupt(void);
static void SPI_enable(void);
static void SPI_disable(void);
static void SPI_Data_order_select(uint8_t order);
static void SPI_Master_Slave_Select(uint8_t ms);
static void SPI_Clock_Polarity_Select(uint8_t polarity);
static void SPI_Clock_Phase_Select(uint8_t phase);
static void SPI_Clock_Rate(SPI_Clock_Rate_t rate);

/************************************************************************/
/* Private variables                                                    */
/************************************************************************/
static mIsMasterInitialised = false;
static mIsSlaveInitialised = false;

/************************************************************************/
/* Public function definitions                                          */
/************************************************************************/
void SPI_Master_init(void)
{
  if (mIsMasterInitialised)
  {
    return;
  }

  DDR_SPI |= ((1 << SCK) | (1 << MOSI) | (1 << SS));
  
  SPI_Master_Slave_Select(SPI_MODE_MASTER);
  SPI_Data_order_select(SPI_DATAORDER_MSB);
  SPI_Clock_Polarity_Select(LEADING_EDGE_RISING);
  SPI_Clock_Phase_Select(LEADING_EDGE_SAMPLE);
  SPI_Clock_Rate(SPI_CLOCK_RATE_8MHz);
  SPI_enable();

  mIsMasterInitialised = true;
}

void SPI_Slave_init(void)
{
  if (mIsSlaveInitialised)
  {
    return;
  }

  DDRB |= (1 << MISO);
  
  SPI_Master_Slave_Select(SPI_MODE_SLAVE);
  SPI_Data_order_select(SPI_DATAORDER_MSB);
  SPI_Clock_Polarity_Select(LEADING_EDGE_RISING);
  SPI_Clock_Phase_Select(LEADING_EDGE_SAMPLE);
  SPI_Clock_Rate(SPI_CLOCK_RATE_8MHz);
  SPI_enable();

  mIsSlaveInitialised = true;
}

//Need to select slave before sending the data
char SPI_Master_transmit(char data)
{
  SPDR = data;
  while (!(SPSR & (1<<SPIF))) {};
  return SPDR;
}

char SPI_Slave_reveive(char data)
{
  while (!(SPSR & (1<<SPIF))) {};
  return SPDR;
}

/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
static void SPI_enable_interrupt(void)
{
	SPCR |= (1<<SPIE);
}

static void SPI_disable_interrupt(void)
{
	SPCR &= ~(1<<SPIE); 
}

static void SPI_enable(void)
{
	SPCR |= (1<<SPE);
}

static void SPI_disable(void)
{
	SPCR &= ~(1<<SPE);
}


static void SPI_Data_order_select(SPI_DataOder_t order)
{
	switch (order)
	{
		case SPI_DATAORDER_MSB :
			SPCR &= ~(1<<DORD);
			break;
		case SPI_DATAORDER_LSB :
			SPCR |= (1<<DORD);
			break;
		default:
			SPCR &= ~(1<<DORD);
			break;
	}
}

static void SPI_Master_Slave_Select(SPI_Mode_t mode)
{
	switch (mode)
	{
		case SPI_MODE_MASTER:
			SPCR |= (1<<MSTR);
			break;
		case SPI_MODE_SLAVE:
			SPCR &= ~(1<<MSTR);
			break;
		default:
			SPCR |= (1<<MSTR);
			break;
	}
}

static void SPI_Clock_Polarity_Select(uint8_t polarity)
{
	switch (polarity)
	{
		case LEADING_EDGE_FALLING :
			SPCR |= (1<<CPOL);
			break;
		case LEADING_EDGE_RISING :
			SPCR &= ~(1<<CPOL);
			break;
		default :
			SPCR &= ~(1<<CPOL);
			break;
	}
}

static void SPI_Clock_Phase_Select(uint8_t phase)
{
	switch (phase)
	{
		case LEADING_EDGE_SAMPLE :
			SPCR &= ~(1<<CPHA);
			break;
		case TRAILING_EDGE_SAMPLE :
			SPCR |= (1<<CPHA);
			break;
		default :
			SPCR &= ~(1<<CPHA);
			break;
	}
}

/*  SPI2X	SPR1	SPR0		SCK Frequency
*    0		  0		  0				F_CPU / 4    = 4MHz
*    0      0     1				F_CPU / 16   = 1MHz
*    0		  1		  0				F_CPU / 64   = 250KHz
*    0		  1		  1				F_CPU / 128  = 125KHz
*    1		  0		  0				F_CPU / 2    = 8MHz
*    1		  0		  1				F_CPU / 8    = 2MHz
*    1		  1		  0				F_CPU / 32   = 500KHz
*	   1		  1		  1				F_CPU / 64   = 250KHz
*/
static void SPI_Clock_Rate(SPI_Clock_Rate_t rate)
{
  switch (rate)
  {
    case SPI_CLOCK_RATE_8MHz:
      SPCR &= ~(1<<SPR1);
      SPCR &= ~(1<<SPR0);
      SPSR |=  (1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_4MHz:
      SPCR &= ~(1<<SPR1);
      SPCR &= ~(1<<SPR0);
      SPSR &= ~(1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_2MHz:
      SPCR &= ~(1<<SPR1);
      SPCR |=  (1<<SPR0);
      SPSR |=  (1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_1MHz:
      SPCR &= ~(1<<SPR1);
      SPCR |=  (1<<SPR0);
      SPSR &= ~(1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_500KHz:
      SPCR |=  (1<<SPR1);
      SPCR &= ~(1<<SPR0);
      SPSR |=  (1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_250KHz:
      SPCR |=  (1<<SPR1);
      SPCR &= ~(1<<SPR0);
      SPSR &= ~(1<<SPI2X);
      break;
    case SPI_CLOCK_RATE_125KHz:
      SPCR |=  (1<<SPR1);
      SPCR |=  (1<<SPR0);
      SPSR &= ~(1<<SPI2X);
      break;
    default:
      SPCR &= ~(1<<SPR1);
      SPCR |=  (1<<SPR0);
      SPSR |=  (1<<SPI2X);
      break;
  }
}

