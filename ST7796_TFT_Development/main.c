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
#include <stddef.h>
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
  pDDRV_Button_init();
  
  aTRS_ENG_drawBoundary();
  LCD_ShowString(320, 20, 2, "TETRIS", YELLOW, BLACK, 0);
  
  while (1)
  {
    __Button_Status button = pDDRV_Button_GetStatus();

    char ScoreString[7];
    char B_up[2];
    char B_left[2];
    char B_down[2];
    char B_right[2];
    char B_rotate[2];

    sprintf(ScoreString, "%6d", aTRS_ENG_GetScore());
    sprintf(B_up, "%d", button.up);
    sprintf(B_left, "%d", button.left);
    sprintf(B_down, "%d", button.down);
    sprintf(B_right, "%d", button.right);
    sprintf(B_rotate, "%d", button.rotate);

    LCD_ShowString(320, 80, 1, ScoreString, WHITE, BLACK, 0);
    LCD_ShowString(320, 100, 1, B_up, WHITE, BLACK, 0);
    LCD_ShowString(320, 120, 1, B_left, WHITE, BLACK, 0);
    LCD_ShowString(320, 140, 1, B_down, WHITE, BLACK, 0);
    LCD_ShowString(320, 160, 1, B_right, WHITE, BLACK, 0);
    LCD_ShowString(320, 180, 1, B_rotate, WHITE, BLACK, 0);

    

    if (button.up == BUTTON_PRESSED) 
    {
      if (aTRS_ENG_BoundaryCheck_Translate(0))
      {
        aTRS_ENG_ShapeShiftUp();
      }
    }
    
    if (button.down == BUTTON_PRESSED)
    {
      if (aTRS_ENG_BoundaryCheck_Translate(2))
      {
        aTRS_ENG_ShapeShiftDown();
      }
      else
      {
        aTRS_ENG_AttachToField();
        aTRS_ENG_CheckClearedRows();
        
        if (aTRS_ENG_GenerateNewShape() == NULL) 
        {
          aTRS_ENG_GameOver();
        }
      }
    } 

    if (button.right == BUTTON_PRESSED)
    {
      if (aTRS_ENG_BoundaryCheck_Translate(1))
      {
        aTRS_ENG_ShapeShiftRight();    
      }     
    }

    if (button.left == BUTTON_PRESSED)
    {
      if (aTRS_ENG_BoundaryCheck_Translate(3))
      {
        aTRS_ENG_ShapeShiftLeft();
      }
    }

    if (button.rotate == BUTTON_PRESSED) 
    {
      if(aTRS_ENG_BoundaryCheck_Rotate())
      {
        aTRS_ENG_ShapeRotate();
      }
    }

    aTRS_ENG_drawCurrentShape();
    _delay_ms(10);


  }
}


