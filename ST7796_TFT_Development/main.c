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

char mScore[10];

int main(void)
{
  LCD_Init();
  LCD_SetBackLight(20);
  LCD_direction(LCD_ROTATE_90);
  pDDRV_Button_init();
  
  aTRS_ENG_drawBoundary();
  LCD_ShowString(270, 10, 4, "TETRIS", YELLOW, BLACK, 0);
  LCD_GUI_DrawRectangle(320, 90, 380, 163, WHITE);
  LCD_ShowString(250, 200, 2, "Score", YELLOW, BLACK,0);
  LCD_ShowNum(340, 200, 2, WHITE, BLACK, (int)aTRS_ENG_GetScore() );

  aTRS_ENG_GenerateNewShape();

  

  while (1)
  {
    sprintf(mScore, "%d", aTRS_ENG_GetScore());
    
    __Button_Status button = pDDRV_Button_GetStatus();

    char ScoreString[7]; 

    if (button.up == BUTTON_PRESSED) 
    {
      if (aTRS_ENG_BoundaryCheck_Translate(0))
      {
        aTRS_ENG_drawCurrentShape(BLACK);
        aTRS_ENG_ShapeShiftUp();
      }
    }
    
    if (button.down == BUTTON_PRESSED)
    {
      if (aTRS_ENG_BoundaryCheck_Translate(2))
      {
        aTRS_ENG_drawCurrentShape(BLACK);
        aTRS_ENG_ShapeShiftDown();
      }
      else
      {
        aTRS_ENG_AttachToField();
        aTRS_ENG_CheckClearedRows();
        LCD_ShowNum(340, 200, 2, WHITE, BLACK, (int)aTRS_ENG_GetScore() );
        
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
        aTRS_ENG_drawCurrentShape(BLACK);
        aTRS_ENG_ShapeShiftRight();    
      }     
    }

    if (button.left == BUTTON_PRESSED)
    {
      if (aTRS_ENG_BoundaryCheck_Translate(3))
      {
        aTRS_ENG_drawCurrentShape(BLACK);
        aTRS_ENG_ShapeShiftLeft();
      }
    }

    if (button.rotate == BUTTON_PRESSED) 
    {
      if(aTRS_ENG_BoundaryCheck_Rotate())
      {
        aTRS_ENG_drawCurrentShape(BLACK);
        aTRS_ENG_ShapeRotate();
      }
    }

    aTRS_ENG_drawCurrentShape(GREEN);
    aTRS_ENG_drawNextShape(320, 100, BLUE);
    
    //aTRS_ENG_UpdateCurrentShape();


  }
}


