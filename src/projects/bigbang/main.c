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

#include "common.h"
#include "include.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "board_config.h"
#include "init.h"
#include "board.h"
#include "Kalman.h"

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;


/*-----------------ʹ�ô������˽���һ��ͼ�����ݵ�������-------------*/

void main()
{
// DisableInterrupts;
//  board_init();  
//  uart_init(UART0,115200);
// // EnableInterrupts;
//
//  while(1)
//  {
//    printf("hello");
//   Light3_turn();
//  }
  
   uint16_t  angle_m;
     uint16_t  gyro_m;
     DisableInterrupts;
     board_init();
     uart_init(UART0,115200);
    while(1)
    {
      uint16_t acc_m,gyro_m;
      angle_m   = acc_data_get();
      gyro_m = gyro_data_get(); 
      Kalman_Filter(acc_m,gyro_m);
      sent_to_computer((uint16_t)angle_m , (uint16_t)gyro_m ,(uint16_t)angle);
      delayms(1);
       
    }
 
  
}
