/*
 * shapes.c
 *
 * Created: 12/8/2019 11:45:23 PM
 *  Author: sunsh
 */ 

#include <avr/io.h>
#include <stdlib.h>

const uint8_t shape0[] = {  0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0,};

const uint8_t shape1[] = {  0, 0, 1, 0,
                      0, 1, 1, 0,
                      0, 1, 0, 0,
                      0, 0, 0, 0,};

const uint8_t shape2[] = {  0, 1, 0, 0,
                      0, 1, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 0,};

const uint8_t shape3[] = {  0, 0, 0, 0,
                      0, 1, 1, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

const uint8_t shape4[] = {  0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

const uint8_t shape5[] = {  0, 1, 0, 0,
                      0, 1, 0, 0,
                      0, 1, 1, 0,
                      0, 0, 0, 0,};

const uint8_t shape6[] = {  0, 0, 1, 0,
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
  uint8_t* tmp;
  switch (shapeNum)
  {
    case 0:
      tmp = shape0;
      break;
    case 1:
      tmp = shape1;
      break;
    case 2:
      tmp = shape2;
      break;
    case 3:
      tmp = shape3;
      break;
    case 4:
      tmp = shape4;
      break;
    case 5:
      tmp = shape5;
      break;
    case 6:
      tmp = shape6;
      break;
    default:
      tmp = shape0;
      break;
  }
  return tmp;
}
