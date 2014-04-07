/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��isr.c
 * ����         ���жϴ�������
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	



#include  "common.h"
#include  "include.h"
#include  "LED_CORE.h"


extern u32 rowCnt ;   //�м���
extern u8 ImageBuf[ROW][COL];
extern u8 SampleFlag;



/*----------------ʹ�ô������˽���һ��ͼ�����ݵ��жϷ������--------------------*/

void PORTD_IRQHandler()
{
   //---HREF���жϴ���
    if( PORTD_ISFR & (1 << 10))         //PTD10�����ж�  
    {
        PORTD_ISFR  |= (1 << 10);        //д1���жϱ�־λ
        
         if ( SampleFlag == 0 )         //����һ��ʱ����
        { 
          return;
        }
        
        delayms(2);                     //��ʱ1.6ms������������
        
        
        if(rowCnt < ROW)
        {
            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTB_BYTE0_IN, ImageBuf[rowCnt-99], PTE7, DMA_BYTE1, COL, DMA_rising);
            //----ʹ��DMA����ʼ����ʱ���ֹDMA
            DMA_EN(DMA_CH4); 
        //-------------DAM��ʼ��ͨ��4������ԴΪPTD��ÿ�δ�������ImageBuf[]ָ���У�PCLK��PTE7������ÿ�δ���1���ֽڣ�ÿ�δ�������320�Σ������ش���
            rowCnt++;
        }
        else
        {
          rowCnt = 0;
          DMA_DIS(DMA_CH4);
          
          //---------��Ϊ��̬��ʾ��ɾ������------------//
          DisableInterrupts;
          
          SampleFlag = 0;
        }
    }

}

void PORTE_IRQHandler()
{
    //---VSYN���жϴ���
    if(PORTE_ISFR & (1 << 0))
    {
        PORTE_ISFR |= (1 << 0);
        rowCnt = 0;
        SampleFlag = 1;
    }
}

/*************************************************************************
*                            �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�DMA_CH4_Handler
*  ����˵����DMAͨ��4���жϷ�����
*  ����˵������
*  �������أ���
*
*************************************************************************/
void DMA_CH4_Handler(void)
{
    //DMAͨ��4
    DMA_IRQ_CLEAN(DMA_CH4); //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    
    DMA_EN(DMA_CH4);                                    //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����)
}

void DMA_CH1_Handler(void)
{
    //DMAͨ��4
    DMA_IRQ_CLEAN(DMA_CH1); //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    
    DMA_EN(DMA_CH1);                                    //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����)
}

extern cars_status car;
u32 a,b,c,d;

void PIT_CH0_Handler()
{
    PIT_Flag_Clear(PIT0);
    car->speed_left_m=pulse_cnt_left();
    car->speed_right_m= pulse_cnt_right();
    speed_control(car);
}

void PIT_CH1_Handler(void)
{
   
    PIT_Flag_Clear(PIT1); 
    static unsigned int count1,count2;
    switch(count1)
    {
    case 0:
              break;
    case 1:
              break;
    case 2:
              break;
    case 3:
              blance_comp_filter(3.5,0.005,car);
              break;
    case 4:
            count2++;
          if(count2==20)
          {
               car->speed_left_m  = 1000*left_speed();
               car->speed_right_m = 1000*right_speed();
               speed_control(car);
               count2=0;
               printf("%f\t%f\n",car->speed_duty,car->left_duty);
          }
          
         speed_control_output(car);
         car->left_duty     = car->blance_duty + car->speed_duty + car->direction_left_duty;
         car->right_duty    = car->blance_duty + car->speed_duty + car->direction_right_duty;
         motor_set(car);
         break;
         
    default:
              break;
    }
    
  count1++;
  
  if(count1 == 5)
      count1 = 0;
    
    
}

char str[10];
u8 len;
float num;
void UART0_IRQHandler(void)
{
    u8 i;
    DisableInterrupts;
    
    uart_pendstr(UART0,str);
    if(str[0] == '\0')
    {
        EnableInterrupts;
        return;
    }
    len = strlen(str);
    
    if((str[0] == 'p')||(str[0] == 'P'))
    {
        for(i=0; i<len-1;i++)
        {
            str[i] = str[i+1];
        }
        car->speed_p = str2ufloat(str,len-1);
        printf("you send p = %f\n",car->speed_p);
        
        /*****�û�����*********/
        
        
        
    }
    else if((str[0] == 'd')||(str[0] == 'D'))
    {
        for(i=0; i<len-1;i++)
        {
            str[i] = str[i+1];
        }
        car->speed_d = str2ufloat(str,len-1);
        printf("you send d = %f\n",car->speed_d );
        
        /*****�û�����*********/
        
        
        
    }

    EnableInterrupts;
}

