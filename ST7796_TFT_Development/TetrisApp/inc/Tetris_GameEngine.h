/*
 * Tetris_GameEngine.h
 *
 * Created: 12/8/2019 10:43:48 PM
 *  Author: sunsh
 */ 


#ifndef TETIRS_GAMEENGINE_H_
#define TETIRS_GAMEENGINE_H_

#define SCREEN_WIDTH  14
#define SCREEN_HEIGHT 20

typedef struct 
{
  uint16_t frame_x1;
  uint16_t frame_y1;
  uint16_t frame_x2;
  uint16_t frame_y2;
  uint16_t frame_thickness;
  uint16_t frame_colour;
  uint16_t frame_bg;
} __tetrisFrame;

typedef struct 
{
  uint16_t unitBlock_x1;
  uint16_t unitBlock_y1;
  uint16_t unitBlock_x2;
  uint16_t unitBlock_y2;
} __unitBlockPos;

typedef struct
{
  uint16_t num0;
  uint16_t num1;
  uint16_t num2;
  uint16_t num3;
  uint16_t num4;
  uint16_t num5;
  uint16_t num6;
  uint16_t num7;
  uint16_t num8;
  uint16_t num9;
  uint16_t num10;
  uint16_t num11;
  uint16_t num12;
  uint16_t num13;
  uint16_t num14;
  uint16_t num15;
} __currentShape;

typedef struct  
{
  int8_t row;
  int8_t col;
  uint8_t rot;
} __shapeCursor;

void aTRS_ENG_drawBoundary(void);
void aTRS_ENG_refreshAllBlocks(uint16_t colour);
void aTRS_ENG_drawCurrentShape(uint16_t colour);
void aTRS_ENG_ShapeShiftUp(void);
void aTRS_ENG_ShapeShiftDown(void);
void aTRS_ENG_ShapeShiftRight(void);
void aTRS_ENG_ShapeShiftLeft(void);
void aTRS_ENG_moveShapeCursor(uint8_t row, uint8_t col);
void aTRS_ENG_returnShapeCursorHome();

bool aTRS_ENG_BoundaryCheck_Translate(uint8_t direction);
bool aTRS_ENG_BoundaryCheck_Rotate(void);

uint8_t* aTRS_ENG_GenerateNewShape(void);
void aTRS_ENG_CheckClearedRows(void);
#endif /* TETRIS_GAMEENGINE_H_ */