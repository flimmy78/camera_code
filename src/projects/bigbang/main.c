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


/*-----------------ʹ�ô������˽���һ��ͼ�����ݵ�������-------------*/

void main()
{ 
  s32 acc_32=1;
  u16 acc;
  float ans1;
  board_init();
  uart_init(UART0,115200);
  left_run(15,ahead);
  right_run(15,ahead);
  printf("getchar\n");
  uart_getchar(UART0);
  while(1)
  {
    left_run_s(acc_32);
    right_run_s(acc_32);
    printf("getchar\n");
    acc = uart_getchar(UART0);
    switch (acc)
    {
    case 'a':
      acc_32++;
      break;
    case 'b':
      acc_32--;
      break;
    case 'd':
      acc_32 = 0-acc_32;
      break;
    }
    printf("%d",acc_32);
    
//    if(acc > 'd')
//      acc_32++;
//    else
//      acc_32--;
//    printf("%d\n",acc_32);
//    
//    acc = acc_data_get();
//    acc_32 = acc;
//    ans1 = asin((acc_32-zero)/g);
//    printf("AD16:%u\tAD32:%d\tasin:%f\n",acc,acc_32,ans1);
  }
}
