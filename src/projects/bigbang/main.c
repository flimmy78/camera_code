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

u8 ImageBuf[ROW][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;


/*-----------------ʹ�ô������˽���һ��ͼ�����ݵ�������-------------*/

void main()
{
  char s[20];
  uint16_t AD;
  Light_init;
  uart_init(UART0,115200);
  LCD_init();
  LCD_write_english_string(0,0,"hello");
  adc_init(ADC0,AD8);
  
  while(1)
  {
    AD   =   ad_once(ADC0,AD8,ADC_16bit);
   // uart_putchar   (UART0, (char) AD);
   printf("%u,",AD);
   
    
//    sprintf(s,"%u",AD);                        //���øú�������������ת��Ϊ�ַ���
//    LCD_write_english_string(0,0,s);
//    printf("%u\t",AD);
//    AD = ad_once(ADC0,AD9,ADC_16bit);
//    sprintf(s,"%u",AD); 
//    LCD_write_english_string(0,1,s);
//    printf("%u\n",AD);
  }
}
