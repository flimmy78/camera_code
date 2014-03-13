/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*                                  SonyCCD图像采集实验
*
*  实验说明：通过Kinetis的DMA和EXTI驱动SonyCCD摄像头
*
*  实验操作：Kinetis与SonyCCD接线方式：
*                       Kinetis            SonyCCD
*                        PTE7               PCLK
*                        PTD10              HREF
*                        PTE0               VSYN
*                        GND                GND
*             Kinetis与串口模块接线方式：
*                   PTC3接RX    PTC4接TX   默认串口1 波特率115200
*             分辨率输出320*240
*
*  说    明：数据的发送和采集并不同步进行，采集完一帧图像停止采集，将其发
*            送，发送完毕后才继续采集
*
*  修    改：聂晓艺
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
//  
//  DMA_count_Init(DMA_CH4, PTC0, 10000, DMA_rising_down);
//  pit_init_ms(PIT0,200);
//    
//  car->angle_p   = 125.5;
//  car->gyro_d    = 5;
//  car->angle_set =7.5;
//  car->gyro_set  =6.1;
//  uart_init(UART0,115200);
 while(1)
 { 
  angle_m = acc_data_get();
  gyro_m  = gyro_data_get();
  comp_filter(angle_m,gyro_m,3.5, 0.005,car);
  (car->left_duty)=(car->right_duty) = (car->angle - car->angle_set)*car->angle_p + (gyro_m - car->gyro_set)*car->gyro_d;  
   right_run_s((int32_t)car->right_duty);
   left_run_s((int32_t)car->left_duty);
   delayms(5);
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
