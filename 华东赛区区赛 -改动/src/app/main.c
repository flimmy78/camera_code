/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************/

#include "include.h"

#if 0  //Ϊ1�������������������в��Ժ���
void main()
{
  SYS_INIT();  //ϵͳ ��ʼ��
	
}



#else
void main(void)  //���Ժ���
{
  SYS_INIT();          //ϵͳ ��ʼ��
  delay_ms(500);
  get_gyr_offset();
  
  //wait_kalman();//5s                       
  //enable_irq(68); //��pit0�ж�
  //enable_irq(69); //��pit1�ж�
  UART_IRQ_EN(UART1);  //���� 1   �������ж�
  
  car_states = 1;
  
  
  //motor_set(motor_left, 100);
  //motor_set(motor_right,100);
  FTM_PWM_Duty(FTM0,CH1,100);
  FTM_PWM_Duty(FTM0,CH2,200);
  FTM_PWM_Duty(FTM0,CH3,300);
  FTM_PWM_Duty(FTM0,CH4,400);


  while(1)
  {
    /*���Լ��ٶ�������ADֵ
    uart_sendStr (UART1,"\n������Xad:");uart_send_4data(ad_gyrx);
    uart_sendStr (UART1,"  ���ٶ�Yad:");uart_send_4data(ad_accy);
    uart_sendStr (UART1,"  ���ٶ�Zad:");uart_send_4data(ad_accz);
    uart_sendStr (UART1,"  ");uart_send_4data(gyrx_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accy_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accz_offset);
    delay_ms(200);*/
    
    /*���Լ��ٶ������ǽǶȽ��ٶ�ֵ
    uart_sendStr (UART1,"\n");uart_send_2_1data(acc_th);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.th);
    uart_sendStr (UART1,"       ");uart_send_2_1data(gyr_xw);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.w);
    delay_ms(10);*/
    
    
    /*���������� */
    uart_sendStr (UART1,"\n������������");uart_send_4data(motor_l.pulse);
    uart_sendStr (UART1,"  ������������");uart_send_4data(motor_r.pulse);
    uart_sendStr (UART1,"    �����ٶȣ�");uart_send_4data(car.SpeedAverage);
    delay_ms(100);
    
  }
  
}

#endif

