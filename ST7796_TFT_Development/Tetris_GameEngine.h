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
  uint8_t row;
  uint8_t col;
  uint8_t rot;
} __shapeCursor;

void drawFrame(void);
void refreshAllBlocks(uint16_t colour);
__unitBlockPos getUnitBlockPos(uint8_t row, uint8_t col);
void drawCurrentShape(uint8_t shapeNum, uint16_t colour);
void ShapeShiftUp(void);
void ShapeShiftDown(void);
void ShapeShiftRight(void);
void ShapeShiftLeft(void);
void moveShapeCursor(uint8_t row, uint8_t col);
void returnShapeCursorHome();

#endif /* TETRIS_GAMEENGINE_H_ */