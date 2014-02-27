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

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;


/*-----------------ʹ�ô������˽���һ��ͼ�����ݵ�������-------------*/

void main()
{
    u16  i;
    u16  j;

    //----��ʼ��ͼ������----//
    for(i=0; i<ROW; i++)
    {
      	for(j=0; j<COL; j++)
      	{
       	 	ImageBuf[i][j] = 0;
     	}
    }
    
    uart_init(UART1,115200);
    
    FTM_PWM_init(FTM0,CH0,5000000,80);       //����PWM������DMA������Ƶ��5MHz
    
    DisableInterrupts;  
    
    //----��ʼ���ⲿ�ж�---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTD10 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    exti_init(PORTE,  0, rising_down);       //VSYN----PORTE0 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����

    EnableInterrupts;

    for(;;)
    {
      while(SampleFlag == 1);				//SampleFlag==1,��Ƭ�������ڲɼ�ͼ��ʱ���

      for(i=0;i<ROW;i++)
      {
          for(j=0;j<COL;j++)
          {
              printf("%-4d",ImageBuf[i][j]);
          }
          printf("\n");
      }
      
      EnableInterrupts;
    }
    
         
}