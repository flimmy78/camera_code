/**********************************************
*   File��Nokia5110.c
*   Description��Nokia5110LCD��������
*	  Creared Date: 2011-5-29
*	  Last Modified: 2011-5-29
*   Author: GaoQuan
*   Notes: BL Ϊ���⣬��VCC������
**********************************************/
#include "common.h"
#include "nokia5110.h"
#include "ASCII.h"


//#define Nokia5110_SCE_DDR         DDRA_DDRA0
#define Nokia5110_SCE             GPIOE_PDOR //6Ƭѡ
//#define Nokia5110_RST_DDR         DDRA_DDRA1
#define Nokia5110_RST             GPIOE_PDOR//8��λ,0��λ6
//#define Nokia5110_DC_DDR          DDRA_DDRA2
#define Nokia5110_DC              GPIOE_PDOR //4  1д���ݣ�0дָ��
//#define Nokia5110_DN_DDR          DDRA_DDRA3
#define Nokia5110_DN              GPIOE_PDOR//2����
//#define Nokia5110_SCLK_DDR        DDRA_DDRA4
#define Nokia5110_SCLK            GPIOE_PDOR//0ʱ��  




void delay_1us(void)                 //1us��ʱ
{
	unsigned int i;
	for(i=0;i<1000;i++);
}


/*-----------------------------------------------------------------------
LCD_init      : 5110LCD��ʼ��
 
-----------------------------------------------------------------------*/
void LCD_init(void)
{
    SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;
    PORTE_PCR0=0;
    PORTE_PCR0|=PORT_PCR_MUX(1);
    PORTE_PCR1|=PORT_PCR_MUX(1);
    PORTE_PCR2|=PORT_PCR_MUX(1);
    PORTE_PCR4|=PORT_PCR_MUX(1);
    PORTE_PCR6|=PORT_PCR_MUX(1);
    GPIOE_PDDR|=(1<<0);
    GPIOE_PDDR|=(1<<8);
    GPIOE_PDDR|=(1<<2);
    GPIOE_PDDR|=(1<<4);
    GPIOE_PDDR|=(1<<6);
	    
	Nokia5110_RST &= ~(1<<8);  // ������λ������
	delay_1us();
	Nokia5110_RST|= (1<<8);
	
	
	Nokia5110_SCE&=~(1<<6);	// �ر�LCD
	delay_1us();
	
	Nokia5110_SCE|= (1<<6);	 // ʹ��LCD
	delay_1us();
	                             
	LCD_write_byte(0x21, 0);	// ʹ����չ��������LCDģʽ
	LCD_write_byte(0xc8, 0);	// ����ƫ�õ�ѹ
	LCD_write_byte(0x06, 0);	// �¶�У��
	LCD_write_byte(0x13, 0);	// 1:48
	LCD_write_byte(0x20, 0);	// ʹ�û�������
	LCD_clear();	                // ����
	LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
	       
	Nokia5110_SCE &=~(1<<6);	// �ر�LCD
}

/*-----------------------------------------------------------------------
LCD_clear         : LCD����
 
-----------------------------------------------------------------------*/
void LCD_clear(void)
{
	unsigned int i;
	
	LCD_write_byte(0x0c, 0);			
	LCD_write_byte(0x80, 0);			
	
	for (i=0; i<504; i++)
		LCD_write_byte(0, 1);			
}



void LCD_write_byte(unsigned char dat, unsigned char command)
{
	unsigned char i;
	//PORTB &= ~Nokia5110_SCE ;		        // ʹ��LCD
	Nokia5110_SCE &= ~(1<<6);
	
	if (command == 0)
	// PORTB &= ~Nokia5110_DC ;	        // ��������
	Nokia5110_DC &=~(1<<4);
	else
	// PORTB |= Nokia5110_DC ;		        // ��������
	Nokia5110_DC |=(1<<4);
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		Nokia5110_DN |= (1<<2);
		else
		Nokia5110_DN &=~(1<<2);
		Nokia5110_SCLK &= ~(1<<0);
		dat = dat << 1;
		Nokia5110_SCLK |=(1<<0);
	}
	Nokia5110_SCE|= (1<<6);
}



/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD����

�������:X       :0-83
          Y       :0-5
 
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char x, unsigned char y)
{
	LCD_write_byte(0x40 | y, 0);		// row
	LCD_write_byte(0x80 | x, 0);          	// column
}

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

�������:c       :��ʾ���ַ�;

-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)//��ֽ����
{
	unsigned char line;
	
	c -= 32;
	
	for (line=0; line<6; line++)
		LCD_write_byte(font6x8[c][line], 1);
}
void LCD_write_char_I(unsigned char c)//��ֽ����
{
	unsigned char line;
	
	c -= 32;
	
	for (line=0; line<6; line++)
		LCD_write_byte(~font6x8[c][line], 1);
}
void LCD_write_ASCII_char(unsigned char Y,unsigned char X,unsigned char c)
{
	unsigned char line;
	LCD_set_XY(Y,X*6);
	c -= 32;
	
	for (line=0; line<6; line++)
		LCD_write_byte(font6x8[c][line], 1);
}
void LCD_write_ASCII_char_I(unsigned char Y,unsigned char X,unsigned char c)
{
	unsigned char line;
	LCD_set_XY(Y,X*6);
	c -= 32;
	
	for (line=0; line<6; line++)
		LCD_write_byte(~font6x8[c][line], 1);
}

/*-----------------------------------------------------------------------
LCD_write_ASCII_string  : Ӣ���ַ�����ʾ

�������:*s      :�ַ���ָ��;
          X?Y    : ����  ��Y:0-5 ��X:0-13
		
-----------------------------------------------------------------------*/
void LCD_write_ASCII_string(unsigned char Y,unsigned char X,char *s)
{
	LCD_set_XY(Y,X);
	while (*s) 
	{
		LCD_write_char(*s);
		s++;
	}
}
void LCD_write_ASCII_string_I(unsigned char Y,unsigned char X,char *s)
{
	LCD_set_XY(Y,X*6);
	while (*s) 
	{
		LCD_write_char_I(*s);
		s++;
	}
}

//row:�� page:�� num:�ֵĸ��� hanzi:������������(�����ֽڵ���)
void LCD_write_hanzi(unsigned int row, unsigned int page,unsigned int num,u8 *hanzi) 
{
	unsigned char i,j,temp;  	
	
	temp=row;
        for(j=0;j<num;j++)
        {   if(j==5){page +=2;row = temp;}
            if(j==10){page +=2;row = temp;}
            LCD_set_XY(row*8, page);// ��ʾ�����ϰ벿�֣�
            for(i=0; i<16;i++) 
            {
                    LCD_write_byte(hanzi[j*32+i],1); 
            }
            
            LCD_set_XY(row*8, page+1);// ��ʾ�����°벿�� 
            for(i=16; i<32;i++) 
            {
                    LCD_write_byte(hanzi[j*32+i],1);
            }
            row +=2;
        }	
}

/*-----------------------------------------------------------------------
LCD_draw_map      : λͼ����

�������:X Y    :λͼ���Ƶ���ʼ����;
          *map    :λͼ����λ��;
          Pix_x   :λͼ����(��)
          Pix_y   :λͼ����(��)

-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(u8 X,u8 Y,u8 *map,u8 Pix_x,u8 Pix_y)
{
	unsigned int i,n;
	unsigned char row;
	
	if (Pix_y%8==0) row=Pix_y/8;      //����λͼ��ռ����
	else
	row=Pix_y/8+1;
	
	for (n=0;n<row;n++)
	{
		LCD_set_XY(X,Y);
		for(i=0; i<Pix_x; i++)
		{
			LCD_write_byte(map[i+n*Pix_x], 1);
		}
		Y++;                         //����
	}      
}


//============Number 2 String==============
void NumToStr( float number,char *p)
{
	
	if(number < 0) 
          {
            p[0] = '-'; 
            number = number*(-1);
          } 
	else 
          {
            p[0] = '+';
          }
        p[1] = (u16)(number)/1000+48;   //234.56   //2
        p[2] = (u16)(number)/100%10+48;   //234.56   //2
        p[3] = (u16)(number)/10%10+48;              // 3
        p[4] = (u16)(number)%10+48;                  //4
        p[5] = '.';                               // .
        p[6] = (u16)(number*10)%10+48;               //5
        p[7] = (u32)(number*100)%10+48;              //6
        p[8] = '\0';
        
        
} 


