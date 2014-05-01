/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************/

#include "include.h"

#if 0  //为1运行主函数，否则运行测试函数
void main()
{
  SYS_INIT();  //系统 初始化
	
}



#else
void main(void)  //测试函数
{
  SYS_INIT();          //系统 初始化
  delay_ms(500);
  get_gyr_offset();
  
  //wait_kalman();//5s                       
  //enable_irq(68); //开pit0中断
  //enable_irq(69); //开pit1中断
  UART_IRQ_EN(UART1);  //串口 1   开接收中断
  
  car_states = 1;
  
  
  //motor_set(motor_left, 100);
  //motor_set(motor_right,100);
  FTM_PWM_Duty(FTM0,CH1,100);
  FTM_PWM_Duty(FTM0,CH2,200);
  FTM_PWM_Duty(FTM0,CH3,300);
  FTM_PWM_Duty(FTM0,CH4,400);


  while(1)
  {
    /*测试加速度陀螺仪AD值
    uart_sendStr (UART1,"\n陀螺仪Xad:");uart_send_4data(ad_gyrx);
    uart_sendStr (UART1,"  加速度Yad:");uart_send_4data(ad_accy);
    uart_sendStr (UART1,"  加速度Zad:");uart_send_4data(ad_accz);
    uart_sendStr (UART1,"  ");uart_send_4data(gyrx_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accy_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accz_offset);
    delay_ms(200);*/
    
    /*测试加速度陀螺仪角度角速度值
    uart_sendStr (UART1,"\n");uart_send_2_1data(acc_th);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.th);
    uart_sendStr (UART1,"       ");uart_send_2_1data(gyr_xw);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.w);
    delay_ms(10);*/
    
    
    /*编码器测速 */
    uart_sendStr (UART1,"\n左轮脉冲数：");uart_send_4data(motor_l.pulse);
    uart_sendStr (UART1,"  右轮脉冲数：");uart_send_4data(motor_r.pulse);
    uart_sendStr (UART1,"    汽车速度：");uart_send_4data(car.SpeedAverage);
    delay_ms(100);
    
  }
  
}

#endif

