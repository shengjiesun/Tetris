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
void drawUnitBlock(__unitBlockPos block, uint16_t colour);
static __unitBlockPos getNextShapeUnitBlockPos(uint16_t x1, uint16_t y1, uint8_t row, uint8_t col);

/************************************************************************/
/* Private variables                                                    */
/************************************************************************/
static const uint8_t shape0[] = {0, 0, 0, 0,\
                                 1, 1, 1, 1,\
                                 0, 0, 0, 0,\
                                 0, 0, 0, 0,};

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

static const uint16_t shapeColour[8] =
{
  RED,
  GREEN,
  YELLOW,
  MAGENTA,
  BLUE,
  GRAY,
  BROWN,
  CYAN,
};

static const uint8_t* mShapeArray[7] = {shape0, shape1, shape2, shape3, shape4, shape5, shape6};

static uint8_t *field[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};
static uint8_t* mCurrentShape = shape0;
static uint8_t* mNextShape = NULL;
static uint8_t mNextShapeIndex = 0;
static uint16_t mScore = 0;
static int8_t mMoveDirection = MOVE_NULL;
static bool newGame = true;

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
  .rot = 0,
  .colour = RED
};

static __shapeCursor mPreviousCursor =
{
  .row = 0,
  .col = 5,
  .rot = 0,
  .colour = RED
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
  if (!newGame)
  {
    mCurrentShape = mNextShape;
  }
  mNextShapeIndex = rand() % 7;
  mNextShape = mShapeArray[mNextShapeIndex];
  uint8_t newColourNum = rand() % 8;
    
  mShapeCursor.colour = shapeColour[newColourNum]; 
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

  newGame = false;
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
  mMoveDirection = MOVE_UP;
  mPreviousCursor = mShapeCursor;
  mShapeCursor.row--;
}

void aTRS_ENG_ShapeShiftDown(void)
{
  mMoveDirection = MOVE_DOWN;
  mPreviousCursor = mShapeCursor;
  mShapeCursor.row++;
}

void aTRS_ENG_ShapeShiftRight(void)
{
  mMoveDirection = MOVE_RIGHT;
  mPreviousCursor = mShapeCursor;
  mShapeCursor.col++;
}

void aTRS_ENG_ShapeShiftLeft(void)
{
  mMoveDirection = MOVE_LEFT;
  mPreviousCursor = mShapeCursor;
  mShapeCursor.col--;
}

void aTRS_ENG_ShapeRotate(void)
{
  mMoveDirection = MOVE_ROTATE;
  mPreviousCursor = mShapeCursor;
  mShapeCursor.rot++;
  if (mShapeCursor.rot >= 4) 
  {
    mShapeCursor.rot = 0;
  }
}

void aTRS_ENG_UpdateCurrentShape(void)
{
  uint8_t rowPos = mPreviousCursor.row;
  uint8_t colPos = mPreviousCursor.col;
  __unitBlockPos blockPos;
  uint16_t mColour;

  if ((mMoveDirection != MOVE_ROTATE) && (mMoveDirection != MOVE_NULL))
  {
    for (uint8_t i = 0; i < 4; i++)
    { 
      if (mMoveDirection == MOVE_LEFT)
      {
        blockPos = getUnitBlockPos(rowPos + i, colPos+3);
      }

      if (mMoveDirection == MOVE_DOWN)
      {
        blockPos = getUnitBlockPos(rowPos, colPos + i);
      }

      if (mMoveDirection == MOVE_RIGHT)
      {
        blockPos = getUnitBlockPos(rowPos + i, colPos);
      }

      if (mMoveDirection == MOVE_UP)
      {
        blockPos = getUnitBlockPos(rowPos + 3, colPos + i);
      }

      drawUnitBlock(blockPos, BLACK);
    }
  }

  for (uint8_t i = 0; i < 4; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      if (mCurrentShape[Rotate(j, i, mShapeCursor.rot)]) mColour = mShapeCursor.colour;
      else mColour = BLACK;

      if ((mShapeCursor.col + j >= 0) && (mShapeCursor.col + j < SCREEN_WIDTH) && (mShapeCursor.row + i < SCREEN_HEIGHT) && (mShapeCursor.row + i >= 0)) //Check if shape is out of field
      {
        if (field[(mShapeCursor.row + i) * SCREEN_WIDTH + (mShapeCursor.col + j)] == 0) //Check if field pos is blank
        {
          
          if (mMoveDirection == MOVE_LEFT)
          {
            if ((j > 0) && (mCurrentShape[Rotate(j, i, mShapeCursor.rot)] != mCurrentShape[Rotate(j-1, i, mPreviousCursor.rot)]))
            {
              blockPos = getUnitBlockPos(rowPos, colPos) ;
              drawUnitBlock(blockPos, mColour);
            }
          }
          else if (mMoveDirection == MOVE_RIGHT)
          {
            if ((j < 3) && (mCurrentShape[Rotate(j, i, mShapeCursor.rot)] != mCurrentShape[Rotate(j+1, i, mPreviousCursor.rot)]))
            {
              blockPos = getUnitBlockPos(rowPos, colPos) ;
              drawUnitBlock(blockPos, mColour);
            }
          }
          else if (mMoveDirection == MOVE_DOWN)
          {
            if ((i < 3) && (mCurrentShape[Rotate(j, i, mShapeCursor.rot)] != mCurrentShape[Rotate(j, i+1, mPreviousCursor.rot)]))
            {
              blockPos = getUnitBlockPos(rowPos, colPos) ;
              drawUnitBlock(blockPos, mColour);
            }
          }
          else if (mMoveDirection == MOVE_UP)
          {
            if ((i > 0) && (mCurrentShape[Rotate(j, i, mShapeCursor.rot)] != mCurrentShape[Rotate(j, i-1, mPreviousCursor.rot)]))
            {
              blockPos = getUnitBlockPos(rowPos, colPos) ;
              drawUnitBlock(blockPos, mColour);
            }            
          }
          else
          {          
            blockPos = getUnitBlockPos(rowPos, colPos) ;
            
            drawUnitBlock(blockPos, mColour);
          }
        }
      }

      colPos++;
    }
    colPos = mShapeCursor.col;
    rowPos++;
  }
  
  //aTRS_ENG_drawCurrentShape(GREEN);
  mMoveDirection = MOVE_NULL;
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

      if ((mShapeCursor.col + j >= 0) && (mShapeCursor.col + j < SCREEN_WIDTH) && (mShapeCursor.row + i < SCREEN_HEIGHT) && (mShapeCursor.row + i >= 0)) //Check if shape is out of field
      {
        if (field[(mShapeCursor.row + i) * SCREEN_WIDTH + (mShapeCursor.col + j)] == 0) //Check if field pos is blank
        {
            blockPos = getUnitBlockPos(rowPos, colPos) ;
            
            drawUnitBlock(blockPos, mColour);
        }
      }

      colPos++;
    }
    colPos = mShapeCursor.col;
    rowPos++;
  }
}

void aTRS_ENG_drawNextShape(uint16_t x, uint16_t y, uint16_t colour)
{
  uint8_t rowPos = 0;
  uint8_t colPos = 0;
  
  __unitBlockPos blockPos;
  uint16_t mColour;
  
  for (uint8_t i = 0; i < 4; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      if (mNextShape[Rotate(j, i, 0)]) mColour = colour;
      else mColour = BLACK;
          blockPos = getNextShapeUnitBlockPos(x, y, rowPos, colPos) ;
          
          drawUnitBlock(blockPos, mColour);

      colPos++;
    }
    colPos = 0;
    rowPos++;
  }
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
        drawUnitBlock(blockPos, RED);
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
    case 0:
      tmpShapeCursor.row--;
      break;
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
      && ((tmpShapeCursor.col + nCol < 0) || (tmpShapeCursor.col + nCol > SCREEN_WIDTH - 1) || (tmpShapeCursor.row + nRow < 0) || (tmpShapeCursor.row + nRow > SCREEN_HEIGHT -1)))
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
      && ((tmpShapeCursor.col + nCol < 0) || (tmpShapeCursor.col + nCol > SCREEN_WIDTH - 1) || (tmpShapeCursor.row + nRow < 0) || (tmpShapeCursor.row + nRow > SCREEN_HEIGHT -1)))
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
  uint8_t clearedRowNum = 0;
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
          uint8_t tmpBlock = field[(m-1)*SCREEN_WIDTH + l];
          blockPos = getUnitBlockPos(m, l);
          if (tmpBlock && !field[(m)*SCREEN_WIDTH + l])
          {
            drawUnitBlock(blockPos, RED);
          }
          else if (!tmpBlock && field[(m)*SCREEN_WIDTH + l])
          {
            LCD_GUI_DrawFillRectangle(blockPos.unitBlock_x1,
            blockPos.unitBlock_y1,
            blockPos.unitBlock_x2,
            blockPos.unitBlock_y2,
            BLACK);
          }
          field[(m)*SCREEN_WIDTH + l] = field[(m-1)*SCREEN_WIDTH + l];
        }
      }
      clearedRowNum++;
      mScore += 100*clearedRowNum;
    }
    aTRS_ENG_returnShapeCursorHome();
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

uint16_t aTRS_ENG_GetScore(void)
{
  return mScore;
}

void aTRS_ENG_GameOver(void)
{
  LCD_ShowString(60, 100, 4, "GAME", WHITE, BLACK, 0);
  LCD_ShowString(60, 150, 4, "OVER", WHITE, BLACK, 0);
  _delay_ms(1000);
  aTRS_ENG_refreshAllBlocks(WHITE);
  aTRS_ENG_ClearField();
  LCD_GUI_DrawFillRectangle(TetrisFrame.frame_x1+1,
                            TetrisFrame.frame_y1+1,
                            TetrisFrame.frame_x2-1,
                            TetrisFrame.frame_y2-2,
                            BLACK);
  aTRS_ENG_returnShapeCursorHome();
  mScore = 0;

  aTRS_ENG_GenerateNewShape();
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

static __unitBlockPos getNextShapeUnitBlockPos(uint16_t x1, uint16_t y1, uint8_t row, uint8_t col)
{
  __unitBlockPos blockPos =
  {
    .unitBlock_x1 = x1 +(col*UNIT_BLOCK_SIZE + col) + 1,
    .unitBlock_y1 = y1 + (row*UNIT_BLOCK_SIZE + row) + 1,
    .unitBlock_x2 = x1 +(col*UNIT_BLOCK_SIZE + col) + UNIT_BLOCK_SIZE,
    .unitBlock_y2 = y1 + (row*UNIT_BLOCK_SIZE + row) + UNIT_BLOCK_SIZE,
  };
  return blockPos;
}

void drawUnitBlock(__unitBlockPos block, uint16_t colour)
{
    LCD_GUI_DrawFillRectangle(block.unitBlock_x1+1,
                              block.unitBlock_y1+1,
                              block.unitBlock_x2-1,
                              block.unitBlock_y2-2,
                              colour);

    LCD_GUI_DrawLine(block.unitBlock_x1+1, block.unitBlock_y2-1, block.unitBlock_x1+2, block.unitBlock_y2-1, colour);
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
      _delay_ms(10);
    }
  }
}