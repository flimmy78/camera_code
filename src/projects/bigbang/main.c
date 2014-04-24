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
#include "image.h"


struct cars_status car_s;
cars_status car= &car_s;   //

extern volatile u8 vref_flag;       //场中断判别标志
extern volatile u16  row_count;     //行计数
extern u8   image[ROW][COL];   //图像存放区

int16_t edge_l[ROW];                //存取图像左偏差。
int16_t edge_r[ROW];               //存取图像右偏差。
u8 image_flag = ROW_START;                //存取图像处理标志。
u8  threshold = 150;             //黑白阈值。
extern volatile u16  row_count;

#if 1
void main()
{ 
  
  //车体系统设置
  DisableInterrupts;
  board_init();
  uart_init(UART0,115200);
  exti_init(PORTA,17,rising_down);
  exti_init(PORTA,26,rising_down);
  DMA_count_Init(DMA_CH0, PTA24, 10000, DMA_rising_down);
  
  //车体参数设置。
 
  car->angle_p   = 125.5;
  car->gyro_d    = 1.5; //4.5
  car->angle_set = -35.6;//-35.4;//-48.5;//-48.00;//-47.84;//-45.94;//-45.54;//-45.84;//-46.14;//-31.14;//-42.5
  car->gyro_set  =  0;
  
  
  car->speed_set = -200.0;         
  car->speed_p   = 0.8; //0.8       
  car->speed_i   = 1.2;//8.5       
  car->speed_d   = 0.0;        
     
  car->direction_p  = 0;             //方向控制p参数。
  car->direction_d  = 0;             //方向控制d参数。
  car->direction_left_duty  = 0;
  car->direction_right_duty = 0;
  EnableInterrupts;
 
//  int num[5];
//  char str;
//  int i;
//  float data;
// //EnableInterrupts;
// //发送调试数据，数据为5为，‘12345’对应123.45.
//  printf("\n\n");
//  printf("按c进入命令模式\n");
//  
// while(1)
// {    
//    str = uart_getchar(UART0);
//    if(str == 'c')
//      {
//        while(1)
//        {
//          printf("命令模式\n");
//          printf("按s电机停止工作\n");
//          printf("按r电机开始工作\n");
//          printf("按p打印参数\n");
//          str = uart_getchar(UART0);
//          if(str == 'r')
//            {
//              EnableInterrupts;  break;
//            }
//          switch(str)
//          {
//          case 's': DisableInterrupts;
//                    left_run_s(0);
//                    right_run_s(0);  break;
//          case 'p': print(car);break;
//          default : printf("输入命令错请重新输入\n");break;
//          }
//          
//        }
//      }
//    else if(str == 's')
//    {  
//      EnableInterrupts;
//      while(1)
//      {
//        if(uart_getchar(UART0) == '+')
//             car->angle_set +=0.1;
//        if(uart_getchar(UART0) == '-')
//             car->angle_set -=0.1;
//        printf("%f\n", car->angle_set);
//        if(uart_getchar(UART0) == 'b')
//          break;
//      }
//    }
//        
//    else
//      {
//        for(i=0;i<5;i++)
//        {
//          num[i] = uart_getchar(UART0) - '0';
//        }
//        data = num[0]*100 + num[1] * 10 + num[2]  + num[3] * 0.1 +num[4] *0.01;
//       switch(str)
//       {
//       case 'a': car->angle_set = -data ; printf("angle_set:%f\n",car->angle_set);break;
//       case 'p':  car->angle_p = data;printf("car->angle_p:%f\n",car->angle_p);break;
//       case 'i':
//       case 'I':  car->speed_i = data;printf("speed_i:%f\n",car->speed_i);break;
//       case 'D':
//       case 'd':  car->gyro_d = data;printf("car->gyro_d:%f\n",car->gyro_d);break;
//       default :break;
//      } 
//      }
//}
//
//u8 i = 0;       //图像行计数。
//
//  while(1)
//  {    
//    if(i == 40)
//       i = 0;
//    if(row_count > ROW_END )
//       image_flag = ROW_START;
//    if(row_count > ROW_START && row_count < ROW_END + 1)
//    {
//      if(row_count>image_flag)
//      {
//        edge_r[i] = image_right_offset(image ,image_flag );
//        edge_l[i] = image_left_offset (image ,image_flag );
//        
//        i++;
//        image_flag++;
//        
//        printf("%d\t%d\t%d\n",image_flag,edge_r[i],edge_l[i]);
//      }
//    }
//  }

}


#else

extern volatile u8 vref_flag;       //场中断判别标志
extern volatile u16  row_count;     //行计数
extern u8   image[ROW][COL];   //图像存放区

void main()
{
//    u8 i,j;
    
    uart_init(UART0,115200);
//    exti_init(PORTA,17,falling_up);
    exti_init(PORTA,17,rising_down);
    exti_init(PORTA,26,rising_down);
    DMA_count_Init(DMA_CH0, PTA24, 10000, DMA_rising_down);
    
    for(;;)
    {
//        if(vref_flag == 2)
//        {
//            uart_putchar (UART0, 0x00);
//            uart_putchar (UART0, 0xFF);
//            uart_putchar (UART0, 0x01);
//            uart_putchar (UART0, 0x00);
//
//            for(i=0;i<ROW;i++)
//            {
//                for(j=0;j<COL;j++)
//                    uart_putchar(UART0,image[i][j]);
//            }
//            
//            vref_flag = 0;
////            DisableInterrupts;
//        }
    }
}

#endif