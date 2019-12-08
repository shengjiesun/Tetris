/*
 * LCD_GUI.c
 *
 * Created: 12/3/2019 10:51:24 PM
 *  Author: sunsh
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include "LCD_Driver.h"
#include "LCD_GUI.h"
#include "font.h"

extern LCD_Dev lcddev;

/************************************************************************/
/* Private function prototypes                                          */
/************************************************************************/
static void drawCircle8(int xc, int yc, int x, int y, uint16_t colour);
static void swap(uint16_t *a, uint16_t *b);
static uint32_t mypow(uint8_t m, uint8_t n);

/************************************************************************/
/* Public function prototypes                                           */
/************************************************************************/

/*************************Geometric Shapes*******************************/

void LCD_GUI_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_SetCursor(x,y);
  LCD_WriteData_16Bit(color);
}

void LCD_GUI_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t colour)
{
  if (ex < sx)
  {
    swap(&ex, &sx);
  }
  if (ey < sy)
  {
    swap(&ey, &sy);
  }

  uint16_t i,j;
  uint16_t width=ex-sx+1;
  uint16_t height=ey-sy+1;
  LCD_SetWindows(sx,sy,ex,ey);
  for(i=0;i<height;i++)
  {
    for(j=0;j<width;j++)
    LCD_WriteData_16Bit(colour);
  }
}

void LCD_GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
  uint16_t t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;

  delta_x=x2-x1;
  delta_y=y2-y1;
  uRow=x1;
  uCol=y1;
  if(delta_x>0)incx=1;
  else if(delta_x==0)incx=0;
  else {incx=-1;delta_x=-delta_x;}
  if(delta_y>0)incy=1;
  else if(delta_y==0)incy=0;
  else{incy=-1;delta_y=-delta_y;}
  if( delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  for(t=0;t<=distance+1;t++ )
  {
    LCD_GUI_DrawPoint(uRow, uCol, colour);
    xerr+=delta_x ;
    yerr+=delta_y ;
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

void LCD_GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
  LCD_GUI_DrawLine(x1, y1, x2, y1, colour);
  LCD_GUI_DrawLine(x1, y1, x1, y2, colour);
  LCD_GUI_DrawLine(x1, y2, x2, y2, colour);
  LCD_GUI_DrawLine(x2, y1, x2, y2, colour);
}

void LCD_GUI_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
  LCD_GUI_Fill(x1, y1, x2, y2, colour);
}

void LCD_GUI_DrawCircle(int xc, int yc, int r, uint16_t colour)
{
  int x = 0, y = r, yi, d;
  d = 3 - 2 * r;

  while (x <= y)
  {
    drawCircle8(xc, yc, x, y, colour);
    if (d < 0) {
      d = d + 4 * x + 6;
      } else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void LCD_GUI_DrawDisc(int xc, int yc, int r, uint16_t colour)
{
  int x = 0, y = r, yi, d;
  d = 3 - 2 * r;

  while (x <= y) 
  {
    for (yi = x; yi <= y; yi++)
    drawCircle8(xc, yc, x, yi, colour);

    if (d < 0) 
    {
      d = d + 4 * x + 6;
      } else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void LCD_GUI_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
  LCD_GUI_DrawLine(x0,y0,x1,y1, colour);
  LCD_GUI_DrawLine(x1,y1,x2,y2, colour);
  LCD_GUI_DrawLine(x2,y2,x0,y0, colour);
}

/*************************Strings****************************************/

void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp;
  uint8_t pos, t;
  num=num-' ';
  const unsigned char* fontArray = Aria16x8;

  uint8_t charWidth = fontArray[1];
  uint8_t charHeight = fontArray[2];


  LCD_SetWindows(x,y,x+charWidth*size-1,y+charHeight*size-1);
  for(pos=0;pos<charHeight*size;pos++)
  {
    temp=Aria16x8[(num*charHeight + pos/size) + FONT_INFO_BYTE];
    for(t=0;t<charWidth*size;t++)
    {
      if(temp & (1<<(7-t/size)))LCD_WriteData_16Bit(fc);
      else LCD_WriteData_16Bit(bc);
    }
  }
}

void LCD_ShowCharLarge(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp;
  uint8_t pos, t;
  num=num-' ';
  const unsigned char* fontArray = Aria80x40;

  uint8_t charWidth = fontArray[1];
  uint8_t charHeight = fontArray[2];


  LCD_SetWindows(x,y,x+charWidth*size-1,y+charHeight*size-1);
  for(pos=0;pos<charHeight*size;pos++)
  {
    temp=fontArray[(num*charHeight + pos/size) + FONT_INFO_BYTE];
    for(t=0;t<8;t++)
    {
      if(temp & (1<<(7-t)))LCD_WriteData_16Bit(fc);
      else LCD_WriteData_16Bit(bc);
    }
    temp=fontArray[(num*charHeight + pos/size + 1) + FONT_INFO_BYTE];
    for(t=0;t<8;t++)
    {
      if(temp & (1<<(7-t)))LCD_WriteData_16Bit(fc);
      else LCD_WriteData_16Bit(bc);
    }
  }
}

// void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode)
// {
//   uint8_t temp;
//   uint8_t pos, t;
// 
//   num=num-' ';
//   LCD_SetWindows(x,y,x+8*size-1,y+16*size-1);
//   for(pos=0;pos<16*size;pos++)
//   {
//     temp=asc2_1608[num][pos/size];
//     for(t=0;t<8*size;t++)
//     {
//       if(temp & (1<<(t/size)))LCD_WriteData_16Bit(fc);
//       else LCD_WriteData_16Bit(bc);
//     }
//   }
// 
//   LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
// }

void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size, uint8_t *p, uint16_t fc, uint16_t bc, uint8_t mode)
{
  while((*p<='~')&&(*p>=' '))
  {
    if(x>(lcddev.width-1)||y>(lcddev.height-1))
    return;
    LCD_ShowChar(x,y,fc,bc,*p,size,mode);
    x+=8*size;
    p++;
  }
}

void LCD_ShowNum(uint16_t x, uint16_t y, uint8_t size, uint16_t fc, uint16_t bc, int num)
{
  uint8_t value[15];
  sprintf(value, "%d", num);
  LCD_ShowString(x, y, size, value, fc, bc, 0);
}




/************************************************************************/
/* Private function definitions                                         */
/************************************************************************/
static void drawCircle8(int xc, int yc, int x, int y, uint16_t colour)
{
  LCD_DrawPoint(xc + x, yc + y, colour);
  LCD_DrawPoint(xc - x, yc + y, colour);
  LCD_DrawPoint(xc + x, yc - y, colour);
  LCD_DrawPoint(xc - x, yc - y, colour);
  LCD_DrawPoint(xc + y, yc + x, colour);
  LCD_DrawPoint(xc - y, yc + x, colour);
  LCD_DrawPoint(xc + y, yc - x, colour);
  LCD_DrawPoint(xc - y, yc - x, colour);
}

static void swap(uint16_t *a, uint16_t *b)
{
  uint16_t tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

static uint32_t mypow(uint8_t m, uint8_t n)
{
  uint32_t result=1;
  while(n--)result*=m;
  return result;
}