

#include "init.h"
#include "board.h"
#include "FTM.h"

/**********************
*ʹ�õ������ų�ʼ��
*
**********************/
static void used_pin_config()
{
  asm ("nop");
}

/*************���������ʼ��*****************/

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
* �弶��ʼ��
*/
void board_init()
{
  //���ڳ�ʼ��
  uart_init(UART0,115200); 
  
  //���İ��ϵ�LED�Ƴ�ʼ��
  Light_init;
  printf("light is inited\n");
  
  //5110LCD��ʼ��
  LCD_init();
  printf("LCD is inited\n");
  
  //���ٶȼƵ�ADͨ����ʼ��
  adc_init(ADC0,AD8); 
  printf("���ٶȼ� is inited\n");
  
  //������ADͨ����ʼ��
  adc_init(ADC1,AD9);
  printf("������ is inited");
  
  
  board_pit_init(void);
  
  //���������ʼ��
  motor_init();
  
  //�������ó�ʼ��
  used_pin_config();                    
}