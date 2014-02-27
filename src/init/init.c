

#include "init.h"

/**********************
*使用到的引脚初始化
*
**********************/
static void used_pin_config()
{
  asm ("nop");
}





/******************
* 板级初始化
*/
void board_init()
{
  //核心板上的LED灯初始化
  Light_init; 
  
  //5110LCD初始化
  LCD_init();      
  
  //加速度计的AD通道初始化
  adc_init(ADC0,AD8); 
  
  //陀螺仪AD通道初始化
  adc_init(ADC0,AD9);
  
  //串口初始化
  uart_init(UART0,115200); 
  
  //引脚配置初始化
  used_pin_config();                    
}