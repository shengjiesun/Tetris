/*
 * Buttons.c
 *
 * Created: 12/9/2019 11:10:12 PM
 *  Author: sunsh
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdbool.h>
#include "Buttons.h"
#include "ADC.h"

#define BUTTON_READ_NUM    5

 
static bool mButtonStatusNew;
static bool mButtonStatusOld;

static __Button_Status button;

void pDDRV_Button_init(void)
{

  DDRF &= ~((1<<PF0) | (1<<PF1) | (1<<PF2) | (1<<PF3) | (1<<PF4));
  PORTF |= ((1<<PF0) | (1<<PF1) | (1<<PF2) | (1<<PF3) | (1<<PF4));
  mButtonStatusNew = BUTTON_UNPRESSED;
  mButtonStatusOld = BUTTON_UNPRESSED;
}

uint8_t pDDRV_Button_readButton(__ButtonTypeDef buttonNum)
{
  switch (buttonNum)
  {
    case BUTTON_UP:
      if (PINF & (1<<PF0)) return 1;
      else return 0;
      break;
    case BUTTON_LEFT:
      if (PINF & (1<<PF1)) return 1;
      else return 0;
      break;
    case BUTTON_DOWN:
      if (PINF & (1<<PF2)) return 1;
      else return 0;
      break;
    case BUTTON_RIGHT:
      if (PINF & (1<<PF3)) return 1;
      else return 0;
      break;
    case BUTTON_ROTATE:
      if (PINF & (1<<PF4)) return 1;
      else return 0;
      break;
    default:
      break;
  }
  return 2;
}

__Button_Status pDDRV_Button_GetStatus(void)
{
  __Button_Status ButtonStatus;

  bool outputButtonStatus = 2;
  uint8_t mButtonStatusCount = 0;
  for (uint8_t NumBut = 0; NumBut < BUTTON_NUM; NumBut++)
  {
    
    //Check button status several times in a roll
    for (int i = 0; i < BUTTON_READ_NUM; i++)
    {
      if (pDDRV_Button_readButton(NumBut) == 0)
      {
        mButtonStatusCount++;
      }
    }

    //If button is low for all attempted reads, then it is pressed
    if (mButtonStatusCount >= BUTTON_READ_NUM)
    {
      mButtonStatusNew = BUTTON_PRESSED;
    }
    else
    {
      mButtonStatusNew = BUTTON_UNPRESSED;
      mButtonStatusOld = BUTTON_UNPRESSED;
    }

    //If the new button status is pressed then output status is pressed
    if ((mButtonStatusNew == BUTTON_PRESSED) && (mButtonStatusNew != mButtonStatusOld ))
    {
      outputButtonStatus = BUTTON_PRESSED;
      mButtonStatusOld = mButtonStatusNew;
    } 
    else
    {
      outputButtonStatus = BUTTON_UNPRESSED;
    }

    switch (NumBut)
    {
      case BUTTON_UP:
        ButtonStatus.up = outputButtonStatus;
        break;
      case BUTTON_LEFT:
        ButtonStatus.left = outputButtonStatus;
        break;
      case BUTTON_DOWN:
        ButtonStatus.down = outputButtonStatus;
        break;
      case BUTTON_RIGHT:
        ButtonStatus.right = outputButtonStatus;
        break;
      case BUTTON_ROTATE:
        ButtonStatus.rotate = outputButtonStatus;
        break;
      default:
        break;
    }
  }

  return ButtonStatus;
}