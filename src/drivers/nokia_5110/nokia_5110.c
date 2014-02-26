

#include "nokia_5110.h"
#include "write_5110.h"


/*-----------------------------------------------------------------------
LCD_init          : 3310LCD��ʼ��

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 
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
    LCD_RST = 1;                // ����һ����LCD��λ�ĵ͵�ƽ����
    

    LCD_CE = 0;                 // �ر�LCD
    delayus(1);
		
    LCD_CE = 1;                 // ʹ��LCD
    delayus(1);

    LCD_write_byte(0x21, 0);	// ʹ����չ��������LCDģʽ
    LCD_write_byte(0xc8, 0);	// ����ƫ�õ�ѹ
    LCD_write_byte(0x06, 0);	// �¶�У��
    LCD_write_byte(0x13, 0);	// 1:48
    LCD_write_byte(0x20, 0);	// ʹ�û�������
    LCD_clear();	            // ����
    LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
        
           
    LCD_CE = 0;                 // �ر�LCD
}

/*-----------------------------------------------------------------------
LCD_clear         : LCD��������,���ҽ�XY��������

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);
	LCD_set_XY(0,0);											//��XY��������
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��

���������X       ��0��83
          Y       ��0��5

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);								// column
    LCD_write_byte(0x80 | X, 0);          						// row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

���������c       ����ʾ���ַ���

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char row;

    c -= 32;

    for (row=0; row<6; row++)
      LCD_write_byte(font6x8[c][row], 1);
  }

/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 		
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
LCD_draw_map      : λͼ���ƺ���

���������X��Y    ��λͼ���Ƶ���ʼX��Y���ꣻ
          *map    ��λͼ�������ݣ�
          Pix_x   ��λͼ���أ�����
          Pix_y   ��λͼ���أ���

��д����          ��2004-8-13
����޸�����      ��2004-8-13 
-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y)
  {
    unsigned int i,n;
    unsigned char row;
    
    if (Pix_y%8==0) row=Pix_y/8;      					//����λͼ��ռ����
      else
        row=Pix_y/8+1;
    
    for (n=0;n<row;n++)
      {
      	LCD_set_XY(X,Y);
        for(i=0; i<Pix_x; i++)
          {
            LCD_write_byte(map[i+n*Pix_x], 1);
          }
        Y++;                         					//����
      }      
  }

/*-----------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD

���������data    ��д������ݣ�
          command ��д����/����ѡ��

��д����          ��2004-8-10 
����޸�����      ��2004-8-13 
-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
    LCD_CE = 0;
    
    if (command == 0)
     LCD_DC = 0;                //д����
    else
     LCD_DC = 1;                //д����
    
    
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
  
 
  
  
  
  
  /*=====================������ͼ����====================
��    �룺map  ͼ��������ַ
��    �ߣ���ʢƽ
�޸�ʱ�䣺2013-3-12
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
  
  





/*==============ͼ����ʾ���������ܳ������ã������һ�ֽ�дnokia5110��8���㣩========
LCD_draw_picture    : ��һ����άͼ�������nokia5110����ʾ

���������start_x     ����ʾͼ��Ŀ�ʼ���кţ�
          start_y    ����ʾͼ��Ŀ�ʼ���кţ�
          map       : ͼ�����飻
          row     : ������
          col      ��������0-5����

��д����          ��2013-6-29 
����޸�����      ��2013-6-29 
��������          ��2013-07-04
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
LCD_draw_pixel    : ��һ����άͼ�������nokia5110����ʾ

���������image    ��Ҫ��ʾͼ�������׵�ַ(ָ��)������������������
          row     ��ͼ�����������ص㣩��
          col      : ͼ�����������ص㣩��
          threshold: ��ֵ����ֵ������Ƕ�ֵ���Ժ��ͼ�񣬷�ֵΪ0����
          X        ����ʼ��ʾ��X���ꣻ
          Y        ����ʼ��ʾ��Y���꣨0-5����

��д����          ��2013-6-29 
����޸�����      ��2013-6-29 
��������          ��2013-07-04
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
LCD_draw_camera    : ��һ����άͼ�������nokia5110����ʾ,���ܳ�����ͷר�ã�
                     ����Ұ�𿪷���

���������image    ��Ҫ��ʾͼ�������׵�ַ(ָ��)������������������
          row     ��ͼ�����������ص㣩��
          col      : ͼ�����������ص㣩��
          threshold: ��ֵ����ֵ������Ƕ�ֵ���Ժ��ͼ�񣬷�ֵΪ0����
          X        ����ʼ��ʾ��X���ꣻ
          Y        ����ʼ��ʾ��Y���꣨0-5����

��д����          ��2013-9-21 
����޸�����      �� 
��������          ��
-----------------------------------------------------------------------*/
void LCD_draw_camera(void *imag,unsigned char row,
                     unsigned char col,unsigned char threshold,
                     unsigned char X,unsigned char Y)
{
    u8 i,j,k,temp;
    
    u8 *image = imag;
    
    LCD_set_XY(X,Y);               //����ͼ����ʼ����
    
    if(!(row%8))
    {
        for(k=0;k<row/8+1;k++)
        {
            LCD_set_XY(X+k,Y);         //����һ�к󣬶�����һ��ͼ�����ʼ���� 
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
            LCD_set_XY(X+k,Y);         //����һ�к󣬶�����һ��ͼ�����ʼ���� 
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

