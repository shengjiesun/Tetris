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
#include "shapes.h"

__tetrisFrame TetrisFrame =
{
  .frame_x1 = 10,
  .frame_y1 = 8,
  .frame_x2 = 222,
  .frame_y2 = 310,
  .frame_thickness = 2,
  .frame_colour = WHITE,
  .frame_bg     = BLACK,
};

static __shapeCursor mShapeCursor = 
{
  .row = 0,
  .col = 5,
  .rot = 0
};

void drawFrame(void)
{
  LCD_GUI_DrawRectangle(TetrisFrame.frame_x1, 
                        TetrisFrame.frame_y1, 
                        TetrisFrame.frame_x2, 
                        TetrisFrame.frame_y2, 
                        TetrisFrame.frame_colour);
  LCD_GUI_DrawRectangle(TetrisFrame.frame_x1 - TetrisFrame.frame_thickness,
                        TetrisFrame.frame_y1 - TetrisFrame.frame_thickness,
                        TetrisFrame.frame_x2 + TetrisFrame.frame_thickness,
                        TetrisFrame.frame_y2 + TetrisFrame.frame_thickness,
                        TetrisFrame.frame_colour);
}

void refreshAllBlocks(uint16_t colour)
{
  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < 14; j++)
    {
      LCD_GUI_DrawFillRectangle(11 +(j*UNIT_BLOCK_SIZE + j) + 1,
                                9 + (i*UNIT_BLOCK_SIZE + i) + 1,
                                11 +(j*UNIT_BLOCK_SIZE + j) + UNIT_BLOCK_SIZE,
                                9 + (i*UNIT_BLOCK_SIZE + i) + UNIT_BLOCK_SIZE,
                                colour);
    }
  }
}

__unitBlockPos getUnitBlockPos(uint8_t row, uint8_t col)
{
  __unitBlockPos blockPos =
  {
    .unitBlock_x1 = 11 +(col*UNIT_BLOCK_SIZE + col) + 1,
    .unitBlock_y1 = 9 + (row*UNIT_BLOCK_SIZE + row) + 1,
    .unitBlock_x2 = 11 +(col*UNIT_BLOCK_SIZE + col) + UNIT_BLOCK_SIZE,
    .unitBlock_y2 = 9 + (row*UNIT_BLOCK_SIZE + row) + UNIT_BLOCK_SIZE, 
  };
  return blockPos;
}

void moveShapeCursor(uint8_t row, uint8_t col)
{
  mShapeCursor.row = row;
  mShapeCursor.col = col;
}

void returnShapeCursorHome()
{
  moveShapeCursor(0, 5);
}

void ShapeShiftUp(void)
{
  mShapeCursor.row--;
}

void ShapeShiftDown(void)
{
  mShapeCursor.row++;
}

void ShapeShiftRight(void)
{
  mShapeCursor.col++;
}

void ShapeShiftLeft(void)
{
  mShapeCursor.col--;
}

void ShapeRotate(void)
{
  mShapeCursor.rot++;
  if (mShapeCursor.rot >= 4) mShapeCursor.rot = 0;
}


void drawCurrentShape(uint8_t shapeNum, uint16_t colour)
{
  uint8_t rowPos = mShapeCursor.row;
  uint8_t colPos = mShapeCursor.col;
  
  uint8_t *shape = showShape(shapeNum);
  __unitBlockPos blockPos;
  uint16_t mColour;
  
  for (uint8_t i = 0; i < 4; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      //moveShapeCursor(rowPos, colPos);
      if (shape[Rotate(j, i, mShapeCursor.rot)]) mColour = colour;
      else mColour = BLACK;
      blockPos = getUnitBlockPos(rowPos, colPos) ;
      LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1, 
                                blockPos.unitBlock_y1, 
                                blockPos.unitBlock_x2, 
                                blockPos.unitBlock_y2, 
                                mColour);
      colPos++;
    }
    colPos = mShapeCursor.col;
    rowPos++;
  }
  moveShapeCursor(mShapeCursor.row, mShapeCursor.col);
}
      
