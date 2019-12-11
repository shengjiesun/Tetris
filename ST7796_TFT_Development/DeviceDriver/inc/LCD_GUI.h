/*
 * LCD_GUI.h
 *
 * Created: 12/3/2019 11:27:37 PM
 *  Author: sunsh
 */ 


#ifndef LCD_GUI_H_
#define LCD_GUI_H_

void LCD_GUI_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_GUI_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t colour);
void LCD_GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
void LCD_GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
void LCD_GUI_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
void LCD_GUI_DrawCircle(int xc, int yc, int r, uint16_t colour);
void LCD_GUI_DrawDisc(int xc, int yc, int r, uint16_t colour);
void LCD_GUI_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size, uint8_t *p, uint16_t fc, uint16_t bc, uint8_t mode);
void LCD_ShowNum(uint16_t x, uint16_t y, uint8_t size, uint16_t fc, uint16_t bc, int num);
#endif /* LCD_GUI_H_ */