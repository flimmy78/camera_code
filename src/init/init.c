

#include "init.h"

/**********************
*ʹ�õ������ų�ʼ��
*
**********************/
static void used_pin_config()
{
  asm ("nop");
}





/******************
* �弶��ʼ��
*/
void board_init()
{
  //���İ��ϵ�LED�Ƴ�ʼ��
  Light_init; 
  
  //5110LCD��ʼ��
  LCD_init();      
  
  //���ٶȼƵ�ADͨ����ʼ��
  adc_init(ADC0,AD8); 
  
  //������ADͨ����ʼ��
  adc_init(ADC0,AD9);
  
  //���ڳ�ʼ��
  uart_init(UART0,115200); 
  
  //�������ó�ʼ��
  used_pin_config();                    
}