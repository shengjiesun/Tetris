/*
 * LCD_Driver.h
 *
 * Created: 12/3/2019 12:48:20 AM
 *  Author: sunsh
 */ 


#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

/************************************************************************/
/* Colour definition                                                    */
/************************************************************************/
#define WHITE       0xFFFF
#define BLACK      	0x0000
#define BLUE       	0x001F
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40
#define BRRED 			0XFC07
#define GRAY  			0X8430

#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LIGHTGRAY   0XEF5B
#define LGRAY 			0XC618

#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

/************************************************************************/
/* LCD screen dimension                                                 */
/************************************************************************/
#define LCD_W       320
#define LCD_H       480

/************************************************************************/
/* Structure definition                                                 */
/************************************************************************/
typedef struct
{
  uint16_t width;
  uint16_t height;
  uint16_t id;
  uint8_t  dir;
  uint16_t wramcmd;
  uint16_t setxcmd;
  uint16_t setycmd;
} LCD_Dev;

typedef enum
{
  LCD_ROTATE_0  = 0,
  LCD_ROTATE_90,
  LCD_ROTATE_180,
  LCD_ROTATE_270
} LCD_Rotation_t;



/************************************************************************/
/* Public function prototypes                                           */
/************************************************************************/
void LCD_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_RESET(void);
void LCD_WriteData_16Bit(uint16_t Data);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_direction(LCD_Rotation_t direction);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t colour);
void LCD_SetBackLight(uint8_t duty);

#endif /* LCD_DRIVER_H_ */