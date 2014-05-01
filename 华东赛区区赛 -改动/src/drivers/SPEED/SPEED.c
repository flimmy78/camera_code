#include "include.h"


float CarSpeedAverageOld=0;
/*********************************************************
----------------------------------------------------------
读取编码器：每5ms读取一次编码器值，把读取的值累计在pulse中，
累计到20次时(100ms)，计算速度进行速度控制，并清零pulse
----------------------------------------------------------
**********************************************************/
void encoder_read(void)
{ 
  motor_l.pulse = DMA_count_get(DMA_CH0);	//----读脉冲数; 
  DMA_count_reset(DMA_CH0);			//----计数清零 
  if(gpio_get(PORTA,27) == 1)
    motor_l.pulse = -motor_l.pulse;

  motor_r.pulse = DMA_count_get(DMA_CH2);	//----读脉冲数;
  DMA_count_reset(DMA_CH2);			//----计数清零
  if(gpio_get(PORTA,26) == 0)
    motor_r.pulse = -motor_r.pulse;
}

/************************************************************
-------------------------------------------------------------
速度控制调用周期100ms  PI控制
包含单位换算(mm/s)
 
-------------------------------------------------------------
*************************************************************/
void speed_control(void)
{
  float SpeedAverage;
  static float SpeedErrorLast;
  
  CarSpeedAverageOld = car.SpeedAverage;
  
  SpeedAverage = (motor_l.pulse + motor_r.pulse)/2.0;  //平均脉冲数
  
  car.SpeedAverage = SpeedAverage*2;//5为轮子滚动1mm产生5个脉冲，时间为0.1s（单位：mm/s)
  
  //if(car.SpeedAverage-CarSpeedAverageOld>1500 || car.SpeedAverage-CarSpeedAverageOld<-1500)///////消去突变
    //car.SpeedAverage = CarSpeedAverageOld;
  
  car.SpeedError = car.SpeedAverage - car.SpeedBase; //实际值 - 给定值

  car.Distance = car.Distance + car.SpeedError*0.1;//Distance不是距离，而是速度误差的积分。

  car.Acceleration = car.SpeedError - SpeedErrorLast;//Acceleration不是加速度，而是速度误差的微分。 

  SpeedErrorLast = car.SpeedError;

  if(car.Distance>=200)  //积分饱和消除
  {
    car.Distance=200;
  }
  else if(car.Distance<=-200)
  {
    car.Distance=-200;
  }  

  motor_l.speed_pwm_old = motor_l.speed_pwm_new; 
  motor_r.speed_pwm_old = motor_r.speed_pwm_new;

  motor_l.speed_pwm_new = car.SpeedError*sp_p + car.Distance*sp_i;  //要用正反馈，速度误差计算时是实际值-给定值
  motor_r.speed_pwm_new = car.SpeedError*sp_p + car.Distance*sp_i; 
}

/*********************************************************
----------------------------------------------------------
函数名称speed_control_output(void)
函数功能：该函数将速度输出变化量平均到20个控制周期中  
          其中 SpeedControl（）每100MS一次，中断5MS  
          分成20次线性调速
----------------------------------------------------------
**********************************************************/
void speed_control_output(void) 
{    
  float value_l,value_r;    
  value_l=(motor_l.speed_pwm_new - motor_l.speed_pwm_old)/20.0;
  value_r=(motor_r.speed_pwm_new - motor_r.speed_pwm_old)/20.0;
  motor_l.speed_pwm += value_l;
  motor_r.speed_pwm += value_r;              
}