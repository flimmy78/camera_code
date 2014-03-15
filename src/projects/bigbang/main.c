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


//const s32 g=15587;
//const s32 zero=26420;
//
u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;

extern cars_status car;
cars_status car;   //


void main()
{ 
  
  board_init(); 
  DisableInterrupts;
  
  DMA_count_Init(DMA_CH4, PTC0, 10000, DMA_rising_down);
  pit_init_ms(PIT0,200);
  pit_init_ms(PIT1,5);
  car->angle_p   = 35.5;
  car->gyro_d    = 0.5;
  car->angle_set =7.5;
  car->gyro_set  =2;
  uart_init(UART0,115200);
  EnableInterrupts;
 while(1)
 {
   
 }
   
}

//void main()
//{
//    FTM_PWM_init(FTM0,CH0,10000,50);
//    uart_init(UART0,115200);
//    DMA_count_Init(DMA_CH4, PTC0, 10000, DMA_rising_down);
//    pit_init_ms(PIT0,200);
//    
//    for(;;);
//}
