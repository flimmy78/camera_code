/*************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*                                  SonyCCDͼ��ɼ�ʵ��
*
*  ʵ��˵����ͨ��Kinetis��DMA��EXTI����SonyCCD����ͷ
*
*  ʵ�������Kinetis��SonyCCD���߷�ʽ��
*                       Kinetis            SonyCCD
*                        PTE7               PCLK
*                        PTD10              HREF
*                        PTE0               VSYN
*                        GND                GND
*             Kinetis�봮��ģ����߷�ʽ��
*                   PTC3��RX    PTC4��TX   Ĭ�ϴ���1 ������115200
*             �ֱ������320*240
*
*  ˵    �������ݵķ��ͺͲɼ�����ͬ�����У��ɼ���һ֡ͼ��ֹͣ�ɼ������䷢
*            �ͣ�������Ϻ�ż����ɼ�
*
*  ��    �ģ�������
*************************************************************************/
#include <math.h>
#include "common.h"
#include "include.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "init.h"
#include "board.h"
#include "Kalman.h"


const s32 g=15587;
const s32 zero=26420;

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;

//extern cars_status car;

struct cars_status car_s;
cars_status car= &car_s;   //

void main()
{ 
 
  //����ϵͳ����
  DisableInterrupts;
  board_init();
  uart_init(UART0,115200);
   
  //����������á�
   
  car->left_duty = 0;
  car->right_duty =0;
  
  car->angle_p   = 85.5;
  car->gyro_d    = 5;
  car->angle_set = 5.0;
  car->gyro_set  = 1;
  
  car->speed_set = 0.0;
  car->speed_d   = 75.5;
  car->speed_p   = 4.1;
  car->speed_duty =0;
  car->direction_left_duty  = 0;
  car->direction_right_duty =0;
  
  EnableInterrupts;
 
 while(1)
 {
   
    motor_set(car);
    
 } 

  
}
