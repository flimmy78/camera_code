

#include "init.h"

/**********************
*ʹ�õ������ų�ʼ��
*
**********************/
void used_pin_config()
{
  asm ("nop");
}





/******************
* �弶��ʼ��
*/
void board_init()
{
  Light_init;
  LCD_init();
  used_pin_config();
}