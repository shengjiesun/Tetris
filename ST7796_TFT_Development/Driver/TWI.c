/*
 * TWI.c
 *
 * Created: 4/7/2019 1:06:36 AM
 *  Author: sunsh
 */ 

#include <avr/io.h>
#include <util/twi.h>
#include <stdbool.h>
#include "TWI.h"

static bool mInitialised = false;

void TWI_init()
{
  if (mInitialised)
  {
    return;
  }

	//set SCL to 400kHz
	TWSR = 0x00;   //prescaler:1
	TWBR = I2C_BIT_RATE;
	//enable TWI
	TWCR = (1<<TWEN);

  mInitialised = true;
}



uint8_t TWIStart(uint8_t address)
{
	uint8_t status;
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);					//Start condition
	while ((TWCR & (1<<TWINT)) == 0);						//wait for transmission to complete
	
	status = TW_STATUS & 0xF8;								//check if start carried out successfully, Mask prescaler bits
	if ( (status != TW_START) && (status != TW_REP_START))  
	{
		return 1;											//Indicate failure if fail to start 
	}
	
	TWDR = address;											//load the TW data register with the byte
	TWCR = (1<<TWINT)|(1<<TWEN);							//start transmission of address
	while ((TWCR & (1<<TWINT)) == 0);						//wait for end of transmission
	
	status = TW_STATUS & 0xF8;								//check if start carried out successfully, Mask prescaler bits 
	if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK))// check if the device has acknowledged the READ / WRITE mode
	{
		return 1;											//Indicate failure if fail to start 
	}
	
	return 0;
}



uint8_t TWIStartWait(uint8_t address)
{
	uint8_t status;
	
	while (1)
	{
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);					//Start condition
		while ((TWCR & (1<<TWINT)) == 0);						//wait for transmission to complete

		status = TW_STATUS & 0xF8;								//check if start carried out successfully, Mask prescaler bits
		if ( (status != TW_START) && (status != TW_REP_START))
		{
			continue;
		}		
		
		TWDR = address;											//send device address
		TWCR = (1<<TWINT)|(1<<TWEN);
		
		while((TWCR & (1<<TWINT)) == 0);						//wail until transmission completed
		
		status = TW_STATUS & 0xF8;							    //Check the value of the TWI status register, Mask prescaler bits;
		if ((status == TW_MT_SLA_NACK) || (status == TW_MR_DATA_NACK))
		{
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);				//device busy, send stop condition to terminate write operation
			while(TWCR & (1<<TWSTO));							//wait until stop condition is executed and bus released
			continue;
		}				
		break;													//if status != TW_MT_SLA_ACK, 		
	}
}



void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);					//Send the stop condition
	while (TWCR & (1<<TWSTO));								//Wait until stop condition is excecuted and bus released
}



uint8_t TWIWrite(uint8_t u8data)
{
	uint8_t status;
	
	TWDR = u8data;										//load the TW data register with the byte	
	TWCR = (1<<TWINT)|(1<<TWEN);						
	while ((TWCR & (1<<TWINT)) == 0);					//wait for the transmission to complete
	
	status = TW_STATUS & 0xF8;							//check if start carried out successfully, Mask prescaler bits
	if (status != TW_MT_DATA_ACK)
	{
		return 1;
	}
	
	return 0;
}



uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}



uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}