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
extern unsigned int count2_temp;
u8 D = 10;
void main()
{ 
    u8 i;
    u32 gyro_intr;
    //车体系统设置
    DisableInterrupts;
    board_init();
    uart_init(UART4,115200); 
    //车体参数设置。
    car->angle_p   = 201.5;//125.5;//125.5;
    car->gyro_d    = 1.59;//0.8;//0.8;//1.50;
    car->angle_set = 30.8;//37.0;//31.5;
    car->gyro_set  =  0;

    car->speed_set = 0.0;         
    car->speed_p   = 0.0;//10.0;        
    car->speed_i   = 0.0;//4.1;//0.92//0.36;//0.0;           
     
    car->direction_p  = 0.0;             //方向控制p参数。
    car->direction_d  = 0;             //方向控制d参数。
    car->direction_left_duty  = 0;
    car->direction_right_duty = 0;
    
    delayms(2000);
    printf("发送任意字符计算零偏值\n");
    uart_getchar(UART4);
    for(i=0;i<100;i++)
    {
        gyro_intr += ad_ave(ADC1,SE10,ADC_16bit,20);
        delayms(5);
    }
    
    GYRO_ZERO = gyro_intr/i;
 printf("OK 初始化完毕\n");
#if 1
     char str;
     float time = 1.0;
     while(1)
    {    
        str = uart_getchar(UART4);
        printf("row_count:%u\t count2_temp:%u\n;",row_count,count2_temp);
        switch(str)
        {
            case 's': DisableInterrupts;
                       left_run_s(0);
                       right_run_s(0);  break;
                       
            case 'r':EnableInterrupts;  break;
            
        }
        
        if(str == 't')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='*')
                {time *= 10;printf("t=%f\n",time);break;}
                if(uart_getchar(UART4)=='/')
                    {time /= 10;printf("t=%f\n",time);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        if(str == 'p')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->angle_p += 1*time;printf("p=%f\n",car->angle_p);break;}
                if(uart_getchar(UART4)=='-')
                    {car->angle_p -= 1*time;printf("p=%f\n",car->angle_p);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'd')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->gyro_d += 0.01*time;printf("d=%f\n",car->gyro_d);break;}
                if(uart_getchar(UART4)=='-')
                {car->gyro_d -= 0.01*time;printf("d=%f\n",car->gyro_d);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'a')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->angle_set += 0.1;printf("a=%f\n",car->angle_set);break;}
                if(uart_getchar(UART4)=='-')
                {car->angle_set -= 0.1;printf("a=%f\n",car->angle_set);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'i')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->speed_i += 0.01*time;printf("i=%f\n",car->speed_i);break;}
                if(uart_getchar(UART4)=='-')
                {car->speed_i -= 0.01*time;printf("i=%f\n",car->speed_i);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        
        }
        
        if(str == 'P')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->speed_p += 0.01*time;printf("P=%f\n",car->speed_p);break;}
                if(uart_getchar(UART4)=='-')
                    {car->speed_p -= 0.01*time;printf("P=%f\n",car->speed_p);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'R')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {R += 0.5;printf("R=%f\n",R);break;}
                if(uart_getchar(UART4)=='-')
                    {R -= 0.5;printf("R=%f\n",R);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'D')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {D += 1;printf("dead=%d\n",D);right_dead=left_dead=D;break;}
                if(uart_getchar(UART4)=='-')
                    {D -= 1;printf("dead=%d\n",D);right_dead=left_dead=D;break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
  
    }

    
#endif  
    
    
    
//    int num[5];
//    char str;    
//    int i;
//    float data;
//    EnableInterrupts;
//发送调试数据，数据为5为，‘12345’对应123.45.
//    printf("\n\n");
//    printf("按c进入命令模式\n");
    
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
// }
 
}

