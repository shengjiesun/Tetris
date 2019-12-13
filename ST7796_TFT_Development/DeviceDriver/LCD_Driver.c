/*
 * LCD_Driver.c 
 *
 * Created: 12/3/2019 12:47:58 AM
 *  Author: sunsh
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "LCD_Driver.h"
#include "BackLight.h"
#include "SPI.h"

#define DDR_LCD_CTRL DDRE
#define LCD_RS       PE5
#define LCD_RST      PE4
#define LCD_CS       SS

#define LCD_CS_SET   (PORT_SPI |= (1<<LCD_CS))
#define LCD_RS_SET   (PORTE    |= (1<<LCD_RS))     //RS set for writing data
#define LCD_RST_SET  (PORTE    |= (1<<LCD_RST))

#define LCD_CS_CLR   (PORT_SPI &= ~(1<<LCD_CS))
#define LCD_RS_CLR   (PORTE    &= ~(1<<LCD_RS))    //RS clear for writing command
#define LCD_RST_CLR  (PORTE    &= ~(1<<LCD_RST))


/************************************************************************/
/* Private function prototypes                                          */
/************************************************************************/
static void LCD_WR_REG(uint8_t data);
static void LCD_WR_DATA(uint8_t data);
static void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue);
static void LCD_WriteRAM_Prepare(void);
static void LCD_GPIOInit(void);


/************************************************************************/
/* Private variables                                                    */
/************************************************************************/
LCD_Dev lcddev;
uint16_t POINT_COLOR = 0x0000;
uint16_t BACK_COLOR = 0xFFFF;  


/************************************************************************/
/* Public function definitions                                          */
/************************************************************************/
void LCD_Init(void)
{
  SPI_Master_init();
  LCD_GPIOInit();
  Backlight_Init();
  LCD_RESET();

  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0xC3);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x96);
  LCD_WR_REG(0x36);
  LCD_WR_DATA(0x68);
  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x05);
  LCD_WR_REG(0xB0);
  LCD_WR_DATA(0x80);
  LCD_WR_REG(0xB6);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x02);
  LCD_WR_REG(0xB5);
  LCD_WR_DATA(0x02);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x04);
  LCD_WR_REG(0xB1);
  LCD_WR_DATA(0x80);
  LCD_WR_DATA(0x10);
  LCD_WR_REG(0xB4);
  LCD_WR_DATA(0x00);
  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0xC6);
  LCD_WR_REG(0xC5);
  LCD_WR_DATA(0x24);
  LCD_WR_REG(0xE4);
  LCD_WR_DATA(0x31);
  LCD_WR_REG(0xE8);
  LCD_WR_DATA(0x40);
  LCD_WR_DATA(0x8A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x29);
  LCD_WR_DATA(0x19);
  LCD_WR_DATA(0xA5);
  LCD_WR_DATA(0x33);
  LCD_WR_REG(0xC2);
  LCD_WR_REG(0xA7);

  LCD_WR_REG(0xE0);
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x13);
  LCD_WR_DATA(0x12);
  LCD_WR_DATA(0x12);
  LCD_WR_DATA(0x2B);
  LCD_WR_DATA(0x3C);
  LCD_WR_DATA(0x44);
  LCD_WR_DATA(0x4B);
  LCD_WR_DATA(0x1B);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1D);
  LCD_WR_DATA(0x21);

  LCD_WR_REG(0XE1);
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x13);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x0D);
  LCD_WR_DATA(0x27);
  LCD_WR_DATA(0x3B);
  LCD_WR_DATA(0x44);
  LCD_WR_DATA(0x4D);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1D);
  LCD_WR_DATA(0x21);

  LCD_WR_REG(0X36);
  LCD_WR_DATA(0xEC);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0xC3);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x69);
  LCD_WR_REG(0X13);
  LCD_WR_REG(0X11);
  LCD_WR_REG(0X29);

  LCD_direction(LCD_ROTATE_0);
  SetBackLightDuty(100);
  LCD_Clear(BLACK);
}

void LCD_Clear(uint16_t Color)
{
  unsigned int i,m;
  LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
  LCD_CS_CLR;
  LCD_RS_SET;
  for(i=0;i<lcddev.height;i++)
  {
    for(m=0;m<lcddev.width;m++)
    {
      LCD_WriteData_16Bit(Color);
    }
  }
  LCD_CS_SET;
}

void LCD_RESET(void)
{
  LCD_RST_CLR;
  _delay_ms(100);
  LCD_RST_SET;
  _delay_ms(50);
}

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
  LCD_WR_REG(lcddev.setxcmd);
  LCD_WR_DATA(xStar >> 8);
  LCD_WR_DATA(0x00FF & xStar);
  LCD_WR_DATA(xEnd >> 8);
  LCD_WR_DATA(0x00FF & xEnd);

  LCD_WR_REG(lcddev.setycmd);
  LCD_WR_DATA(yStar >> 8);
  LCD_WR_DATA(0x00FF & yStar);
  LCD_WR_DATA(yEnd >> 8);
  LCD_WR_DATA(0x00FF & yEnd);

  LCD_WriteRAM_Prepare();
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);
}

void LCD_direction(LCD_Rotation_t direction)
{
  lcddev.setxcmd=0x2A;
  lcddev.setycmd=0x2B;
  lcddev.wramcmd=0x2C;
  switch(direction)
  {
    case LCD_ROTATE_0:
      lcddev.width=LCD_W;
      lcddev.height=LCD_H;
      LCD_WriteReg(0x36,(1<<3)|(1<<6));
      break;
    case LCD_ROTATE_90:
      lcddev.width=LCD_H;
      lcddev.height=LCD_W;
      LCD_WriteReg(0x36,(1<<3)|(1<<5));
      break;
    case LCD_ROTATE_180:
      lcddev.width=LCD_W;
      lcddev.height=LCD_H;
      LCD_WriteReg(0x36,(1<<3)|(1<<7));
      break;
    case LCD_ROTATE_270:
      lcddev.width=LCD_H;
      lcddev.height=LCD_W;
      LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
      break;
    default:
      break;
  }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t colour)
{
  LCD_SetCursor(x,y);
  LCD_WriteData_16Bit(colour);
}

void LCD_SetBackLight(uint8_t duty)
{
  SetBackLightDuty(duty);
}

void LCD_WriteData_16Bit(uint16_t Data)
{
  LCD_CS_CLR;
  LCD_RS_SET;
  SPI_Master_transmit(Data>>8);
  SPI_Master_transmit(Data);
  LCD_CS_SET;
}


/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
static void LCD_WR_REG(uint8_t data)
{
  LCD_CS_CLR;
  LCD_RS_CLR;
  SPI_Master_transmit(data);
  LCD_CS_SET;
}

static void LCD_WR_DATA(uint8_t data)
{
  LCD_CS_CLR;
  LCD_RS_SET;
  SPI_Master_transmit(data);
  LCD_CS_SET;
}

static void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
  LCD_WR_DATA(LCD_RegValue);
}

static void LCD_WriteRAM_Prepare(void)
{
  LCD_WR_REG(lcddev.wramcmd);
}

void LCD_GPIOInit(void)
{
  DDR_LCD_CTRL |= ((1 << LCD_RST) | (1 << LCD_RS));
}