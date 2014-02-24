

#include "init.h"

/**********************
*使用到的引脚初始化
*
**********************/
void used_pin_config()
{
  asm ("nop");
}





/******************
* 板级初始化
*/
void board_init()
{
  Light_init;
  LCD_init();
  used_pin_config();
}