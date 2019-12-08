/* USART Library Header file
* Created on : 21/06/2018
* Creator    : Max Sun
*/

#ifndef UART_H
#define UART_H
#endif

#define RX_BUFFER_SIZE 40
#define TX_BUFFER_SIZE 40

volatile unsigned char Rx_buffer[RX_BUFFER_SIZE];
volatile unsigned char Tx_buffer[TX_BUFFER_SIZE];


/************************************************************************/
/* Public function prototypes                                           */
/************************************************************************/
void USART_init(uint16_t BAUDRATE);
void USART_putstring(char* StringPtr);
void USART_printInt(long long integer);
void USART_printIntln(long long integer);
void USART_printFloat(float float_number);
long USART_received_Int(void);
