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


/*-----------------使用串口猎人接受一幅图像数据的主程序-------------*/

void main()
{ 
  s32 acc_32=1;
  u16 acc;
  float ans1;
  board_init();
  uart_init(UART0,115200);
  left_run(15,ahead);
  right_run(15,ahead);
  printf("getchar\n");
  uart_getchar(UART0);
  while(1)
  {
    left_run_s(acc_32);
    right_run_s(acc_32);
    printf("getchar\n");
    acc = uart_getchar(UART0);
    switch (acc)
    {
    case 'a':
      acc_32++;
      break;
    case 'b':
      acc_32--;
      break;
    case 'd':
      acc_32 = 0-acc_32;
      break;
    }
    printf("%d",acc_32);
    
//    if(acc > 'd')
//      acc_32++;
//    else
//      acc_32--;
//    printf("%d\n",acc_32);
//    
//    acc = acc_data_get();
//    acc_32 = acc;
//    ans1 = asin((acc_32-zero)/g);
//    printf("AD16:%u\tAD32:%d\tasin:%f\n",acc,acc_32,ans1);
  }
}
