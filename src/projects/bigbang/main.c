/*************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*                                  SonyCCDͼ��ɼ�ʵ��
*
*  ʵ��˵����ͨ��Kinetis��DMA��EXTI����SonyCCD����ͷ
*
*  ʵ�������Kinetis��SonyCCD���߷�ʽ��
*                       Kinetis            SonyCCD
*                        PTE7               PCLK
*                        PTD10              HREF
*                        PTE0               VSYN
*                        GND                GND
*             Kinetis�봮��ģ����߷�ʽ��
*                   PTC3��RX    PTC4��TX   Ĭ�ϴ���1 ������115200
*             �ֱ������320*240
*
*  ˵    �������ݵķ��ͺͲɼ�����ͬ�����У��ɼ���һ֡ͼ��ֹͣ�ɼ������䷢
*            �ͣ�������Ϻ�ż����ɼ�
*
*  ��    �ģ�������
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

extern volatile u8 vref_flag;       //���ж��б��־
extern volatile u16  row_count;     //�м���
extern u8   image[ROW][COL];   //ͼ������

int16_t edge_l[ROW];                //��ȡͼ����ƫ�
int16_t edge_r[ROW];               //��ȡͼ����ƫ�
u8 image_flag = ROW_START;                //��ȡͼ�����־��
u8  threshold = 150;             //�ڰ���ֵ��
extern volatile u16  row_count;

void main()
{ 
    //����ϵͳ����
    DisableInterrupts;
    board_init();
    uart_init(UART4,115200); 
    //����������á�
    car->angle_p   = 155;//155.5;
    car->gyro_d    = 0.80;//1.50;
    car->angle_set = 30.7;
    car->gyro_set  =  0;


    car->speed_set = 0.0;         
    car->speed_p   = 0.0;        
    car->speed_i   = 0.1;           
     
    car->direction_p  = 0.0;             //�������p������
    car->direction_d  = 0.0;             //�������d������
    car->direction_left_duty  = 0;
    car->direction_right_duty = 0;
//    EnableInterrupts;
 
#if 1
     char str; 
     while(1)
    {    
        str = uart_getchar(UART4);
        switch(str)
        {
            case 's': DisableInterrupts;
                       left_run_s(0);
                       right_run_s(0);  break;
                       
            case 'r':EnableInterrupts;  break;
            
        }
        
        if(str == 'p')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->angle_p += 1;printf("p=%f\n",car->angle_p);break;}
                if(uart_getchar(UART4)=='-')
                    {car->angle_p -= 1;printf("p=%f\n",car->angle_p);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'd')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->gyro_d += 0.01;printf("d=%f\n",car->gyro_d);break;}
                if(uart_getchar(UART4)=='-')
                {car->gyro_d -= 0.01;printf("d=%f\n",car->gyro_d);break;}
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
                {car->speed_i += 0.01;printf("i=%f\n",car->speed_i);break;}
                if(uart_getchar(UART4)=='-')
                {car->speed_i -= 0.01;printf("i=%f\n",car->speed_i);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        
        }
        
        if(str == 'P')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->speed_p += 0.01;printf("P=%f\n",car->speed_p);break;}
                if(uart_getchar(UART4)=='-')
                    {car->speed_p -= 0.01;printf("P=%f\n",car->speed_p);break;}
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
//���͵������ݣ�����Ϊ5Ϊ����12345����Ӧ123.45.
//    printf("\n\n");
//    printf("��c��������ģʽ\n");
    
// while(1)
// {    
//    str = uart_getchar(UART0);
//    if(str == 'c')
//      {
//        while(1)
//        {
//          printf("����ģʽ\n");
//          printf("��s���ֹͣ����\n");
//          printf("��r�����ʼ����\n");
//          printf("��p��ӡ����\n");
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
//          default : printf("�������������������\n");break;
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

