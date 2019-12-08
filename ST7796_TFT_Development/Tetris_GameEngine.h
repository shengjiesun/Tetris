/*
 * Tetris_GameEngine.h
 *
 * Created: 12/8/2019 10:43:48 PM
 *  Author: sunsh
 */ 


#ifndef TETIRS_GAMEENGINE_H_
#define TETIRS_GAMEENGINE_H_

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

void drawFrame(void);




#endif /* TETRIS_GAMEENGINE_H_ */