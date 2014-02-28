

#include "init.h"
#include "board.h"
#include "FTM.h"

/**********************
*使用到的引脚初始化
*
**********************/
static void used_pin_config()
{
  asm ("nop");
}

/*************电机驱动初始化*****************/

void motor_init(void)
{
  FTM_PWM_init(RIGHT_A_FTM,RIGHT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(RIGHT_B_FTM,RIGHT_B_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_A_FTM,LEFT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_B_FTM,LEFT_B_CH,MOTOR_FRE,INIT_DUTY);
}

void board_pit_init(void)
{
  ;
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
  LCD_init();
  printf("LCD is inited\n");
  
  //加速度计的AD通道初始化
  adc_init(ADC0,AD8); 
  printf("加速度计 is inited\n");
  
  //陀螺仪AD通道初始化
  adc_init(ADC1,AD9);
  printf("陀螺仪 is inited");
  
  
  board_pit_init(void);
  
  //电机驱动初始化
  motor_init();
  
  //引脚配置初始化
  used_pin_config();                    
}