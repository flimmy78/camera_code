/******************** (C) COPYRIGHT 2012-2013 岱默科技 DEMOK*********
 * 文件名         ： main.c
 * 描述           ： 主函数通用模板
 *
 * 实验平台       ： 岱默科技DEMOK Kinetis开发板
 * 作者           ： 岱默科技DEMOK Kinetis开发小组

 * 淘宝店铺       ： http://shop60443799.taobao.com/
 * 技术交流邮箱   ： 1030923155@qq.com 
 * 技术交流QQ群   ： 103360642

* 最后修订时间    ：2012-11-7
* 修订内容        ：无
**********************************************************************************/

#include "common.h"
#include "include.h"

/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*                                  OV7620图像采集实验
*
*  实验说明：通过Kinetis的DMA和EXTI驱动OV7640摄像头
*
*  实验操作：Kinetis与OV7620接线方式：
*                       Kinetis            OV7640
*                       PTD[0:7]           Y[0:7]
*                        PTE7               PCLK
*                        PTD10              HREF
*                        PTE0               VSYN
*                        PTC7               SCL
*                        PTC8               SDA
*                        GND                GND
*             Kinetis与串口模块接线方式：
*                   PTC3接RX    PTC4接TX   默认串口1 波特率115200
*            OV7640分辨率输出320*240
*
*
*  实验效果：在DEMOK提供的DemokTool上位机中可以看到采集到的图像
*
*  实验目的：学习DMA OV7640时序
*
*  修改时间：2012-11-14
*
*  备    注：Kinetis是3.3V供电，OV7640是3.3V供电，IO兼容
*
*  说    明：DEMOK原创 仅供参考 请勿拷贝
*************************************************************************/

u8 ImageBuf[ROW][COL];
u8 lcdis[48][84];


volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;

void main()
{
    u16  i;
    u16  j;

    
    
    //----初始化图像数组----//
    for(i=0; i<ROW; i++)
    {
      for(j=0;j<COL ;j++)
      {
        ImageBuf[i][j] = 0;
      }
    }
    
    
    //----初始化LCD-------//   
    LCD_init();     
	LCD_clear();
    
    
    DisableInterrupts;  
    
    //----初始化外部中断---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTC16 端口外部中断初始化 ，上升沿触发中断，内部下拉
    exti_init(PORTE,  0, rising_down);      //VSYN----PORTC18 端口外部中断初始化 ，上升沿触发中断，内部下拉

    EnableInterrupts;
    
    
    while(SampleFlag == 2)
    {
      for(i=0;(i>=20)&&((i-20)%6==0)&&(i<=302);i++)
      {
           for(j=0;(j>=20)&&((j-21)%2==0)&&(j<=186);j++)
           {
               lcdis[(i-20)/6][(j-20)/2] = ImageBuf[i][j];
           }
      }
      
      LCD_draw_pixel(lcdis,48,84,100,0,0);
      delay();
      
      SampleFlag = 0;
      
      EnableInterrupts;
    }
      
}