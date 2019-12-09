/*
 * shapes.c
 *
 * Created: 12/8/2019 11:45:23 PM
 *  Author: sunsh
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "shapes.h"

uint8_t shape0[] = {  0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0,};

uint8_t shape1[] = {  0, 0, 1, 0,
                      0, 1, 1, 0,
                      0, 1, 0, 0,
                      0, 0, 0, 0,};

uint8_t shape2[] = {  0, 1, 0, 0,
                      0, 1, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 0,};

uint8_t shape3[] = {  0, 0, 0, 0,
                      0, 1, 1, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

uint8_t shape4[] = {  0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

uint8_t shape5[] = {  0, 1, 0, 0,
                      0, 1, 0, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

uint8_t shape6[] = {  0, 0, 1, 0,
                      0, 1, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 0,};

uint8_t Rotate(uint8_t px, uint8_t py, uint8_t r)
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

uint8_t* showShape(uint8_t shapeNum)
{
  switch (shapeNum)
  {
    case 0:
      return shape0;
    case 1:
      return shape1;
    case 2:
      return shape2;
    case 3:
      return shape3;
    case 4:
      return shape4;
    case 5:
      return shape5;
    case 6:
      return shape6;
    default:
      return shape0;
  }
}
