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
#include  "nokia_5110.h"    //nokia_5110 lcd ������


u8 ImageBuf[ROW-200][COL];

volatile u32 rowCnt = 0 ;
volatile u8 SampleFlag = 0;



#if 0

/*-----------------ʹ�ô������˽���һ��ͼ�����ݵ�������-------------*/

void main()
{
    u16  i;
    u16  j;
    LCD_init();
    LCD_write_englis_string(0,0,"hello");

//    LED_init();

    //----��ʼ��ͼ������----//
    for(i=0; i<ROW-200; i++)
    {
      for(j=0;j<COL ;j++)
      {
        ImageBuf[i][j] = 0;
      }
    }
    
    uart_init(UART0,115200);
    printf("hello");
    
    
    DisableInterrupts;  
    
    //----��ʼ���ⲿ�ж�---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTD10 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    exti_init(PORTE,  0, rising_down);       //VSYN----PORTE0 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����

    EnableInterrupts;

    while(SampleFlag != 2);

    for(i=0;i<ROW-200;i++)
    {
        for(j=0;j<COL;j++)
        {
            printf("%-4d",ImageBuf[i][j]);
        }
        printf("\n");
    }
    
    DisableInterrupts;
         
}



#elif 1

/*----------------ʹ����λ������ͼ���������-----------------*/

void main()
{
    u16  i;
    u16  j;
    LCD_init();
    LCD_write_english_string(0,0,"hello");

//    LED_init();

    //----��ʼ��ͼ������----//
    for(i=0; i<ROW-200; i++)
    {
      for(j=0;j<COL ;j++)
      {
        ImageBuf[i][j] = 0;
      }
    }
    
    uart_init(UART0,115200);
    
    
    DisableInterrupts;  
    
    //----��ʼ���ⲿ�ж�---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTD10 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    exti_init(PORTE,  0, rising_down);       //VSYN----PORTE0 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    EnableInterrupts;
    
//    delayms(100);
    
//    enable_irq (91);
    for(;;)
    {
        while(SampleFlag == 2);
        
//        disable_irq (90);
//        disable_irq (91);
          
        uart_putchar (UART0,0);
        uart_putchar (UART0,255);
        uart_putchar (UART0,1);
        uart_putchar (UART0,0);        //������λ��������
            
        for(i=0; i<ROW-200; i++)
        {
          for(j=0;j <COL; j++)
          {
            uart_putchar(UART0,ImageBuf[i][j]);
          }
        }
        
//        for(j=0; j<COL; j++)
//        {
            uart_putchar(UART0,255);
//        }
        
        SampleFlag = 0;
//        EnableInterrupts;
//        enable_irq (91);
//        enable_irq (90);
    }  
}



#else

/*---------------ʹ�ô������˷���N��ͼ�����ݵ�������----------------*/
/*---------------------ÿ��ͼ��������END��β-----------------------*/

#define N 10

void main()
{
    u16  i;
    u16  j;
    u8   n;

//    LED_init();

    //----��ʼ��ͼ������----//
    for(i=0; i<ROW-200; i++)
    {
      for(j=0; j<COL ;j++)
      {
        ImageBuf[i][j] = 0;
      }
    }
    
    uart_init(UART0,115200);
    
    
    DisableInterrupts;  
    
    //----��ʼ���ⲿ�ж�---//
    exti_init(PORTD, 10, rising_down);       //HREF----PORTD10 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����
    exti_init(PORTE,  0, rising_down);       //VSYN----PORTE0 �˿��ⲿ�жϳ�ʼ�� �������ش����жϣ��ڲ�����

    EnableInterrupts;

    for(n=0; n<N; n++)
    {
        while(SampleFlag != 2);

        for(i=0;i<ROW-200;i++)
        {
            for(j=0;j<COL;j++)
            {
                printf("%-4d",ImageBuf[i][j]);
            }
            printf("\n");
        }       
        printf("END\n");
        
        SampleFlag = 0;
        EnableInterrupts;
    }
}

#endif