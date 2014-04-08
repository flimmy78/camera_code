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



#include  "common.h"
#include  "include.h"
#include  "LED_CORE.h"


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
            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTB_BYTE0_IN, ImageBuf[rowCnt-99], PTE7, DMA_BYTE1, COL, DMA_rising);
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
    DMA_IRQ_CLEAN(DMA_CH4); //清除通道传输中断标志位    (这样才能再次进入中断)
    
    DMA_EN(DMA_CH4);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
}

void DMA_CH1_Handler(void)
{
    //DMA通道4
    DMA_IRQ_CLEAN(DMA_CH1); //清除通道传输中断标志位    (这样才能再次进入中断)
    
    DMA_EN(DMA_CH1);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
}

extern cars_status car;
u32 a,b,c,d;

void PIT_CH0_Handler()
{
    PIT_Flag_Clear(PIT0);
    car->speed_left_m=pulse_cnt_left();
    car->speed_right_m= pulse_cnt_right();
    speed_control(car);
}

void PIT_CH1_Handler(void)
{
   
    PIT_Flag_Clear(PIT1); 
    static unsigned int count1,count2;
    switch(count1)
    {
    case 0:
              break;
    case 1:
              break;
    case 2:
              break;
    case 3:
              blance_comp_filter(3.5,0.005,car);
              break;
    case 4:
            count2++;
          if(count2==20)
          {
               car->speed_left_m  = 1000*left_speed();
               car->speed_right_m = 1000*right_speed();
               speed_control(car);
               count2=0;
               printf("%f\t%f\n",car->speed_duty,car->left_duty);
          }
          
         speed_control_output(car);
         car->left_duty     = car->blance_duty + car->speed_duty + car->direction_left_duty;
         car->right_duty    = car->blance_duty + car->speed_duty + car->direction_right_duty;
         motor_set(car);
         break;
         
    default:
              break;
    }
    
  count1++;
  
  if(count1 == 5)
      count1 = 0;
    
    
}

char str[10];
u8 len;
float num;
void UART0_IRQHandler(void)
{
    u8 i;
    DisableInterrupts;
    
    uart_pendstr(UART0,str);
    if(str[0] == '\0')
    {
        EnableInterrupts;
        return;
    }
    len = strlen(str);
    
    if((str[0] == 'p')||(str[0] == 'P'))
    {
        for(i=0; i<len-1;i++)
        {
            str[i] = str[i+1];
        }
        car->speed_p = str2ufloat(str,len-1);
        printf("you send p = %f\n",car->speed_p);
        
        /*****用户函数*********/
        
        
        
    }
    else if((str[0] == 'd')||(str[0] == 'D'))
    {
        for(i=0; i<len-1;i++)
        {
            str[i] = str[i+1];
        }
        car->speed_d = str2ufloat(str,len-1);
        printf("you send d = %f\n",car->speed_d );
        
        /*****用户函数*********/
        
        
        
    }

    EnableInterrupts;
}

