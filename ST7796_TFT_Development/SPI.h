/* SPI.h Serial Perripheral Interface library
*  Created on : 24.6.2018
*  Created by : Max Sun
*
*/

#ifndef SPI_H
#define SPI_H


/************************************************************************/
/* SPI pin allocation for ATmega2560                                    */
/************************************************************************/
#define DDR_SPI         DDRB
#define SS              PB0
#define SCK              PB1
#define MOSI            PB2
#define MISO            PB3
#define PORT_SPI        PORTB

/************************************************************************/
/* SPI data order                                                       */
/************************************************************************/
typedef enum
{
  SPI_DATAORDER_MSB = 0,
  SPI_DATAORDER_LSB
} SPI_DataOder_t;

/************************************************************************/
/* SPI mode                                                             */
/************************************************************************/
typedef enum
{
  SPI_MODE_SLAVE = 0,
  SPI_MODE_MASTER
} SPI_Mode_t;

/************************************************************************/
/* SPI clock polarity                                                   */
/************************************************************************/
#define LEADING_EDGE_RISING 0
#define LEADING_EDGE_FALLING 1

/************************************************************************/
/* SPI clock phase                                                      */
/************************************************************************/
#define LEADING_EDGE_SAMPLE 0
#define TRAILING_EDGE_SAMPLE 1

/************************************************************************/
/* SPI clock polarity                                                   */
/************************************************************************/
typedef enum
{ 
  SPI_CLOCK_RATE_8MHz = 0,
  SPI_CLOCK_RATE_4MHz,
  SPI_CLOCK_RATE_2MHz,
  SPI_CLOCK_RATE_1MHz,
  SPI_CLOCK_RATE_500KHz,
  SPI_CLOCK_RATE_250KHz,
  SPI_CLOCK_RATE_125KHz,
  SPI_CLOCK_RATE_NUM
} SPI_Clock_Rate_t;


/************************************************************************/
/* Public function prototypes                                           */
/************************************************************************/
void SPI_Master_init(void);
void SPI_Slave_init(void);
char SPI_Master_transmit(char data);
char SPI_Slave_reveive(char data);

#endif
