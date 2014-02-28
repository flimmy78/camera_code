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
#include "include.h"/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
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
            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, ImageBuf[rowCnt-99], PTE7, DMA_BYTE1, COL, DMA_rising);
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
    DMA_IRQ_CLEAN(DMA_CH4);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    DMA_IRQ_DIS(DMA_CH4);                               //��ֹDMA   �ȴ���һ�����ж����ٿ���DMA
}



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



/*----------------ʹ�ô������˽���һ��ͼ�����ݵ��жϷ������--------------------*/

//void PORTD_IRQHandler()
//{
//   //---HREF���жϴ���
//    if( PORTD_ISFR & (1 << 9))         //PTD10�����ж�  
//    {
//        PORTD_ISFR  |= (1 << 9);        //д1���жϱ�־λ
//        
//         if ( SampleFlag == 0 )         //����һ��ʱ����
//        { 
//          return;
//        }
//        
//        delayms(2);                     //��ʱ1.6ms������������
        
        
//        if(rowCnt < ROW)
//        {
//			delayus(2);
			
//            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, ImageBuf[rowCnt], PTE7, DMA_BYTE1, COL, DMA_rising);
            //----ʹ��DMA����ʼ����ʱ���ֹDMA
//            DMA_EN(DMA_CH4); 
        //-------------DAM��ʼ��ͨ��4������ԴΪPTD��ÿ�δ�������ImageBuf[]ָ���У�PCLK��PTE7������ÿ�δ���1���ֽڣ�ÿ�δ�������320�Σ������ش���
//            rowCnt++;
			
//			printf("%d ",rowCnt);
//        }
//        else
//        {
//          rowCnt = 0;
//          DMA_DIS(DMA_CH4);
//          
//          //---------��Ϊ��̬��ʾ��ɾ������------------//
//          DisableInterrupts;
//          
//          SampleFlag = 2;
//		  
////		  printf("%d ",SampleFlag);
//        }
//    }
//
//}

void PORTC_IRQHandler()
{
    //---VSYN���жϴ���
    if(PORTC_ISFR & (1 << 16))
    {
        PORTC_ISFR |= (1 << 16);
		
		delayms(10);
		
        rowCnt = 0;
//        SampleFlag = 1;
		SampleFlag++;
		
//		printf("%d\n",SampleFlag);
//		printf("\nSampleFlag = %d\n",SampleFlag);
    }
}

//u8 VSYN_Flag = 0;
//void PORTC_IRQHandler()
//{
//    //---VSYN���жϴ���
//    if(PORTC_ISFR & (1 << 16))
//    {
//        PORTC_ISFR |= (1 << 16);
//        
//		if(VSYN_Flag == 0)
//		{
//			/* ����ʱ��*/
//			SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //ʹ��PITʱ��
//			/* PITģ����� PIT Module Control Register (PIT_MCR) */
//			PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
//			
//			PIT_LDVAL0 = 0xFFFFFFFF;
//			PIT_CVAL0  = 0x00000000;
//			PIT_TFLG0  = 0x00000001;
//			PIT_TCTRL0 = 0x00000001;
//			
//			VSYN_Flag = 1;
//		}
//		else
//		{
//			PIT_TCTRL0 = 0x00000000;
//			
//			VSYN_Flag = 0;
//			
//			printf("PIT_LDVAL0 = %u\n",PIT_LDVAL0);
//			printf("PIT_CVAL0  = %u\n",PIT_CVAL0);
//			
//			//PIT_TCTRL0 = 0x00000000;
//		}
//
//    }
//}

/*************************************************************************
*                            �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�DMA_CH4_Handler
*  ����˵����DMAͨ��4���жϷ�����
*  ����˵������
*  �������أ���
*
*************************************************************************/
//void DMA_CH4_Handler(void)
//{
//    //DMAͨ��4
//    DMA_IRQ_CLEAN(DMA_CH4);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
//    DMA_IRQ_DIS(DMA_CH4);                               //��ֹDMA   �ȴ���һ�����ж����ٿ���DMA
//}

