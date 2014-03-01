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

#include "common.h"
#include "include.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "board_config.h"
#include "init.h"
#include "board.h"

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;


/*-----------------使用串口猎人接受一幅图像数据的主程序-------------*/

//void main()
//{
// 
//  board_init();            //初始化
//
//  right_run(20,ahead);
//  left_run(20,ahead);
//  
//  while(1)
//  {
//    ;
//  }
//}


void main()
{
	adc_init(ADC0,AD8);		//加速度计数据获取初始化
	
	for(;;)
	{
		printf("%f\n",acc_angle_get());
		
		delayms(30);
	}
}