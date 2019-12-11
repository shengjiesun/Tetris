/*
 * Tetris_GameEngine.c
 *
 * Created: 12/8/2019 10:42:48 PM
 *  Author: sunsh
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stddef.h>
#include "LCD_Driver.h"
#include "LCD_GUI.h"
#include "Tetris_GameEngine.h"

#define UNIT_BLOCK_SIZE 14


/************************************************************************/
/* Private function prototype                                           */
/************************************************************************/
static uint8_t Rotate(uint8_t px, uint8_t py, uint8_t r);
static __unitBlockPos getUnitBlockPos(uint8_t row, uint8_t col);

/************************************************************************/
/* Private variables                                                    */
/************************************************************************/
static const uint8_t shape0[] = {0, 0, 1, 0,\
                                 0, 0, 1, 0,\
                                 0, 0, 1, 0,\
                                 0, 0, 1, 0,};

static const uint8_t shape1[] = {0, 0, 1, 0,\
                                 0, 1, 1, 0,\
                                 0, 1, 0, 0,\
                                 0, 0, 0, 0,};

static const uint8_t shape2[] = {0, 1, 0, 0,\
                                 0, 1, 1, 0,\
                                 0, 0, 1, 0,\
                                 0, 0, 0, 0,};

static const uint8_t shape3[] = {0, 0, 0, 0,\
                                 0, 1, 1, 0,\
                                 0, 1, 1, 0,\
                                 0, 0, 0, 0,};

static const uint8_t shape4[] = {0, 0, 1, 0,\
                                 0, 0, 1, 0,\
                                 0, 1, 1, 0,\
                                 0, 0, 0, 0,};

static const uint8_t shape5[] = {0, 1, 0, 0,\
                                 0, 1, 0, 0,\
                                 0, 1, 1, 0,\
                                 0, 0, 0, 0,};

static const uint8_t shape6[] = {0, 0, 1, 0,\
                                 0, 1, 1, 0,\
                                 0, 0, 1, 0,\
                                 0, 0, 0, 0,};

static const uint8_t* mShapeArray[7] = {shape0, shape1, shape2, shape3, shape4, shape5, shape6};

static uint8_t *field[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};
static uint8_t* mCurrentShape = shape0;

static __tetrisFrame TetrisFrame =
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

/************************************************************************/
/* Public function definitions                                          */
/************************************************************************/
void aTRS_ENG_drawBoundary(void)
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

uint8_t* aTRS_ENG_GenerateNewShape(void)
{
  uint8_t newShape = rand() % 7;
  mCurrentShape = mShapeArray[newShape];
  aTRS_ENG_returnShapeCursorHome();

  for (uint8_t nRow = 0; nRow < 4; nRow++)
  {
    for (uint8_t nCol = 0; nCol < 4; nCol++)
    {
      if ((mCurrentShape[Rotate(nCol, nRow, mShapeCursor.rot)] != 0) && (field[(mShapeCursor.row + nRow)  * SCREEN_WIDTH + mShapeCursor.col + nCol] !=0))
      {
        mCurrentShape = NULL;
      }
    }
  }
  return mCurrentShape;
}

void aTRS_ENG_moveShapeCursor(uint8_t row, uint8_t col)
{
  mShapeCursor.row = row;
  mShapeCursor.col = col;
}

void aTRS_ENG_returnShapeCursorHome()
{
  aTRS_ENG_moveShapeCursor(0, 5);
}

void aTRS_ENG_ShapeShiftUp(void)
{
  mShapeCursor.row--;
}

void aTRS_ENG_ShapeShiftDown(void)
{
  mShapeCursor.row++;
}

void aTRS_ENG_ShapeShiftRight(void)
{
  mShapeCursor.col++;
}

void aTRS_ENG_ShapeShiftLeft(void)
{
  mShapeCursor.col--;
}

void aTRS_ENG_ShapeRotate(void)
{
  mShapeCursor.rot++;
  if (mShapeCursor.rot >= 4) mShapeCursor.rot = 0;
}

void aTRS_ENG_drawCurrentShape(uint16_t colour)
{
  
  uint8_t rowPos = mShapeCursor.row;
  uint8_t colPos = mShapeCursor.col;
  
  __unitBlockPos blockPos;
  uint16_t mColour;
  
  for (uint8_t i = 0; i < 4; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      if (mCurrentShape[Rotate(j, i, mShapeCursor.rot)]) mColour = colour;
      else mColour = BLACK;

      if ((mShapeCursor.col + j >= 0) && (mShapeCursor.col + j < SCREEN_WIDTH) && (mShapeCursor.row + i < SCREEN_HEIGHT)) //Check if shape is out of field
      {
        if (field[(mShapeCursor.row + i) * SCREEN_WIDTH + (mShapeCursor.col + j)] == 0) //Check if field pos is blank
        {
            blockPos = getUnitBlockPos(rowPos, colPos) ;
            
            LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
                                      blockPos.unitBlock_y1,
                                      blockPos.unitBlock_x2,
                                      blockPos.unitBlock_y2,
                                      mColour);
        }
      }

      colPos++;
    }
    colPos = mShapeCursor.col;
    rowPos++;
  }
  aTRS_ENG_moveShapeCursor(mShapeCursor.row, mShapeCursor.col);
}

void aTRS_ENG_AttachToField(void)
{
  __unitBlockPos blockPos;
  for (uint8_t nRow = 0; nRow < 4; nRow++)
  {
    for (uint8_t nCol= 0; nCol < 4; nCol++)
    {
      if ((mShapeCursor.row + nRow < SCREEN_HEIGHT) && 
          (mShapeCursor.col + nCol < SCREEN_WIDTH)  && 
          (mShapeCursor.col + nCol >= 0) && 
          (mCurrentShape[Rotate(nCol, nRow, mShapeCursor.rot)] != 0))
      {
        field[(mShapeCursor.row + nRow)* SCREEN_WIDTH + mShapeCursor.col+ nCol] = 1;
        blockPos = getUnitBlockPos(mShapeCursor.row + nRow, mShapeCursor.col+ nCol) ;
        LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
                                  blockPos.unitBlock_y1,
                                  blockPos.unitBlock_x2,
                                  blockPos.unitBlock_y2,
                                  RED);
      }
    }
  }
}


/*
 * Boundary Checks
 */
bool aTRS_ENG_BoundaryCheck_Translate(uint8_t direction)
{
  __shapeCursor tmpShapeCursor = mShapeCursor;

  switch (direction)
  {
    case 1:
    tmpShapeCursor.col++;
    break;
    case 2:
    tmpShapeCursor.row++;
    break;
    case 3:
    tmpShapeCursor.col--;
    break;
    default:
    break;
  }

  for (uint8_t nRow = 0; nRow < 4; nRow++)
  {
    for (uint8_t nCol = 0; nCol < 4; nCol++)
    {
      //Check if exceed game frame
      if ((mCurrentShape[Rotate(nCol, nRow, mShapeCursor.rot)] != 0)
      && ((tmpShapeCursor.col + nCol < 0) || (tmpShapeCursor.col + nCol > SCREEN_WIDTH - 1) || (tmpShapeCursor.row + nRow > SCREEN_HEIGHT -1)))
      {
        return false;
      }

      //Check if stopped by piled up shapes
      if ((mCurrentShape[Rotate(nCol, nRow, mShapeCursor.rot)] != 0) && (field[(tmpShapeCursor.row + nRow)  * SCREEN_WIDTH + tmpShapeCursor.col + nCol] !=0))
      {
        return false;
      }
    }
  }
  return true;
}

bool aTRS_ENG_BoundaryCheck_Rotate(void)
{
  __shapeCursor tmpShapeCursor = mShapeCursor;
  tmpShapeCursor.rot++;
  if (mShapeCursor.rot >= 4) mShapeCursor.rot = 0;

  for (uint8_t nRow = 0; nRow < 4; nRow++)
  {
    for (uint8_t nCol = 0; nCol < 4; nCol++)
    {
      if ((mCurrentShape[Rotate(nCol, nRow, tmpShapeCursor.rot)] != 0)
      && ((tmpShapeCursor.col + nCol < 0) || (tmpShapeCursor.col + nCol > SCREEN_WIDTH - 1) || (tmpShapeCursor.row + nRow > SCREEN_HEIGHT -1)))
      {
        return false;
      }
    }
  }
  return true;
}

void aTRS_ENG_CheckClearedRows(void)
{
  __unitBlockPos blockPos;
  for (uint8_t i = 0; i < SCREEN_HEIGHT; i++)
  {
    uint8_t blockNum = 0;
    for (uint8_t j = 0; j < SCREEN_WIDTH; j++)
    {
      if (field[i*SCREEN_WIDTH + j])
      {
        blockNum++;
      }
    }
    if (blockNum == SCREEN_WIDTH)
    {
      //Render cleared row and update field array
      for (uint8_t k = 0; k < SCREEN_WIDTH; k++)
      {
        blockPos = getUnitBlockPos(i, k) ;
        LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
        blockPos.unitBlock_y1,
        blockPos.unitBlock_x2,
        blockPos.unitBlock_y2,
        BLACK);

        field[i*SCREEN_WIDTH + k] = 0;
      }

      //Shift each column down
      for (uint8_t l = 0; l < SCREEN_WIDTH; l++)
      {
        //loop through each row to the top row 1
        for (uint8_t m = i; m > 0; m--)
        {
          field[(m)*SCREEN_WIDTH + l] = field[(m-1)*SCREEN_WIDTH + l];
          blockPos = getUnitBlockPos(m, l);
          if (field[(m)*SCREEN_WIDTH + l])
          {
            LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
            blockPos.unitBlock_y1,
            blockPos.unitBlock_x2,
            blockPos.unitBlock_y2,
            RED);
          }
          else
          {
            LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
            blockPos.unitBlock_y1,
            blockPos.unitBlock_x2,
            blockPos.unitBlock_y2,
            BLACK);
          }
        }
      }
      aTRS_ENG_returnShapeCursorHome();
    }
  }
}

void aTRS_ENG_ClearField(void)
{
  for (uint8_t i = 0; i < SCREEN_HEIGHT; i++)
  {
    for (uint8_t j = 0; j < SCREEN_WIDTH; j++)
    {
      field[i*SCREEN_WIDTH + j] = 0;
    }
  }
}



/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
static uint8_t Rotate(uint8_t px, uint8_t py, uint8_t r)
{
  uint8_t tmp;
  switch (r % 4)
  {
    case 0:
    tmp = py * 4 + px;
    break;
    case 1:
    tmp = 12 + py - (px * 4);
    break;
    case 2:
    tmp = 15 - (py * 4) - px;
    break;
    case 3:
    tmp = 3 - py + (px * 4);
    break;
    default:
    break;
  }
  return tmp;
}

static __unitBlockPos getUnitBlockPos(uint8_t row, uint8_t col)
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


/************************************************************************/
/* Extra functions                                                      */
/************************************************************************/
void aTRS_ENG_refreshAllBlocks(uint16_t colour)
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