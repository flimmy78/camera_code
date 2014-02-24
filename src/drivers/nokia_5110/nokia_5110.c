

#include "nokia_5110.h"
#include "write_5110.h"


/*-----------------------------------------------------------------------
LCD_init          : 3310LCD初始化

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
-----------------------------------------------------------------------*/

void LCD_pin_config()
{
    gpio_init(PortSCLK, SCLK_Pin,GPO,HIGH);
    gpio_init(PortSDIN, SDIN_Pin,GPO,HIGH);
    gpio_init(PortLCD_DC, LCD_DC_Pin,GPO,HIGH);
    gpio_init(PortLCD_CE, LCD_CE_Pin,GPO,HIGH);
    gpio_init(PortLCD_RST, LCD_RST_Pin,GPO,HIGH);
}

void LCD_init(void)
{
            
    LCD_pin_config();
   
    LCD_RST = 0;
    delayus(1);    
    LCD_RST = 1;                // 产生一个让LCD复位的低电平脉冲
    

    LCD_CE = 0;                 // 关闭LCD
    delayus(1);
		
    LCD_CE = 1;                 // 使能LCD
    delayus(1);

    LCD_write_byte(0x21, 0);	// 使用扩展命令设置LCD模式
    LCD_write_byte(0xc8, 0);	// 设置偏置电压
    LCD_write_byte(0x06, 0);	// 温度校正
    LCD_write_byte(0x13, 0);	// 1:48
    LCD_write_byte(0x20, 0);	// 使用基本命令
    LCD_clear();	            // 清屏
    LCD_write_byte(0x0c, 0);	// 设定显示模式，正常显示
        
           
    LCD_CE = 0;                 // 关闭LCD
}

/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数,并且将XY坐标清零

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);
	LCD_set_XY(0,0);											//将XY坐标清零
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数

输入参数：X       ：0－83
          Y       ：0－5

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);								// column
    LCD_write_byte(0x80 | X, 0);          						// row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符

输入参数：c       ：显示的字符；

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char row;

    c -= 32;

    for (row=0; row<6; row++)
      LCD_write_byte(font6x8[c][row], 1);
  }

/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 		
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
  }



/*-----------------------------------------------------------------------
LCD_draw_map      : 位图绘制函数

输入参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）

编写日期          ：2004-8-13
最后修改日期      ：2004-8-13 
-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y)
  {
    unsigned int i,n;
    unsigned char row;
    
    if (Pix_y%8==0) row=Pix_y/8;      					//计算位图所占行数
      else
        row=Pix_y/8+1;
    
    for (n=0;n<row;n++)
      {
      	LCD_set_XY(X,Y);
        for(i=0; i<Pix_x; i++)
          {
            LCD_write_byte(map[i+n*Pix_x], 1);
          }
        Y++;                         					//换行
      }      
  }

/*-----------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD

输入参数：data    ：写入的数据；
          command ：写数据/命令选择；

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-13 
-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
    LCD_CE = 0;
    
    if (command == 0)
     LCD_DC = 0;                //写命令
    else
     LCD_DC = 1;                //写数据
    
    
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
			SDIN = 1;
		else
            SDIN = 0;
            SCLK = 0;
			dat = dat << 1;
			SCLK = 1;
	}

     LCD_CE = 1;
  }
  
 
  
  
  
  
  /*=====================整屏画图函数====================
输    入：map  图像向量地址
作    者：张盛平
修改时间：2013-3-12
======================================================*/  
  
void LCD_draw_whole(unsigned char * map)
  {
	  int i;
	  
	  LCD_clear();
	  
	  
	  for(i=0;i<504;i++)
	  {
		  LCD_write_byte(*map++,1);
	  }
  }
  
  





/*==============图像显示函数（智能车不常用，数组的一字节写nokia5110的8个点）========
LCD_draw_picture    : 将一个二维图像矩阵在nokia5110中显示

输入参数：start_x     ：显示图像的开始的行号；
          start_y    ：显示图像的开始的列号；
          map       : 图像数组；
          row     : 行数；
          col      ：列数（0-5）；

编写日期          ：2013-6-29 
最后修改日期      ：2013-6-29 
加入日期          ：2013-07-04
================================================================================*/
void LCD_draw_picture(unsigned char start_x, unsigned char start_y, 
                      unsigned char *map, unsigned char row, unsigned col)
  {
        unsigned char *p;
        unsigned i,j;
        p= map;
        for(i=0;i<row;i++)
        {
              LCD_set_XY(start_x,start_y+i);
              for(j=0;j<col;j++)
              {
                    LCD_write_byte(*p++,1);
              }
        }
  }
  
  
  
  
  
  
  
/*-----------------------------------------------------------------------
LCD_draw_pixel    : 将一个二维图像矩阵在nokia5110中显示

输入参数：image    ：要显示图像矩阵的首地址(指针)（可以是数组名）；
          row     ：图像行数（像素点）；
          col      : 图像列数（像素点）；
          threshold: 二值化阀值（如果是二值化以后的图像，阀值为0）；
          X        ：开始显示的X坐标；
          Y        ：开始显示的Y坐标（0-5）；

编写日期          ：2013-6-29 
最后修改日期      ：2013-6-29 
加入日期          ：2013-07-04
-----------------------------------------------------------------------*/
//  void LCD_draw_pixel(void *image, unsigned char row, unsigned char col,
//                    unsigned char threshold,unsigned char X, unsigned char Y)
//   {
//      unsigned char *p_image, i, j, k, temp,n;
//      p_image = image;
//      n=row/8;
//      for(i=0;i<n;i++)
//      {
//            LCD_set_XY(0+X,i+Y);
//            for(j=0;j<col;j++)
//            {
//                  for(k=0;k<8;k++)
//                  {
//                       ((*(p_image+(i*8+k)*col+j))>threshold) ? 
//                                    (temp &= ~(1<<k)) : (temp |= 1<<k);
//                  }
//                  LCD_write_byte(temp,1);
//            }
//      }
//      if(row%8)
//            {
//                  for(j=0;j<col;j++)
//                        {
//                              for(k=0;k<row%8;k++)
//                              {
//                                    ((*(p_image+(i*8+k)*col+j))>threshold) ? 
//                                    (temp &= ~(1<<k)) : (temp |= 1<<k);
//                              }
//                              LCD_write_byte(temp,1);
//                        }
//            }
//    }
//  

  void LCD_draw_pixel(void *image, unsigned char line, unsigned char row,
                    unsigned char threshold,unsigned char X, unsigned char Y)
   {
      unsigned char *p_image, i, j, k, temp,n;
      p_image = image;
      n=line/8;
      for(i=0;i<n;i++)
      {
            LCD_set_XY(0+X,i+Y);
            for(j=0;j<row;j++)
            {
                  for(k=0;k<8;k++)
                  {
                       ((*(p_image+(i*8+k)*row+j))>threshold) ? 
                                     (temp &= ~(1<<k)):(temp |= 1<<k) ;
                  }
                  LCD_write_byte(temp,1);
            }
      }
      if(line%8)
            {
                  for(j=0;j<row;j++)
                        {
                              for(k=0;k<line%8;k++)
                              {
                                    ((*(p_image+(i*8+k)*row+j))>threshold) ? 
                                    (temp &= ~(1<<k)) : (temp |= 1<<k);
                              }
                              LCD_write_byte(temp,1);
                        }
            }
    }













/*-----------------------------------------------------------------------
LCD_draw_camera    : 将一个二维图像矩阵在nokia5110中显示,智能车摄像头专用，
                     基于野火开发库

输入参数：image    ：要显示图像矩阵的首地址(指针)（可以是数组名）；
          row     ：图像行数（像素点）；
          col      : 图像列数（像素点）；
          threshold: 二值化阀值（如果是二值化以后的图像，阀值为0）；
          X        ：开始显示的X坐标；
          Y        ：开始显示的Y坐标（0-5）；

编写日期          ：2013-9-21 
最后修改日期      ： 
加入日期          ：
-----------------------------------------------------------------------*/
void LCD_draw_camera(void *imag,unsigned char row,
                     unsigned char col,unsigned char threshold,
                     unsigned char X,unsigned char Y)
{
    u8 i,j,k,temp;
    
    u8 *image = imag;
    
    LCD_set_XY(X,Y);               //设置图像起始坐标
    
    if(!(row%8))
    {
        for(k=0;k<row/8+1;k++)
        {
            LCD_set_XY(X+k,Y);         //画完一行后，定义下一行图像的起始坐标 
            for(j=0;j<col;j++)
            {
                temp = 0;
                for(i=k*8;i<k*8+8;i++)
                {
                    if(*(image+i*col+j)<threshold)
                    {
                        temp = temp|(1<<(i-k*8));
                    }
                }
                LCD_write_byte(temp,1);
            }
        }
    }
    else
    {
        for(k=0;k<row/8;k++)
        {
            LCD_set_XY(X+k,Y);         //画完一行后，定义下一行图像的起始坐标 
            for(j=0;j<col;j++)
            {
                temp = 0;
                for(i=k*8;i<k*8+8;i++)
                {
                    if(*(image+i*col+j)<threshold)
                    {
                        temp = temp|(1<<(i-k*8));
                    }
                }
                LCD_write_byte(temp,1);
            }
        }
        
        
            LCD_set_XY(X+k,Y);
            temp = 0;
            for(i=k*8;i<k*8+row-row/8*8;i++)
            {
                if(*(image+i*col+j)<threshold)
                {
                    temp = temp|(1<<i-k*8);
                }
            }
            LCD_write_byte(temp,1);
    }

}

