

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
  uart_init(UART0,115200); 
  
  //���İ��ϵ�LED�Ƴ�ʼ��
  Light_init;
  printf("light is inited\n");
  
  //5110LCD��ʼ��
  LCD_init();
  printf("LCD is inited\n");
  
  //�����Ǻͼ��ٶȼƳ�ʼ��
  angle_get_init();
  printf("���ٶȼ� is inited\n");
  printf("������ is inited");
  
//  board_pit_init();
  
  //���������ʼ��
  motor_init();
  
  //�������ó�ʼ��
  used_pin_config();                    
}