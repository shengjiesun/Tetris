/*
 * IncFile1.h
 *
 * Created: 4/7/2019 1:06:11 AM
 *  Author: sunsh
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define F_CPU 16000000UL

#define I2C_PRESCALER 1UL
#define F_I2C_SCL 400000UL
#define I2C_BIT_RATE ((F_CPU/F_I2C_SCL - 16) / (2 * I2C_PRESCALER))


/************************************************************************/
/* Initialization of the I2C bus interface. only need to be called once */                                                             
/************************************************************************/
void TWI_init();



/************************************************************************/
/* Sending starting condition, device address and transfer direction    */
/* @Param(address): 7 bit address with the 8th bit(0:write, 1:read)     */
/* @return 0: device accessed successfully, 1: fail to access the device*/
/************************************************************************/
uint8_t TWIStart(uint8_t address);



/************************************************************************/
/* Issues a start condition and sends address and transfer direction    */
/* If I2C bus is busy, use ack polling to wait until device is ready    */
/* @Param(address): 7 bit address with the 8th bit(0:write, 1:read)     */
/* @return 0: device accessed successfully, 1: fail to access the device*/
/************************************************************************/
uint8_t TWIStartWait(uint8_t address);


/************************************************************************/
/* Sending the stop condition and release the I2C bus                   */
/************************************************************************/
void TWIStop(void);



/************************************************************************/
/* Send one byte to I2C device                                          */
/* @Param u8data? One byte data to be transfered                       */
/* @return 0: written successfully, 1:failed to write                   */
/************************************************************************/
uint8_t TWIWrite(uint8_t u8data);



/************************************************************************/
/* Read one byte from the I2C device, request more data from device     */
/* @return byte read from the I2C device                                */
/************************************************************************/
uint8_t TWIReadACK(void);



/************************************************************************/
/* Read one byte from I2C device, read is followed by a stop condition  */
/* @return byte read from the I2C device								*/
/************************************************************************/
uint8_t TWIReadNACK(void);



#endif /* INCFILE1_H_ */