#include "include.h"

#if 1 //Ϊ1�������������������в��Ժ���
void main()
{
  unsigned char count41 = 0,count42 = 0;
  
  SYS_INIT();          //ϵͳ ��ʼ��  
  delay_s(1);
  get_gyr_offset();
  
  enable_irq(68); //��pit0�ж�
  //UART_IRQ_EN(UART1);  //����1�������ж�
  
  wait_kalman();//4s         
  car_states = 1;
  
  flag1=PTE4_IN;//�ٶ�pd����Ӧ����
  flag2=PTE6_IN;//��ֵ΢������
  flag5=1;
  if(flag2==1)
  {
    ccd1_threshold=25;
    ccd2_IntegrationTime=4;
    zhangai_ad_change=30;
    zhangai_ad=135;
  }
  /*��λѡ��*/
  if(PTE2_IN==0 && PTE0_IN==0 && PTE1_IN==0)//1�� ���⵵λ �����������
  {
    max_speed = 1600;
    min_speed = 1600;
    dr_p = 5.2; 
    dr_d = 17; 
    zhangai_speed=800;
    jiansu_time=25;
    zhangai_time=150;
    flag5=0;
  }
  if(PTE2_IN==0 && PTE0_IN==0 && PTE1_IN==1)//2��
  {
    max_speed = 1200;
    min_speed = 1200;
    dr_p = 3.4;   
    dr_d = 8;  
    zhangai_speed=850;
    jiansu_time=25;
    zhangai_time=200;
  }
  if(PTE2_IN==0 && PTE0_IN==1 && PTE1_IN==0)//3��
  {
    max_speed = 1400;
    min_speed = 1400;
    dr_p = 4.2;  //2.8
    dr_d = 12;  //2.8
    zhangai_speed=800;
    jiansu_time=25;
    zhangai_time=180;
  }                
  if(PTE2_IN==0 && PTE0_IN==1 && PTE1_IN==1)//4��
  {
    max_speed = 1600;
    min_speed = 1600;
    dr_p = 5.2; //3.5
    dr_d = 17; //4.0
    zhangai_speed=800;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==0 && PTE1_IN==0)//5��
  {
    max_speed = 1800;
    min_speed = 1800;
    dr_p = 6.3;
    dr_d = 20;
    zhangai_speed=1000;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==0 && PTE1_IN==1)//6��
  {
    max_speed = 2000;
    min_speed = 2000;
    dr_p = 7.2;
    dr_d = 23;
    zhangai_speed=1300;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==1 && PTE1_IN==0)//7��
  {
    max_speed = 2200;
    min_speed = 2200;
    dr_p = 8.0;
    dr_d = 25;
    zhangai_speed=1500;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==1 && PTE1_IN==1)//8��  ���⵵λ �����������
  {
    max_speed = 1800;
    min_speed = 1800;
    dr_p = 6.3;
    dr_d = 20;
    zhangai_speed=1000;
    jiansu_time=25;
    zhangai_time=150;
    flag5=0;
  }

  while(time < time_run);
  enable_irq(69);
  run_flag = 1;     //��ģ����
  PTC5_OUT  = 0;

  while(1)
  {
    if(car.th-g_th>30 || car.th-g_th<-35)
      car_states = 0;
    
    if(stop_line==1 && straight_flag==1 && flag5==1 && time>=time_run+1000 )
    {
      time_stop=time;
      stop_flag=1;    //��ģֹͣ
      PTC5_OUT  = 1;
    }

    if(zhangai==1 && straight_flag==1 && zhangai_flag==0 && time>=time_run+300 )
    {
      time_zhangai=time;
      zhangai_flag=1;//��ģ����
      PTC7_OUT  = 0;
    }
    
    if(ccd1_TimerFlag10ms == 1)   //10ms���ݲɼ�����
    { 
      ccd1_TimerFlag10ms = 0;
     
      ccd1_ImageCapture();   //�ɼ�CCD����//970us//��ȥPIT1�жϣ�ϵͳ����Ĳɼ�ʱ��Ϊ(1000-228)us*��10-IntegrationTime����1.���̲ɼ�ʱ��;2,����jt_average_filter();ʱ�䣻3����СIntegrationTime
      
      ccd1_CalculateIntegrationTime();   //�Ե����ع�ʱ��
        
      if(++count41 >= 5)//������������λ����ʾ(50ms��������)
      {
        count41 = 0;
        //SendImageData(ccd1_Pixel);
        
        //ccd1_PixelAverageValue = PixelAverage(10,117,ccd1_Pixel);
        //uart_sendStr(UART1,"\n");uart_send_3data(ccd1_PixelAverageValue);
      }
    } 
    
    if(ccd2_TimerFlag15ms == 1 && ccd1_TimerFlag10ms == 0 )   //10ms���ݲɼ����ڣ������Ȳɼ�CCD1�����ݣ��ٲɼ�CCD2������
    { 
      ccd2_TimerFlag15ms = 0;
     
      ccd2_ImageCapture();   //�ɼ�CCD����//970us//��ȥPIT1�жϣ�ϵͳ����Ĳɼ�ʱ��Ϊ(1000-228)us*��10-IntegrationTime����1.���̲ɼ�ʱ��;2,����jt_average_filter();ʱ�䣻3����СIntegrationTime
        
      if(++count42 >= 5)//������������λ����ʾ(50ms��������)
      {
        count42 = 0;
        //SendImageData(ccd2_Pixel);
        
        //ccd2_PixelAverageValue = PixelAverage(10,117,ccd2_Pixel);
        //uart_sendStr(UART1,"\n");uart_send_3data(ccd2_PixelAverageValue);
      }
    }
    
    /*����ֱ������
    uart_sendStr (UART1,"\nsd_p:");uart_send_3data(sd_p);
    uart_sendStr (UART1,"  sd_d:");uart_send_2_1data(sd_d);
    uart_sendStr (UART1,"  sp_p:");uart_send_2_1data(sp_p);
    uart_sendStr (UART1,"  sp_i:");uart_send_2_1data(sp_i);
    uart_sendStr (UART1,"  speed:");uart_send_4data(car.SpeedAverage);
    delay_ms(100);*/
  }
}


   
#else
void main(void)  //���Ժ���
{
  unsigned char count4 = 0;

  SYS_INIT();          //ϵͳ ��ʼ��
  delay_s(1);
  get_gyr_offset();
  
  enable_irq(68); //��pit0�ж�
  //enable_irq(69); //��pit1�ж�
  UART_IRQ_EN(UART1);  //����1�������ж�
  
  wait_kalman();//5s 
  car_states = 1;


  while(1)
  {
    if(car.th-g_th>25 || car.th-g_th< -30)
      car_states = 0;
    
      
    /*���Լ��ٶ�������ADֵ
    uart_sendStr (UART1,"\n������Xad:");uart_send_4data(ad_gyrx);
    //uart_sendStr (UART1,"  ���ٶ�Yad:");uart_send_4data(ad_accy);
    uart_sendStr (UART1,"  ���ٶ�Zad:");uart_send_4data(ad_accz);
    uart_sendStr (UART1,"  ");uart_send_4data(gyrx_offset);
    //uart_sendStr (UART1,"  ");uart_send_4data(accy_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accz_offset);
    delay_ms(200);*/
    
    /*���Լ��ٶ������ǽǶȽ��ٶ�ֵ
    uart_sendStr (UART1,"\n");uart_send_2_1data(acc_th);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.th);
    uart_sendStr (UART1,"       ");uart_send_2_1data(gyr_xw);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.w);
    //uart_sendStr (UART1,"   ");uart_send_3data(gyr_xw_integral);
    delay_ms(10);*/
    
    
    /*���������� 
    uart_sendStr (UART1,"\n������������");uart_send_4data(motor_l.pulse);
    uart_sendStr (UART1,"  ������������");uart_send_4data(motor_r.pulse);
    uart_sendStr (UART1,"    �����ٶȣ�");uart_send_4data(car.SpeedAverage);
    delay_ms(100);*/

    
    /*��������
    motor_set(motor_left, motor_l.pwm);
    motor_set(motor_right,motor_r.pwm);
    uart_sendStr (UART1,"\nl_pwm:");uart_send_3data(motor_l.pwm);
    uart_sendStr (UART1,"  r_pwm:");uart_send_3data(motor_r.pwm);
    delay_ms(300);*/
    
    
    /*����ֱ������
    uart_sendStr (UART1,"\nsd_p:");uart_send_3data(sd_p);
    uart_sendStr (UART1,"  sd_d:");uart_send_2_1data(sd_d);
    delay_ms(200);*/
    
        
    /*�����ٶȲ���
    uart_sendStr (UART1,"\nsp_p:");uart_send_2_1data(sp_p);
    uart_sendStr (UART1,"  sp_i:");uart_send_2_1data(sp_i);
    uart_sendStr (UART1,"  speed:");uart_send_4data(car.SpeedAverage);
    delay_ms(200);*/
   
    /*���뿪�ز���
    uart_sendStr (UART1,"\n");  
    uart_send_3data(PTE1_IN);
    uart_send_3data(PTE0_IN);
    uart_send_3data(PTE2_IN);

    uart_send_3data(PTE4_IN);
    uart_send_3data(PTE6_IN);
    uart_send_3data(PTE8_IN);
    uart_send_3data(PTE10_IN);
    uart_send_3data(PTE12_IN);*/
    
  }
}

#endif

