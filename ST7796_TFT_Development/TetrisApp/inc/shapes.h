/*
 * shapes.h
 *
 * Created: 12/8/2019 11:31:25 PM
 *  Author: sunsh
 */ 

#include <avr/io.h>
#include <stdlib.h>

#ifndef SHAPES_H_
#define SHAPES_H_

#define UNIT_BLOCK_SIZE 14



uint8_t Rotate(uint8_t px, uint8_t py, uint8_t r);
uint8_t* showShape(uint8_t shapeNum);


#endif /* SHAPES_H_ */