/*
 * ST7796_TFT_Development.c
 *
 * Created: 12/1/2019 1:15:01 AM
 * Author : sunsh
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LCD_Driver.h"
//#include "LCD_GUI.h"
#include "SPI.h"
#include "DS1307RTC.h"
#include "TWI.h"
#include "USART.h"

#include "Tetris_GameEngine.h"
#include "Buttons.h"

#define LCD_I2C_ADDRESS (0x3F<<1) // PCF8574AT

void drawling(uint16_t colour);

uint16_t colourArray[8] =
{
  LIGHTGREEN,
  RED,
  GREEN,
  LIGHTBLUE,
  BLUE,
  YELLOW,
  MAGENTA,
  BROWN
};

int main(void)
{
  LCD_Init();
  LCD_SetBackLight(20);
  LCD_direction(LCD_ROTATE_90);
  Button_init();
  
  drawFrame();
  LCD_ShowString(320, 20, 2, "TETRIS", RED, BLACK, 0);
  uint8_t ShapeNum = 4;
  
  while (1)
  {
     uint8_t *field[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};
    
//     for (int j = 0; j < 7; j++)
//     {
//       for (int i = 0; i < 16; i++)
//       {
//         drawCurrentShape(j, GREEN);
//         _delay_ms(800);
//         drawCurrentShape(j, BLACK);
//         ShapeShiftDown();
// 
//         uint16_t buttonXVal = readButtonX();
//         uint16_t buttonYVal = readButtonY();
//         char Xval[6];
//         char Yval[6];
//         sprintf(Xval, "%4d", buttonXVal);
//         sprintf(Yval, "%4d", buttonYVal);
//         LCD_ShowString(320, 100, 1, Xval, WHITE, BLACK, 0);
//         LCD_ShowString(320, 120, 1, Yval, WHITE, BLACK, 0);
//       }
//       returnShapeCursorHome();
//     }
    uint16_t buttonXVal = readButtonX();
    uint16_t buttonYVal = readButtonY();
    uint8_t  buttonSwitch = readButtonSwitch();
    char Xval[6];
    char Yval[6];
    char Sval[6];
    sprintf(Xval, "%4d", buttonXVal);
    sprintf(Yval, "%4d", buttonYVal);
    sprintf(Sval, "%4d", buttonSwitch);
    LCD_ShowString(320, 100, 1, Xval, WHITE, BLACK, 0);
    LCD_ShowString(320, 120, 1, Yval, WHITE, BLACK, 0);
    LCD_ShowString(320, 140, 1, Sval, WHITE, BLACK, 0);


    drawCurrentShape(ShapeNum, GREEN);
    _delay_ms(50);
    if (buttonXVal < 20) 
    {
      drawCurrentShape(ShapeNum, BLACK);
      ShapeShiftUp();
      
    }
    else if (buttonXVal > 1000)
    {
      drawCurrentShape(ShapeNum, BLACK);
      ShapeShiftDown();
    } 

    if (buttonYVal < 20)
    {
      drawCurrentShape(ShapeNum, BLACK);
      ShapeShiftRight();
      
    }
    else if (buttonYVal > 1000)
    {
      drawCurrentShape(ShapeNum, BLACK);
      ShapeShiftLeft();
    }

    if (buttonSwitch == 0) ShapeRotate();
    

    
    

    



  }
}


