/**********************************************
*   File£ºNokia5110.h
*   Description£ºNokia5110LCDÇý¶¯³ÌÐò
*	  Creared Date: 2011-5-29
*	  Last Modified: 2011-5-29
*   Author: GaoQuan
*   Notes: None
**********************************************/
#ifndef __NOKIA5110_H
#define __NOKIA5110_H

void LCD_init(void);
void LCD_clear(void);
void LCD_write_ASCII_string(unsigned char Y,unsigned char X,char *s);
void LCD_write_ASCII_string_I(unsigned char Y,unsigned char X,char *s);               
void LCD_write_char(unsigned char c);
void LCD_write_char_I(unsigned char c);
void LCD_write_ASCII_char(unsigned char Y,unsigned char X,unsigned char c);
void LCD_write_ASCII_char_I(unsigned char Y,unsigned char X,unsigned char c);
void LCD_draw_bmp_pixel(u8 X,u8 Y,u8 *map,u8 Pix_x,u8 Pix_y);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_set_XY(unsigned char X, unsigned char Y);
void delay_1us(void);
void NumToStr( float number,char *p);
void LCD_write_hanzi(unsigned int row, unsigned int page,unsigned int num,u8 *hanzi); 


#endif /* __NOKIA5110_H */