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



#include "common.h"
#include "include.h"

/*************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�PORTC_IRQHandler
*  ����˵����PORTC�˿��жϷ�����
*  ����˵������
*  �������أ���
*  ��    ע�����ź���Ҫ�Լ���ʼ�������
*
*************************************************************************/
extern u32 rowCnt ;   //�м���
extern u8 ImageBuf[ROW][COL];
extern u8 SampleFlag;

u8 i;

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
        
        
        //-------------DAM��ʼ��ͨ��4������ԴΪPTD��ÿ�δ�������ImageBuf[]ָ���У�PCLK��PTE7������ÿ�δ���1���ֽڣ�ÿ�δ�������320�Σ������ش���
        DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, ImageBuf[rowCnt], PTE7, DMA_BYTE1, COL, DMA_rising);
        //----ʹ��DMA����ʼ����ʱ���ֹDMA
        DMA_EN(DMA_CH4); 

        rowCnt++;

        if(rowCnt == ROW)
        {
          rowCnt = 0;
          DMA_DIS(DMA_CH4);
          
          //---------��Ϊ��̬��ʾ��ɾ������------------//
          DisableInterrupts;
          
          SampleFlag = 2;
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
    DMA_IRQ_CLEAN(DMA_CH4);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    DMA_IRQ_DIS(DMA_CH4);                               //��ֹDMA   �ȴ���һ�����ж����ٿ���DMA
 
}