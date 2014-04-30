

#include "init.h"
#include "board.h"
#include "FTM.h"

/**********************
*ʹ�õ������ų�ʼ��
*
**********************/
static void used_pin_config()
{
  //��������
  gpio_init(KEY1_PORT,KEY1_PIN,GPI,1);
  gpio_init(KEY2_PORT,KEY2_PIN,GPI,1);
  gpio_init(KEY3_PORT,KEY3_PIN,GPI,1);
  
  //���뿪������
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  gpio_init(SW8_PORT,SW8_PIN1,GPI,1);
  
  //AD������������
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
* �弶��ʼ��
*/
void board_init()
{
  //���ڳ�ʼ��
  uart_init(UART4,115200); 
  
  //���İ��ϵ�LED�Ƴ�ʼ��
  Light_init;
 
  
  //�����Ǻͼ��ٶȼƳ�ʼ��
  angle_get_init();

  //���������ʼ��
   motor_init();
  
  //�������ó�ʼ��
  used_pin_config();      
  
  //�ٶȲ�����ʼ��
  speed_init();
  
    pit_init_ms(PIT1,1);
  
//    exti_init(PORTE,11,rising_down);     //���ж�
//    exti_init(PORTD,14,rising_down);    //���ж�
//    DMA_count_Init(DMA_CH0, PTA24, 10000, DMA_rising_down);
  
}