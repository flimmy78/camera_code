/*
2007-2-1 12:06
nokia 5110 driver program for 51 mcu
by zl0801

zhaoliang0801@gmail.com

*/


#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include "common.h"
#include "include.h"



// pin define for n5110lcd_8key board
// change this if your hardware changed!
//Nokia5110 引脚配置文件

#define __XPM__

//以下宏定义和枚举定义都和Nokia5110的引脚配置有关；
//请匹配好各自电路板

//Nokia5110 引脚配置文件
#ifdef __ZSP__
#define   PortSCLK        PORTA
#define   PortSDIN        PORTA
#define   PortLCD_DC      PORTA
#define   PortLCD_CE      PORTA
#define   PortLCD_RST     PORTA

#define   SCLK            (PTA29_OUT)		
#define   SDIN            (PTA28_OUT)		
#define   LCD_DC          (PTA27_OUT)		
#define   LCD_CE          (PTA26_OUT)		
#define   LCD_RST         (PTA24_OUT)	


enum LCD_Pin
{
   SCLK_Pin     = 29,		
   SDIN_Pin     = 28,		
   LCD_DC_Pin   = 27,		
   LCD_CE_Pin   = 26,		
   LCD_RST_Pin  = 24
};
#endif

#ifdef __TEST__
#define   PortSCLK        PORTC
#define   PortSDIN        PORTC
#define   PortLCD_DC      PORTC
#define   PortLCD_CE      PORTC
#define   PortLCD_RST     PORTC

#define   SCLK            (PTC4_OUT)		
#define   SDIN            (PTC6_OUT)		
#define   LCD_DC          (PTC8_OUT)		
#define   LCD_CE          (PTC10_OUT)		
#define   LCD_RST         (PTC12_OUT)	

enum LCD_Pin
{
   SCLK_Pin     = 4,		
   SDIN_Pin     = 6,		
   LCD_DC_Pin   = 8,		
   LCD_CE_Pin   = 10,		
   LCD_RST_Pin  = 12
};
#endif



#ifdef __LJ__
#define   PortSCLK        PORTD
#define   PortSDIN        PORTD
#define   PortLCD_DC      PORTD
#define   PortLCD_CE      PORTD
#define   PortLCD_RST     PORTD

#define   SCLK            (PTD11_OUT)		
#define   SDIN            (PTD12_OUT)		
#define   LCD_DC          (PTD13_OUT)		
#define   LCD_CE          (PTD14_OUT)		
#define   LCD_RST         (PTD15_OUT)	

enum LCD_Pin
{
   SCLK_Pin     = 11,		
   SDIN_Pin     = 12,		
   LCD_DC_Pin   = 13,		
   LCD_CE_Pin   = 14,		
   LCD_RST_Pin  = 15
};
#endif


#ifdef __XPM__
#define   PortSCLK        PORTB
#define   PortSDIN        PORTB
#define   PortLCD_DC      PORTB
#define   PortLCD_CE      PORTB
#define   PortLCD_RST     PORTB

#define   SCLK            (PTB22_OUT)		
#define   SDIN            (PTB20_OUT)		
#define   LCD_DC          (PTB18_OUT)		
#define   LCD_CE          (PTB16_OUT)		
#define   LCD_RST         (PTB10_OUT)	

enum LCD_Pin
{
   SCLK_Pin     = 22,		
   SDIN_Pin     = 20,		
   LCD_DC_Pin   = 18,		
   LCD_CE_Pin   = 16,		
   LCD_RST_Pin  = 10
};
#endif


void LCD_pin_config();
void LCD_init(void);
void LCD_clear(void);
void LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row);
void chinese_string(unsigned char X, unsigned char Y, unsigned char T);
void LCD_write_char(unsigned char c);
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_draw_whole(unsigned char *map);
void LCD_draw_picture(unsigned char start_x, unsigned char start_y,
                      unsigned char *map, unsigned char line, unsigned row);
void LCD_draw_pixel(void *image, unsigned char line, unsigned char row,
                    unsigned char threshold,unsigned char X, unsigned char Y);


void LCD_draw_camera(void *imag,unsigned char row,
                     unsigned char col,unsigned char threshold,
                     unsigned char X,unsigned char Y);


#endif