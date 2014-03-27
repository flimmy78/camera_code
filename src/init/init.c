

#include "init.h"
#include "board.h"
#include "FTM.h"

/**********************
*使用到的引脚初始化
*
**********************/
static void used_pin_config()
{
  //按键配置
  gpio_init(KEY1_PORT,KEY1_PIN,GPI,1);
  gpio_init(KEY2_PORT,KEY2_PIN,GPI,1);
  gpio_init(KEY3_PORT,KEY3_PIN,GPI,1);
  
  //拨码开关配置
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  
  //AD输入引脚配置
  gpio_init(AD_PORT,AD_PIN1,GPI,1);
  gpio_init(AD_PORT,AD_PIN2,GPI,1);
  gpio_init(AD_PORT,AD_PIN3,GPI,1);
  gpio_init(AD_PORT,AD_PIN4,GPI,1);
  gpio_init(AD_PORT,AD_PIN5,GPI,1);
  gpio_init(AD_PORT,AD_PIN6,GPI,1);
  gpio_init(AD_PORT,AD_PIN7,GPI,1);
  gpio_init(AD_PORT,AD_PIN8,GPI,1);
  asm ("nop");
}


void board_pit_init(void)
{
  pit_init(PIT0,50000);
  pit_init(PIT1,25030);
}



/******************
* 板级初始化
*/
void board_init()
{
  //串口初始化
  uart_init(UART0,115200); 
  
  //核心板上的LED灯初始化
  Light_init;
  printf("light is inited\n");
  
  //5110LCD初始化
//  LCD_init();
//  printf("LCD is inited\n");
  
  //陀螺仪和加速度计初始化
  angle_get_init();
  printf("加速度计 is inited\n");
  printf("陀螺仪 is inited");
  
//  board_pit_init();
  
  //电机驱动初始化
   motor_init();
  
  //引脚配置初始化
  used_pin_config();      
  
  //速度采样初始化
  speed_init();
  
}