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
  
  
  //����ϵͳ����
  DisableInterrupts;
  board_init();
  uart_init(UART0,115200);
   
  //����������á�
   
  car->left_duty = 10;
  car->right_duty =10;
  
  car->angle_p   =  85.5;
  car->gyro_d    =  5;
  car->angle_set =  -1.0;
  car->gyro_set  =  0.0;
  
  
  car->speed_set = 0.0;
  car->speed_p   = 0;        
  car->speed_i   = 0.0;         
  car->speed_d   = 0.0;        
  car->speed_set = 0;      
  car->speed_duty= 0;
  
  
  
  car->direction_left_duty  = 0;
  car->direction_right_duty = 0;
  int num[5];
  char str;
  int i;
  float data;
  //EnableInterrupts;
  //���͵������ݣ�����Ϊ5Ϊ����12345����Ӧ123.45.
  printf("\n\n");
  printf("��c��������ģʽ\n");
 while(1)
 {    
    str = uart_getchar(UART0);
    if(str == 'c')
      {
        while(1)
        {
          printf("����ģʽ\n");
          printf("��s���ֹͣ����\n");
          printf("��r�����ʼ����\n");
          printf("��p��ӡ����\n");
          str = uart_getchar(UART0);
          if(str == 'r')
            {
              EnableInterrupts;  break;
            }
          switch(str)
          {
          case 's': DisableInterrupts;
                    left_run_s(0);
                    right_run_s(0);  break;
          case 'p': print(car);break;
          default : printf("�������������������\n");break;
          
          }
          
        
        }
      }
    else
      {
        for(i=0;i<5;i++)
        {
          num[i] = uart_getchar(UART0) - '0';
        }
        data = num[0]*100 + num[1] * 10 + num[2]  + num[3] * 0.1 +num[4] *0.01;
       switch(str)
       {
       case 'a': car->angle_set = data ; printf("angle_set:%f\n",car->angle_set);break;
       case 'P':
       case 'p':  car->speed_p = data;printf("speed_p:%f\n",car->speed_p);break;
       case 'i':
       case 'I':  car->speed_i = data;printf("speed_i:%f\n",car->speed_i);break;
       case 'D':
       case 'd':  car->speed_d = data;printf("speed_d:%f\n",car->speed_d);break;
       default :break;
      } 
      }
}
}