/******************** (C) COPYRIGHT 2012-2013 �Ĭ�Ƽ� DEMOK*********
 * �ļ���         �� main.c
 * ����           �� ������ͨ��ģ��
 *
 * ʵ��ƽ̨       �� �Ĭ�Ƽ�DEMOK Kinetis������
 * ����           �� �Ĭ�Ƽ�DEMOK Kinetis����С��

 * �Ա�����       �� http://shop60443799.taobao.com/
 * ������������   �� 1030923155@qq.com 
 * ��������QQȺ   �� 103360642

* ����޶�ʱ��    ��2012-11-7
* �޶�����        ����
**********************************************************************************/

#include "common.h"
#include "include.h"

/*************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*                                  OV7620ͼ��ɼ�ʵ��
*
*  ʵ��˵����ͨ��Kinetis��DMA��EXTI����OV7640����ͷ
*
*  ʵ�������Kinetis��OV7620���߷�ʽ��
*                       Kinetis            OV7640
*                       PTD[0:7]           Y[0:7]
*                        PTE7               PCLK
*                        PTD10              HREF
*                        PTE0               VSYN
*                        PTC7               SCL
*                        PTC8               SDA
*                        GND                GND
*             Kinetis�봮��ģ����߷�ʽ��
*                   PTC3��RX    PTC4��TX   Ĭ�ϴ���1 ������115200
*            OV7640�ֱ������320*240
*
*
*  ʵ��Ч������DEMOK�ṩ��DemokTool��λ���п��Կ����ɼ�����ͼ��
*
*  ʵ��Ŀ�ģ�ѧϰDMA OV7640ʱ��
*
*  �޸�ʱ�䣺2012-11-14
*
*  ��    ע��Kinetis��3.3V���磬OV7640��3.3V���磬IO����
*
*  ˵    ����DEMOKԭ�� �����ο� ���𿽱�
*************************************************************************/

u8 ImageBuf[ROW][COL];
u8 lcdis[48][84];


volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;

void main()
{
    u16  i;
    u16  j;

    
    
    //----��ʼ��ͼ������----//
    for(i=0; i<ROW; i++)
    {
      for(j=0;j<COL ;j++)
      {
        ImageBuf[i][j] = 0;
      }
    }
    
    
    //----��ʼ��LCD-------//   
    LCD_init();     
	LCD_clear();
    
    
    DisableInterrupts;  
    
    //----��ʼ���ⲿ�ж�---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTC16 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    exti_init(PORTE,  0, rising_down);      //VSYN----PORTC18 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����

    EnableInterrupts;
    
    
    while(SampleFlag == 2)
    {
      for(i=0;(i>=20)&&((i-20)%6==0)&&(i<=302);i++)
      {
           for(j=0;(j>=20)&&((j-21)%2==0)&&(j<=186);j++)
           {
               lcdis[(i-20)/6][(j-20)/2] = ImageBuf[i][j];
           }
      }
      
      LCD_draw_pixel(lcdis,48,84,100,0,0);
      delay();
      
      SampleFlag = 0;
      
      EnableInterrupts;
    }
      
}