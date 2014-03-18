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
 
  //车体系统设置
  DisableInterrupts;
  board_init();
  uart_init(UART0,115200);
   
  //车体参数设置。
   
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
