#include "common.h"
#include "include.h"

/*************************************************************************
*  �������ƣ�USART1_IRQHandler
*  ����˵��������1 �ж� ���� ������
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
void USART1_IRQHandler(void)
{
    uint8 ch; 
    s16 i;
    ch = uart_getchar (UART1);      //���յ�һ������
    switch(ch)
    {
      case '1':  //���յ����š�1��
         sd_p = sd_p + 5;
         //motor_l.pwm=motor_l.pwm+1;
         break;
           
      case '2':  //���յ����š�2��
         sd_p = sd_p - 5;
         //motor_l.pwm=motor_l.pwm-1;
         break;
           
      case '3':  //���յ�����'3'
         sd_d = sd_d + 0.1;
         //motor_r.pwm=motor_r.pwm+1;
         break;
           
      case '4':  //���յ�����'4'
         sd_d = sd_d - 0.1;
         //motor_r.pwm=motor_r.pwm-1;
         break;
         
      case '5':  //���յ����š�5��
         sp_p = sp_p + 0.1;
         break;
           
      case '6':  //���յ����š�6��
         sp_p = sp_p - 0.1;
         break;
         
      case '7':  //���յ����š�5��
         sp_i = sp_i + 0.1;
         break;
           
      case '8':  //���յ����š�6��
         sp_i = sp_i - 0.1;
         break;
         
      case '9':  //���յ����š�5��
         car.SpeedBase += 500;
         break;
             
      case '0':  //���յ����š�6��
         car.SpeedBase -= 500; 
         break;
           
    /*  case 'a':
          for(i=0;i<=499;i++)
          {
            uart_sendStr(UART1,"  ");
            uart_send_3data(lanya_direction_offset[i]);
          }
          uart_send_3data(ccd2_IntegrationTime);
          break;
          
      case 'b':
          ccd2_IntegrationTime++;
          break;
          
      case 'c':
          ccd2_IntegrationTime--;
          break;*/
    
      default:
           ;
           break;       
    }
}


/*************************************************************************
*  �������ƣ�PIT0_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*************************************************************************/
void PIT0_IRQHandler(void) //1ms�ж�
{
  static unsigned char count1 = 0, count2 = 0, count_dr = 0,count_ccd2 = 0;
  
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
  
  //C5_OUT  = 0;  
  ccd1_ControlInterrupt(); //CCD�ع⼰�ɼ����ƺ���//105us
  ccd2_ControlInterrupt(); //CCD�ع⼰�ɼ����ƺ���//105us
  //C5_OUT  = 1; 

  switch(count1)
  {
    case 0: /************ƽ���˲��ɼ�ADֵ***************/
        //PTC5_OUT  = 0;
          
          jt_average_filter();//175us
           
        //PTC5_OUT  = 1;
          break;        
               
    case 1: /***********�������˲�***********/
        //PTC5_OUT  = 0;
        
          //hubu_filter();
          kalman_filter();//112us
          if(kalman_count<2000)
            kalman_count++;
  
        //PTC5_OUT  = 1;
          break;
          
    case 2:  /*****************����������******************/
        
        //PTC5_OUT  = 0;
          count_dr++;
          if(count_dr == 2)//10ms�������ڣ����������(5ms���һ��)//44us
          {
            ccd1_DirectionCalculate();
            DirectionControl();  
            count_dr = 0;
            time++;//ϵͳʱ�䣬10ms����
          }
        //PTC5_OUT  = 1;
          break;
        
    case 3:  /******************�������+ֱ������******************/
        //PTC5_OUT  = 0;
         
          if(time > time_run)
            DirectionControlOutput();//12
          stand_control();         //us
          
          count_ccd2++;
          if(count_ccd2 == 3)
          {
            ccd2_get_black_line();
            count_ccd2=0;
          }
          if(zhangai_flag==1 && time-time_zhangai<=jiansu_time)//���ٹ���
          {       
            sp_p_old = sp_p;
            sp_i_old = sp_i;
            car.SpeedBase = zhangai_speed;
          } 
          if(zhangai_flag==1 && time-time_zhangai>jiansu_time && time-time_zhangai<=zhangai_time)//ȥ���ٶȱջ�����
          {
            sp_p=0;
            sp_i=0;
          }
        //PTC5_OUT  = 1;
          break; 
        
    case 4:   /*************�ٶȿ���+������***************/
        //PTC5_OUT  = 0;  //28us
          
          count2++;  //5ms����
          if(count2 == 20)
          { 
            encoder_read();
            speed_control(); //100ms����==>�ٶȿ�������Ϊ100ms
            count2 = 0;
          }
          speed_control_output();//5ms==>�ٶ��������Ϊ5ms==>�������ٶȷ�20�ξ������      
          motor_l.pwm = motor_l.stand_pwm + motor_l.speed_pwm + motor_l.direction_pwm;
          motor_r.pwm = motor_r.stand_pwm + motor_r.speed_pwm + motor_r.direction_pwm;
          motor_control_output();//���е�����ڣ�����PWM�������ط������м��㣬�����

        //PTC5_OUT  = 1;
          break;
          
    default:
          FTM_PWM_Duty(FTM0,CH1,0);
          FTM_PWM_Duty(FTM0,CH2,0);
          FTM_PWM_Duty(FTM0,CH3,0);
          FTM_PWM_Duty(FTM0,CH4,0);
          car_states = 0;
          break;
          
   }
    
  count1++;
  if(count1 == 5)
    count1 = 0;
}

/*************************************************************************
*  �������ƣ�PIT1_IRQHandler
*  ����˵����PIT1 ��ʱ�жϷ�����
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*************************************************************************/
void PIT1_IRQHandler(void)  //100ms�ж�
{
  static unsigned int count3;
  static unsigned char i = 0;
  
  PIT_Flag_Clear(PIT1);       //���жϱ�־λ 
  
  count3++;
  
  if(run_flag==1 &&  stop_flag==0 && zhangai_flag==0)///////////////�����
    {
      if( car.SpeedAverage<min_speed ) 
      {
         car.SpeedBase += 40;
         if(car.SpeedBase >= min_speed)
         {
            car.SpeedBase = min_speed;
            run_flag=0;       //�������
         }
      }
    }
  
  if(stop_flag==1)////////////////////////////////ͣ��
  {
   if(time-time_stop>=(100000/(s16)car.SpeedAverage) )
     car_states = 0;
  }
  
  
  
  
  
  if(zhangai_flag==1 && time-time_zhangai>zhangai_time)////����������
  {
    zhangai_flag=0;
    sp_p = sp_p_old;
    sp_i = sp_i_old;
    flag3=1;
    PTC7_OUT=1;
  }
  if(flag3==1 && time-time_zhangai<=zhangai_time+200)//2s������
  {
      if( car.SpeedAverage<min_speed ) 
      {
         car.SpeedBase += 30;
         if(car.SpeedBase >= min_speed)
            car.SpeedBase = min_speed;
      }
  }
  if(flag3==1 && time-time_zhangai>zhangai_time+200)//���������
  {
    flag3=0;
    run_flag=0;
  }
    
  
 if(flag1==1)
 {
    if(car.SpeedAverage<=1100)
    {
      dr_p = 2.6;   
      dr_d = 6;  
    }
    if(car.SpeedAverage>=1100 && car.SpeedAverage<=1300)
    {
      dr_p = 3.4;   
      dr_d = 8;  
    }
    if(car.SpeedAverage>=1300 && car.SpeedAverage<=1500)
    {
      dr_p = 4.2;  //2.8
      dr_d = 12;  //2.8
    }
    if(car.SpeedAverage>=1500 && car.SpeedAverage<=1700)
    {
      dr_p = 5.2; //3.5
      dr_d = 17; //4.0 
    }
    if(car.SpeedAverage>=1700 && car.SpeedAverage<=1900)
    {
      dr_p = 6.3;
      dr_d = 20; 
    }
    if(car.SpeedAverage>=1900 && car.SpeedAverage<=2100)
    {
      dr_p = 7.2;
      dr_d = 23;
    }
    if(car.SpeedAverage>=2100)
    {
      dr_p = 8.0;
      dr_d = 25;
    }
 }
  

  
  
  
  
    /*�����˲�/�������˲����� 
    jt_average_filter();  
    //hubu_filter();
    kalman_filter();
    
    buf_update();      //���»�����������֡���ݣ�
    uart_send_buf(buf);//�������ݣ�����֡���ݣ�*/
  
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�SysTick_Handler
*  ����˵����ϵͳ�δ�ʱ���жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��ucos���õõ�
*************************************************************************/
void SysTick_Handler(void)
{
    //    OSIntEnter();
    //    OSTimeTick();
    //    OSIntExit();
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�HardFault_Handler
*  ����˵����Ӳ���Ϸ��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-4    �Ѳ���
*  ��    ע��������LED��˸��ָʾ������Ӳ���Ϸ�
*************************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
        printf("\n****Ӳ���Ϸô���!!!*****\r\n\n");
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�PendSV_Handler
*  ����˵����PendSV��������ϵͳ���ã��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-15    �Ѳ���
*  ��    ע��uC/OS�����л�����
*************************************************************************/
void PendSV_Handler(void)
{
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�PORTA_IRQHandler
*  ����˵����PORTA�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�Լ���ʼ�������
*************************************************************************/
void PORTA_IRQHandler()
{
    u8  n = 0;    //���ź�

    n = 0;
    if(PORTA_ISFR & (1 << n))         //PTA0�����ж�
    {
        PORTA_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */

    }

    n = 26;
    if(PORTA_ISFR & (1 << n))         //PTA16�����ж�
    {
        PORTA_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */
        LED_turn(LED1);                 //LED1��ת
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�PORTB_IRQHandler
*  ����˵����PORTB�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
void PORTB_IRQHandler()
{
    u8  n = 0;    //���ź�

    n = 0;
    if(PORTB_ISFR & (1 << n))         //PTB0�����ж�
    {
        PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */
        LED_turn(LED1);                 //LED1��ת
    }

    n = 10;
    if(PORTB_ISFR & (1 << n))         //PTB10�����ж�
    {
        PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */

    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�FTM0_IRQHandler
*  ����˵����FTM0���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/
void FTM0_IRQHandler()
{


}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�FTM1_IRQHandler
*  ����˵����FTM1���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/
void FTM1_IRQHandler()
{
    u8 s = FTM1_STATUS;             //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
    u8 n;
    FTM1_STATUS = 0x00;               //���жϱ�־λ

    n = 0;
    if( s & (1 << n) )
    {
        FTM_CnSC_REG(FTM1_BASE_PTR, n) &= ~FTM_CnSC_CHIE_MASK; //��ֹ���벶׽�ж�
        /*     �û�����       */
        LED_turn(LED1);                             //��תLED1
        printf("\nFTM1�����ж�\n");

        /*********************/
        //�����������￪�����벶׽�жϣ�������main�����������Ҫ������
        //ͨ�� CH0��CH1��Ch2��Ch3 ���˲���
        //FTM_CnSC_REG(FTM1_BASE_PTR,n) |= FTM_CnSC_CHIE_MASK;  //�������벶׽�ж�
        //delayms(10);        //��Ϊ������ź�������̲��ȶ������״���������벶׽�����������ʱ
        //�����ǵ��жϲ�Ӧ�ù�����ʱ�����Կ����벶׽�жϾͷ���main�����������Ҫ������
    }

    n = 1;
    if( s & (1 << n) )
    {
        FTM_CnSC_REG(FTM1_BASE_PTR, n) &= ~FTM_CnSC_CHIE_MASK; //��ֹ���벶׽�ж�
        /*     �û�����       */


        /*********************/
        //�����������￪�����벶׽�ж�
        //FTM_CnSC_REG(FTM1_BASE_PTR,n) |= FTM_CnSC_CHIE_MASK;  //�������벶׽�ж�
    }

}



