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

float angle_m,gyro_m;
void main()
{ 
  DisableInterrupts;
  board_init();
  car->angle_p   = 85.5;
  car->gyro_d    = 5;
  car->angle_set = 2.0;
  car->gyro_set  = 1;
  car->speed_d   = 1;
  car->speed_set = 10;
  car->speed_p   = 1;
  car->left_duty = 10;
  car->right_duty =10;
  uart_init(UART0,115200);
  EnableInterrupts;
//  uart_getchar(UART0);
// car->left_duty   = -100;
// car->right_duty  = -100;
// right_run_s((int32_t)car->right_duty);
// left_run_s((int32_t)car->left_duty);
 while(1)
 {
    

 } 
}
