/* USART Library Header file
* Created on : 21/06/2018
* Creator    : Max Sun
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "USART.h"

static unsigned char USART_receive(void);
static void USART_send(unsigned char data);
 
static volatile uint8_t Rx_buffer_index = 0;
static volatile uint8_t Tx_buffer_index = 0;

/************************************************************************/
/* Public function definitions                                          */
/************************************************************************/
void USART_init(uint16_t BAUDRATE)
{
	uint16_t BAUD_PRESCALLER = ((F_CPU) / ((BAUDRATE) * 16UL) - 1UL);
	UBRR0H = (unsigned char)(BAUD_PRESCALLER>>8);
	UBRR0L = (unsigned char)(BAUD_PRESCALLER);
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); //enable TX and RX
	UCSR0B |= (1<<RXCIE0);			 //enable RX interrupt
	UCSR0C |= (3<<UCSZ00);			//assume 8 bit data frame
	sei();
}
	
void USART_putstring(char* StringPtr)
{
	while (*StringPtr != 0x00)
	{
		USART_send(*StringPtr);
		StringPtr++;
	}	
}

void USART_printInt(long long integer)
{
	char value[10];
	ltoa(integer, value, 10);
	USART_putstring(value);
}

void USART_printIntln(long long integer)
{
	char value[10];
	ltoa(integer, value, 10);
	USART_putstring(value);
	USART_putstring("\n");
}

void USART_printFloat(float float_number)
{
	//need to click project -> (Alt + F7) -> AVR/GNU liner -> General -> check the [use vprintf library]
	// -> Miscellaneous -> type in "-lprintf_flt" in the other linker flags
	//Default disables it because it saves memory.
	char value[50];
	sprintf(value, "%.4f", float_number);
	USART_putstring(value);
}

long USART_received_Int(void)
{
	long number;
	number = atol(Rx_buffer);
	return number;
}


/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
static unsigned char USART_receive(void)
{
  //RXC0 is set when there is unread data in the receive buffer, and cleared when the
  //receive buffer is empty.
  while(!(UCSR0A & (1<<RXC0)));  //wait until RXC0 is set
  return UDR0;
}

static void USART_send(unsigned char data)
{
  //UDRE0 indicates if the transmit buffer (UDR0) is ready to receive new data.
  //if UDRE0 is 1, the buffer is empty and ready to be written.
  while (!(UCSR0A & (1 << UDRE0)));  //wait until UDRE0 is set
  UDR0 = data;
}

ISR(USART0_RX_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	//UDR0 = ReceivedByte; // Echo back the received byte back to the computer
	Rx_buffer[Rx_buffer_index++] = ReceivedByte;
	if (ReceivedByte == '\n') 
	{
		Rx_buffer[Rx_buffer_index] = 0; //put zero to terminate the string array
		USART_putstring("[Rx echo]: ");
		USART_putstring(Rx_buffer);
		Rx_buffer_index = 0;
		ReceivedByte = 0x00;
	}
}
