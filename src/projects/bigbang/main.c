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

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;


/*-----------------使用串口猎人接受一幅图像数据的主程序-------------*/

void main()
{
    u16  i;
    u16  j;
//
//    //----初始化图像数组----//
//    for(i=0; i<ROW; i++)
//    {
//      	for(j=0; j<COL; j++)
//      	{
//       	 	ImageBuf[i][j] = 0;
//     	}
//    }
    
    uart_init(UART0,115200);
    
//    FTM_PWM_init(FTM0,CH0,5000000,80);       //利用PWM波触发DMA，触发频率5MHz
    
    DisableInterrupts;  
    
    //----初始化外部中断---//
    exti_init(PORTD,  9, rising_down);       //HREF----PORTD10 端口外部中断初始化 ，上升沿触发中断，内部下拉
    exti_init(PORTC, 16, rising_down);       //VSYN----PORTE0 端口外部中断初始化 ，上升沿触发中断，内部下拉

    EnableInterrupts;

//    for(;;)
//    {
//      while(SampleFlag != 2);				//SampleFlag==1,单片机正处于采集图像时间段
//
//      for(i=0;i<ROW;i++)
//      {
//          for(j=0;j<COL;j++)
//          {
//              printf("%-4d",ImageBuf[i][j]);
//          }
//          printf("\n");
//      }
//	  
//	  SampleFlag = 0;
      
//      EnableInterrupts;
	while(1)
	{
		if(i!=rowCnt)
		{
			printf("%d ",rowCnt);
			i=rowCnt;
		}
		if(j!=SampleFlag)
			{
			printf("\n%d\n ",SampleFlag);
			j=SampleFlag;
		}
			
	}
//    }
    
}

/**************测试行中断时间***************/
//extern u8 VSYN_Flag;
//void main()
//{
////	u32 a,b;
//	
//	DisableInterrupts;
//	
//	uart_init(UART0,115200);
//	exti_init(PORTC, 16, falling_up);
//	
//	EnableInterrupts;
//	
//	for(;;)
//	{
////		while(VSYN_Flag != 2);
////		
////		a = PIT_LDVAL0;
////		b = PIT_CVAL0;
////		printf("PIT_LDVAL0 = %d\n",a);
////		printf("PIT_CVAL0  = %d\n",b);
////		
////		VSYN_Flag = 0;
//	}
//
//}