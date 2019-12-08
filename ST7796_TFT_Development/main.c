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
  USART_init(9600);
  DS1307_Init();
  LCD_Init();
  LCD_SetBackLight(100);
  LCD_direction(LCD_ROTATE_90);

  //LCD_ShowString(0, 16, 16, "Hello World!", WHITE, BLACK, 0);
  
  tmElements_t tm;
  char line1[20];
  char line2[20];
  char line3[20];
  char line4[20];

  while (1)
  {
    drawFrame();
    
    DS1307_readDate(&tm);
    sprintf(line1, "DATE:   %2d-%2d-%4d", tm.Date, tm.Month, (tm.Year + 2000));
    sprintf(line2, "TIME:    %2d:%2d:%2d ", tm.Hour, tm.Minute, tm.Second);
    sprintf(line3, "Weekday: %d", tm.Day);
    sprintf(line4, "%2d:%2d:%2d ", tm.Hour, tm.Minute, tm.Second);
    
//     uint16_t x0 = rand() % 320;
//     uint16_t y0 = rand() % 480;
//     uint16_t x1 = rand() % 320;
//     uint16_t y1 = rand() % 480;
//     uint16_t x2 = rand() % 320;
//     uint16_t y2 = rand() % 480;
//     uint8_t colour = rand() % 8;
//     uint8_t shape = rand() % 3;
//     uint8_t radius = rand() % 100;

    
     //LCD_ShowCharLarge(0,0,WHITE,BLACK,'!',1,0);
     //LCD_ShowChar2(8,0,WHITE,BLACK,'7',1,0);
     //LCD_ShowString(0,    0, 1, line4, WHITE, BLACK, 0);
     //LCD_ShowString(50,    80, 1, "!", WHITE, BLACK, 0);
     //LCD_ShowString(300, 40, 2, line4, WHITE, BLACK, 0);
     //LCD_ShowString(0,    2*3*16, 1, line2, WHITE, BLACK, 0);



//     for (int i = 0; i < 20; i++)
//     {
//       LCD_ShowString(0, 0, 16, "   ", WHITE, BLACK, 0);
//       LCD_ShowNum(0, 0, 16, WHITE, BLACK, i);
//       _delay_ms(500);
//     }
    
//     for (int i = 0; i < 20; i++)
//     {
//       LCD_ShowString(0, i*16, 16, "Hello World!", WHITE, BLACK, 0);
//     }
//     _delay_ms(1000);
//     for (int i = 0; i < 20; i++)
//     {
//       LCD_ShowString(120, i*16, 16, "Hello World!", WHITE, BLACK, 0);
//     }
//     _delay_ms(1000);
//     for (int i = 0; i < 20; i++)
//     {
//       LCD_ShowString(0, i*16, 16, "Hello World!", BLACK, BLACK, 0);
//     }
//     _delay_ms(1000);
//     for (int i = 0; i < 20; i++)
//     {
//       LCD_ShowString(120, i*16, 16, "Hello World!", BLACK, BLACK, 0);
//     }
//     _delay_ms(1000);

//     LCD_GUI_DrawFillRectangle(0, 0, 300, 400,  colourArray[colour]);
//     LCD_GUI_DrawRectangle(x1, y1, x2, y2, colourArray[colour]);
//     LCD_GUI_DrawDisc(x1, y1, 10, colourArray[colour]);

//     drawling(GREEN);
//     drawling(RED);
//     drawling(WHITE);

   
//     LCD_direction(1);
//     LCD_Clear(YELLOW);
//     _delay_ms(delaytime);
//     LCD_direction(2);
//     LCD_Clear(RED);
//     _delay_ms(delaytime);
//     LCD_direction(3);
//     LCD_Clear(GREEN);
//     _delay_ms(delaytime);
//     LCD_direction(0);
//     LCD_Clear(BLUE);
//     _delay_ms(delaytime);
  }
}

void drawling(uint16_t colour)
{
  for (int i = 0; i < 480; i++)
  {
    if (i % 7 == 0)
    {
      LCD_GUI_DrawLine(0, 0, 320, i, colour);
    }
  }

  for (int i = 320; i > 0; i--)
  {
    if (i % 7 == 0)
    {
      LCD_GUI_DrawLine(0, 0, i, 480, colour);
    }
  }
}

