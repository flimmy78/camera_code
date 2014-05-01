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
#include  "board.h"
#include  "image.h"

/****************************************************ͼ��ɼ�*****************************************************/

extern cars_status car;
volatile u8 vert_flag = 0;       //���ж��б��־
volatile u16  row_count = 0;     //�м���
u8   image[ROW][COL] = {0};   //ͼ������
u8  image_handle_flag = 0;      //ͼ�����־
//u8 err[COL] = {255};

void PORTE_IRQHandler()     //���ж�����
{
    if(PORTE_ISFR & (1 << 11))
    {
        PORTE_ISFR |= (1 << 11);     
    
//        if(vert_flag != 0) return;
        
//        printf("%d\n",row_count);
        
        vert_flag = 1;  //����־��λ
        row_count = 0;  //�м�������
        DMA_count_reset(DMA_CH0);
    }
}

unsigned int count2_temp;
void PORTD_IRQHandler()     //���ж�����
{
    u8 i; //������
   static unsigned int count2;
    
    if(PORTD_ISFR & (1<<14))
    {
        PORTD_ISFR |= (1 << 14);
        if(vert_flag == 0)   return;     //����һ��ʱ����        
        row_count = DMA_count_get(DMA_CH0);
//        printf("%d\n",row_count);
        /*******************���Ʋ���*********************/
        if(row_count%80 == 1)           //5ms�ѵ�
        {
            /*********�˲��������㷨**********/
            //��СҲ��Լ��2.5ms��ʱ��������
            
              car->angle_m = acc_data_get();
              car->gyro_m = gyro_data_get();
              blance_kalman_filter(car);

//              OutData[0] = car->angle;
//              OutData[1] = car->angle_m;
//              OutData[2] = car->gyro;
//              OutData[3] = car->gyro_m;
//              send_toscope();
              
//              printf("%f\t%f\t%f\t%f\n",car->angle,car->angle_m,car->gyro,car->gyro_m);
         
           count2++;
           count2_temp = count2;
         //  printf("%d\n",count2);
          if(count2>=20)
          {
               car->speed_right_m  =  right_speed();
               car->speed_left_m   =  (car->speed_right_m >= 0)? left_speed(): -left_speed();
//             printf("%f\t%f\n",car->speed_left_m,car->speed_right_m);
               speed_control(car);
               count2 = 0;
          }
         speed_control_output(car);
         direction_control_output(car);   //�������ƽ��������������������ͼ�����֡�
         car->left_duty     = car->blance_duty - car->speed_duty - car->direction_left_duty;
         car->right_duty    = car->blance_duty - car->speed_duty + car->direction_right_duty;
         motor_set(car);  
         
//         printf("%d\n",row_count);
    //     gpio_turn(PORTC,14);

        }
        
        /********************ͼ������*********************/
        if((row_count > 161)&&(image_handle_flag == 0))         //�����ο����㷨�������ͼ��δ����
        {
            /**********ͼ����************/
        
            for(i=0;i<40;i++)
            {
              edge_l[i] = image_left_offset(image , i);
              edge_r[i] = image_right_offset(image , i);
            }
            image_err(car, 0 ,39);
            direction_control(car);

//            printf("%f\n",car->direction_err_new);
            image_handle_flag = 1;      //ͼ�����־��1,ͼ��������������
        }
         
        /******************ͼ��ɼ�����*******************/
        if(row_count < ROW_START)   return;     //δ����Ҫ�ɼ�����
        else if(row_count > ROW_END)
        {
            DMA_DIS(DMA_CH4);
            return;
        }
        else
        {
    //          camera_wait();      //Խ��������
            DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTE_BYTE0_IN, image[row_count-ROW_START], PTE27, DMA_BYTE1, COL, DMA_rising);
            //----ʹ��DMA����ʼ����ʱ���ֹDMA
            DMA_EN(DMA_CH4); 
        }
        
        if(row_count == ROW_END)
        {
            image_handle_flag = 0;      //ͼ��ɼ���ɣ�ͼ�����־����
            
//            vert_flag = 2;
        }
    }

}

void DMA_CH4_Handler(void)
{
    //DMAͨ��4
    DMA_IRQ_CLEAN(DMA_CH4); //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    
    DMA_EN(DMA_CH4);                                    //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����)
}


/************************************************************************************************************************************/


u32 a,b,c,d;

void PIT_CH0_Handler()
{
    PIT_Flag_Clear(PIT0);
//    car->speed_left_m=pulse_cnt_left();
//    car->speed_right_m= pulse_cnt_right();
//    speed_control(car);
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
              car->angle_m = acc_data_get();
              car->gyro_m = gyro_data_get();
              blance_kalman_filter(car);
//              OutData[0] = car->angle;
//              OutData[1] = car->angle_m;
//              OutData[2] = car->gyro;
//              OutData[3] = car->gyro_m;
//              send_toscope();
//              printf("%f\t%f\t%f\t%f\n",car->angle,car->angle_m,car->gyro,car->gyro_m);
//              printf("%d\n",ad_ave(ADC1,SE10,ADC_16bit,20));
              break;
    case 2:
              break;
    case 3:
             
              break;
    case 4:
           count2++;
          if(count2==20)
          {
               car->speed_right_m  =  right_speed();
               car->speed_left_m   =  (car->speed_right_m >= 0)? left_speed(): -left_speed();
               speed_control(car);
               count2 = 0;
 //            printf("%d\t%d\n",car->speed_left_m,car->speed_right_m);
          }
         speed_control_output(car);
         car->left_duty     = car->blance_duty - car->speed_duty + car->direction_left_duty;
         car->right_duty    = car->blance_duty - car->speed_duty + car->direction_right_duty;
//         printf("%f\t%f\t%f\n",car->blance_duty,car->speed_duty,car->left_duty);
         motor_set(car);
         break;
    default:
              break;
    }
    
  count1++;
  if(count1 == 5)
      count1 = 0;
       
}

