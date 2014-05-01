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

extern volatile u8 vert_flag;       //���ж��б��־
extern volatile u16  row_count;     //�м���
extern u8   image[ROW][COL];   //ͼ������
//extern u8   err[COL];

int16_t edge_l[ROW];                //��ȡͼ����ƫ�
int16_t edge_r[ROW];               //��ȡͼ����ƫ�
u8 image_flag = ROW_START;                //��ȡͼ�����־��
u8  threshold = 150;             //�ڰ���ֵ��
extern volatile u16  row_count;
extern unsigned int count2_temp;

void main()
{ 
    u8 i,j;
    u32 gyro_intr;
    //����ϵͳ����
    DisableInterrupts;
    board_init();
    uart_init(UART4,115200); 
    //����������á�
    car->angle_p   = 201.5;//125.5;//125.5;
    car->gyro_d    = 1.59;//0.8;//0.8;//1.50;
    car->angle_set = 28.5;//37.0;//31.5;
    car->gyro_set  =  0;

    car->speed_set = 900.0;         
    car->speed_p   = 1.9;//10.0;        
    car->speed_i   = 1.0;//1.4;//4.1;//0.92//0.36;//0.0;           
     
    car->direction_p  = 1.5;             //�������p������
    car->direction_d  = 0;             //�������d������
    car->direction_left_duty  = 0;
    car->direction_right_duty = 0;
    
    delayms(2000);
    printf("���������ַ�������ƫֵ\n");
    uart_getchar(UART4);
    for(i=0;i<100;i++)
    {
        gyro_intr += ad_ave(ADC1,SE10,ADC_16bit,20);
        delayms(5);
    }
    
    GYRO_ZERO = gyro_intr/i;
    printf("OK ��ʼ�����\n");
    
    EnableInterrupts;
    
    
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
                       
            case 'r': EnableInterrupts;  break;
            
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
        
        
        if(str == 'B')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->speed_set += 300;printf("b=%f\n",car->speed_set);break;}
                if(uart_getchar(UART4)=='-')
                    {car->speed_set -= 300;printf("b=%f\n",car->speed_set);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'f')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->direction_p += 0.1;printf("fp=%f\n",car->direction_p);break;}
                if(uart_getchar(UART4)=='-')
                    {car->direction_p -= 0.1;printf("fp=%f\n",car->direction_p);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
        if(str == 'F')
        {
            for(;;)
            {
                if(uart_getchar(UART4)=='+')
                {car->direction_d += 0.1;printf("fd=%f\n",car->direction_d);break;}
                if(uart_getchar(UART4)=='-')
                    {car->direction_d -= 0.1;printf("fd=%f\n",car->direction_d);break;}
                if(uart_getchar(UART4)=='b')
                    break;
            }
        }
        
    }
   
#endif
    
//    for(;;)
//    {
//        if(vert_flag == 2)
//        {
//            uart_putchar (UART4, 0x00);
//            uart_putchar (UART4, 0xFF);
//            uart_putchar (UART4, 0x01);
//            uart_putchar (UART4, 0x00);
//
//            for(i=0;i<ROW;i++)
//            {
//                for(j=0;j<COL;j++)
//                    uart_putchar(UART4,image[i][j]);
//                
//            }
//            
////            for(j=0;j<COL;j++)
////                    uart_putchar(UART4,err[j]);
//            
//            vert_flag = 0;
//////            DisableInterrupts;
//        }
//    }
}

