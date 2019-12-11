/*
 * Buttons.h
 *
 * Created: 12/9/2019 11:13:56 PM
 *  Author: sunsh
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_PRESSED     false
#define BUTTON_UNPRESSED   true

typedef enum
{
  BUTTON_UP = 0,
  BUTTON_LEFT,
  BUTTON_DOWN,
  BUTTON_RIGHT,
  BUTTON_ROTATE,
  BUTTON_NUM
} __ButtonTypeDef;

typedef struct 
{
  uint8_t up;
  uint8_t left;
  uint8_t down;
  uint8_t right;
  uint8_t rotate;
} __Button_Status;

void pDDRV_Button_init(void);
uint8_t pDDRV_Button_readButton(__ButtonTypeDef buttonNum);
__Button_Status pDDRV_Button_GetStatus(void);

#endif /* BUTTONS_H_ */