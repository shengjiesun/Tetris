/*
 * Tetris_GameEngine.c
 *
 * Created: 12/8/2019 10:42:48 PM
 *  Author: sunsh
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include "LCD_Driver.h"
#include "LCD_GUI.h"
#include "Tetris_GameEngine.h"

__tetrisFrame TetrisFrame =
{
  .frame_x1 = 10,
  .frame_y1 = 10,
  .frame_x2 = 220,
  .frame_y2 = 300,
  .frame_thickness = 2,
  .frame_colour = WHITE,
  .frame_bg     = BLACK,
};

void drawFrame(void)
{
  LCD_GUI_DrawRectangle(TetrisFrame.frame_x1, 
                        TetrisFrame.frame_y1, 
                        TetrisFrame.frame_x2, 
                        TetrisFrame.frame_y2, 
                        TetrisFrame.frame_colour);

  LCD_GUI_DrawRectangle(TetrisFrame.frame_x1 + TetrisFrame.frame_thickness,
                        TetrisFrame.frame_y1 + TetrisFrame.frame_thickness,
                        TetrisFrame.frame_x2 - TetrisFrame.frame_thickness,
                        TetrisFrame.frame_y2 - TetrisFrame.frame_thickness,
                        TetrisFrame.frame_colour);

  LCD_GUI_DrawFillRectangle(TetrisFrame.frame_x1 + TetrisFrame.frame_thickness + 1,
                            TetrisFrame.frame_y1 + TetrisFrame.frame_thickness + 1,
                            TetrisFrame.frame_x2 - TetrisFrame.frame_thickness - 1,
                            TetrisFrame.frame_y2 - TetrisFrame.frame_thickness - 1,
                            TetrisFrame.frame_bg);



}
