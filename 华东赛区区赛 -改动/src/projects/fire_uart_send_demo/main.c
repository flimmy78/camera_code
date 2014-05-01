#include "include.h"

#if 1 //为1运行主函数，否则运行测试函数
void main()
{
  unsigned char count41 = 0,count42 = 0;
  
  SYS_INIT();          //系统 初始化  
  delay_s(1);
  get_gyr_offset();
  
  enable_irq(68); //开pit0中断
  //UART_IRQ_EN(UART1);  //串口1开接收中断
  
  wait_kalman();//4s         
  car_states = 1;
  
  flag1=PTE4_IN;//速度pd自适应开关
  flag2=PTE6_IN;//阈值微调开关
  flag5=1;
  if(flag2==1)
  {
    ccd1_threshold=25;
    ccd2_IntegrationTime=4;
    zhangai_ad_change=30;
    zhangai_ad=135;
  }
  /*档位选择*/
  if(PTE2_IN==0 && PTE0_IN==0 && PTE1_IN==0)//1档 特殊档位 不检测起跑线
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
  if(PTE2_IN==0 && PTE0_IN==0 && PTE1_IN==1)//2档
  {
    max_speed = 1200;
    min_speed = 1200;
    dr_p = 3.4;   
    dr_d = 8;  
    zhangai_speed=850;
    jiansu_time=25;
    zhangai_time=200;
  }
  if(PTE2_IN==0 && PTE0_IN==1 && PTE1_IN==0)//3档
  {
    max_speed = 1400;
    min_speed = 1400;
    dr_p = 4.2;  //2.8
    dr_d = 12;  //2.8
    zhangai_speed=800;
    jiansu_time=25;
    zhangai_time=180;
  }                
  if(PTE2_IN==0 && PTE0_IN==1 && PTE1_IN==1)//4档
  {
    max_speed = 1600;
    min_speed = 1600;
    dr_p = 5.2; //3.5
    dr_d = 17; //4.0
    zhangai_speed=800;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==0 && PTE1_IN==0)//5档
  {
    max_speed = 1800;
    min_speed = 1800;
    dr_p = 6.3;
    dr_d = 20;
    zhangai_speed=1000;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==0 && PTE1_IN==1)//6档
  {
    max_speed = 2000;
    min_speed = 2000;
    dr_p = 7.2;
    dr_d = 23;
    zhangai_speed=1300;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==1 && PTE1_IN==0)//7档
  {
    max_speed = 2200;
    min_speed = 2200;
    dr_p = 8.0;
    dr_d = 25;
    zhangai_speed=1500;
    jiansu_time=25;
    zhangai_time=150;
  }
  if(PTE2_IN==1 && PTE0_IN==1 && PTE1_IN==1)//8档  特殊档位 不检测起跑线
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
  run_flag = 1;     //车模启动
  PTC5_OUT  = 0;

  while(1)
  {
    if(car.th-g_th>30 || car.th-g_th<-35)
      car_states = 0;
    
    if(stop_line==1 && straight_flag==1 && flag5==1 && time>=time_run+1000 )
    {
      time_stop=time;
      stop_flag=1;    //车模停止
      PTC5_OUT  = 1;
    }

    if(zhangai==1 && straight_flag==1 && zhangai_flag==0 && time>=time_run+300 )
    {
      time_zhangai=time;
      zhangai_flag=1;//车模减速
      PTC7_OUT  = 0;
    }
    
    if(ccd1_TimerFlag10ms == 1)   //10ms数据采集周期
    { 
      ccd1_TimerFlag10ms = 0;
     
      ccd1_ImageCapture();   //采集CCD像素//970us//除去PIT1中断，系统给予的采集时间为(1000-228)us*（10-IntegrationTime），1.缩短采集时间;2,缩短jt_average_filter();时间；3，减小IntegrationTime
      
      ccd1_CalculateIntegrationTime();   //自调整曝光时间
        
      if(++count41 >= 5)//发送数据至上位机显示(50ms发送周期)
      {
        count41 = 0;
        //SendImageData(ccd1_Pixel);
        
        //ccd1_PixelAverageValue = PixelAverage(10,117,ccd1_Pixel);
        //uart_sendStr(UART1,"\n");uart_send_3data(ccd1_PixelAverageValue);
      }
    } 
    
    if(ccd2_TimerFlag15ms == 1 && ccd1_TimerFlag10ms == 0 )   //10ms数据采集周期，总是先采集CCD1的数据，再采集CCD2的数据
    { 
      ccd2_TimerFlag15ms = 0;
     
      ccd2_ImageCapture();   //采集CCD像素//970us//除去PIT1中断，系统给予的采集时间为(1000-228)us*（10-IntegrationTime），1.缩短采集时间;2,缩短jt_average_filter();时间；3，减小IntegrationTime
        
      if(++count42 >= 5)//发送数据至上位机显示(50ms发送周期)
      {
        count42 = 0;
        //SendImageData(ccd2_Pixel);
        
        //ccd2_PixelAverageValue = PixelAverage(10,117,ccd2_Pixel);
        //uart_sendStr(UART1,"\n");uart_send_3data(ccd2_PixelAverageValue);
      }
    }
    
    /*测量直立参数
    uart_sendStr (UART1,"\nsd_p:");uart_send_3data(sd_p);
    uart_sendStr (UART1,"  sd_d:");uart_send_2_1data(sd_d);
    uart_sendStr (UART1,"  sp_p:");uart_send_2_1data(sp_p);
    uart_sendStr (UART1,"  sp_i:");uart_send_2_1data(sp_i);
    uart_sendStr (UART1,"  speed:");uart_send_4data(car.SpeedAverage);
    delay_ms(100);*/
  }
}


   
#else
void main(void)  //测试函数
{
  unsigned char count4 = 0;

  SYS_INIT();          //系统 初始化
  delay_s(1);
  get_gyr_offset();
  
  enable_irq(68); //开pit0中断
  //enable_irq(69); //开pit1中断
  UART_IRQ_EN(UART1);  //串口1开接收中断
  
  wait_kalman();//5s 
  car_states = 1;


  while(1)
  {
    if(car.th-g_th>25 || car.th-g_th< -30)
      car_states = 0;
    
      
    /*测试加速度陀螺仪AD值
    uart_sendStr (UART1,"\n陀螺仪Xad:");uart_send_4data(ad_gyrx);
    //uart_sendStr (UART1,"  加速度Yad:");uart_send_4data(ad_accy);
    uart_sendStr (UART1,"  加速度Zad:");uart_send_4data(ad_accz);
    uart_sendStr (UART1,"  ");uart_send_4data(gyrx_offset);
    //uart_sendStr (UART1,"  ");uart_send_4data(accy_offset);
    uart_sendStr (UART1,"  ");uart_send_4data(accz_offset);
    delay_ms(200);*/
    
    /*测试加速度陀螺仪角度角速度值
    uart_sendStr (UART1,"\n");uart_send_2_1data(acc_th);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.th);
    uart_sendStr (UART1,"       ");uart_send_2_1data(gyr_xw);
    uart_sendStr (UART1,"   ");uart_send_2_1data(car.w);
    //uart_sendStr (UART1,"   ");uart_send_3data(gyr_xw_integral);
    delay_ms(10);*/
    
    
    /*编码器测速 
    uart_sendStr (UART1,"\n左轮脉冲数：");uart_send_4data(motor_l.pulse);
    uart_sendStr (UART1,"  右轮脉冲数：");uart_send_4data(motor_r.pulse);
    uart_sendStr (UART1,"    汽车速度：");uart_send_4data(car.SpeedAverage);
    delay_ms(100);*/

    
    /*死区测量
    motor_set(motor_left, motor_l.pwm);
    motor_set(motor_right,motor_r.pwm);
    uart_sendStr (UART1,"\nl_pwm:");uart_send_3data(motor_l.pwm);
    uart_sendStr (UART1,"  r_pwm:");uart_send_3data(motor_r.pwm);
    delay_ms(300);*/
    
    
    /*测量直立参数
    uart_sendStr (UART1,"\nsd_p:");uart_send_3data(sd_p);
    uart_sendStr (UART1,"  sd_d:");uart_send_2_1data(sd_d);
    delay_ms(200);*/
    
        
    /*测量速度参数
    uart_sendStr (UART1,"\nsp_p:");uart_send_2_1data(sp_p);
    uart_sendStr (UART1,"  sp_i:");uart_send_2_1data(sp_i);
    uart_sendStr (UART1,"  speed:");uart_send_4data(car.SpeedAverage);
    delay_ms(200);*/
   
    /*拨码开关测试
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

