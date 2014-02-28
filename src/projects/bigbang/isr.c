/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	



#include "common.h"
#include "include.h"/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	



#include "common.h"
#include "include.h"

/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：PORTC_IRQHandler
*  功能说明：PORTC端口中断服务函数
*  参数说明：无
*  函数返回：无
*  备    注：引脚号需要自己初始化来清除
*
*************************************************************************/
extern u32 rowCnt ;   //行计数
extern u8 ImageBuf[ROW][COL];
extern u8 SampleFlag;



/*----------------使用串口猎人接受一幅图像数据的中断服务程序--------------------*/

void PORTD_IRQHandler()
{
   //---HREF行中断处理
    if( PORTD_ISFR & (1 << 10))         //PTD10触发中断  
    {
        PORTD_ISFR  |= (1 << 10);        //写1清中断标志位
        
         if ( SampleFlag == 0 )         //不足一场时返回
        { 
          return;
        }
        
        delayms(2);                     //延时1.6ms，跳过消隐区
        
        
        if(rowCnt < ROW)
        {
            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, ImageBuf[rowCnt-99], PTE7, DMA_BYTE1, COL, DMA_rising);
            //----使能DMA，初始化的时候禁止DMA
            DMA_EN(DMA_CH4); 
        //-------------DAM初始化通道4，数据源为PTD，每次存在数组ImageBuf[]指针中，PCLK接PTE7触发，每次传输1个字节，每次触发传输320次，上升沿触发
            rowCnt++;
        }
        else
        {
          rowCnt = 0;
          DMA_DIS(DMA_CH4);
          
          //---------若为动态显示，删除此行------------//
          DisableInterrupts;
          
          SampleFlag = 0;
        }
    }

}

void PORTE_IRQHandler()
{
    //---VSYN场中断处理
    if(PORTE_ISFR & (1 << 0))
    {
        PORTE_ISFR |= (1 << 0);
        rowCnt = 0;
        SampleFlag = 1;
    }
}

/*************************************************************************
*                            岱默科技DEMOK Kinetis开发小组
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void DMA_CH4_Handler(void)
{
    //DMA通道4
    DMA_IRQ_CLEAN(DMA_CH4);                             //清除通道传输中断标志位    (这样才能再次进入中断)
    DMA_IRQ_DIS(DMA_CH4);                               //禁止DMA   等待下一次行中断来临开启DMA
}



/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：PORTC_IRQHandler
*  功能说明：PORTC端口中断服务函数
*  参数说明：无
*  函数返回：无
*  备    注：引脚号需要自己初始化来清除
*
*************************************************************************/
extern u32 rowCnt ;   //行计数
extern u8 ImageBuf[ROW][COL];
extern u8 SampleFlag;



/*----------------使用串口猎人接受一幅图像数据的中断服务程序--------------------*/

//void PORTD_IRQHandler()
//{
//   //---HREF行中断处理
//    if( PORTD_ISFR & (1 << 9))         //PTD10触发中断  
//    {
//        PORTD_ISFR  |= (1 << 9);        //写1清中断标志位
//        
//         if ( SampleFlag == 0 )         //不足一场时返回
//        { 
//          return;
//        }
//        
//        delayms(2);                     //延时1.6ms，跳过消隐区
        
        
//        if(rowCnt < ROW)
//        {
//			delayus(2);
			
//            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, ImageBuf[rowCnt], PTE7, DMA_BYTE1, COL, DMA_rising);
            //----使能DMA，初始化的时候禁止DMA
//            DMA_EN(DMA_CH4); 
        //-------------DAM初始化通道4，数据源为PTD，每次存在数组ImageBuf[]指针中，PCLK接PTE7触发，每次传输1个字节，每次触发传输320次，上升沿触发
//            rowCnt++;
			
//			printf("%d ",rowCnt);
//        }
//        else
//        {
//          rowCnt = 0;
//          DMA_DIS(DMA_CH4);
//          
//          //---------若为动态显示，删除此行------------//
//          DisableInterrupts;
//          
//          SampleFlag = 2;
//		  
////		  printf("%d ",SampleFlag);
//        }
//    }
//
//}

void PORTC_IRQHandler()
{
    //---VSYN场中断处理
    if(PORTC_ISFR & (1 << 16))
    {
        PORTC_ISFR |= (1 << 16);
		
		delayms(10);
		
        rowCnt = 0;
//        SampleFlag = 1;
		SampleFlag++;
		
//		printf("%d\n",SampleFlag);
//		printf("\nSampleFlag = %d\n",SampleFlag);
    }
}

//u8 VSYN_Flag = 0;
//void PORTC_IRQHandler()
//{
//    //---VSYN场中断处理
//    if(PORTC_ISFR & (1 << 16))
//    {
//        PORTC_ISFR |= (1 << 16);
//        
//		if(VSYN_Flag == 0)
//		{
//			/* 开启时钟*/
//			SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //使能PIT时钟
//			/* PIT模块控制 PIT Module Control Register (PIT_MCR) */
//			PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //使能PIT定时器时钟 ，调试模式下继续运行
//			
//			PIT_LDVAL0 = 0xFFFFFFFF;
//			PIT_CVAL0  = 0x00000000;
//			PIT_TFLG0  = 0x00000001;
//			PIT_TCTRL0 = 0x00000001;
//			
//			VSYN_Flag = 1;
//		}
//		else
//		{
//			PIT_TCTRL0 = 0x00000000;
//			
//			VSYN_Flag = 0;
//			
//			printf("PIT_LDVAL0 = %u\n",PIT_LDVAL0);
//			printf("PIT_CVAL0  = %u\n",PIT_CVAL0);
//			
//			//PIT_TCTRL0 = 0x00000000;
//		}
//
//    }
//}

/*************************************************************************
*                            岱默科技DEMOK Kinetis开发小组
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
//void DMA_CH4_Handler(void)
//{
//    //DMA通道4
//    DMA_IRQ_CLEAN(DMA_CH4);                             //清除通道传输中断标志位    (这样才能再次进入中断)
//    DMA_IRQ_DIS(DMA_CH4);                               //禁止DMA   等待下一次行中断来临开启DMA
//}

