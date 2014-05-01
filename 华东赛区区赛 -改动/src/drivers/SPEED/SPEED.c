#include "include.h"


float CarSpeedAverageOld=0;
/*********************************************************
----------------------------------------------------------
��ȡ��������ÿ5ms��ȡһ�α�����ֵ���Ѷ�ȡ��ֵ�ۼ���pulse�У�
�ۼƵ�20��ʱ(100ms)�������ٶȽ����ٶȿ��ƣ�������pulse
----------------------------------------------------------
**********************************************************/
void encoder_read(void)
{ 
  motor_l.pulse = DMA_count_get(DMA_CH0);	//----��������; 
  DMA_count_reset(DMA_CH0);			//----�������� 
  if(gpio_get(PORTA,27) == 1)
    motor_l.pulse = -motor_l.pulse;

  motor_r.pulse = DMA_count_get(DMA_CH2);	//----��������;
  DMA_count_reset(DMA_CH2);			//----��������
  if(gpio_get(PORTA,26) == 0)
    motor_r.pulse = -motor_r.pulse;
}

/************************************************************
-------------------------------------------------------------
�ٶȿ��ƣd��������100ms  PI����
������λ����(mm/s)
 
-------------------------------------------------------------
*************************************************************/
void speed_control(void)
{
  float SpeedAverage;
  static float SpeedErrorLast;
  
  CarSpeedAverageOld = car.SpeedAverage;
  
  SpeedAverage = (motor_l.pulse + motor_r.pulse)/2.0;  //ƽ��������
  
  car.SpeedAverage = SpeedAverage*2;//5Ϊ���ӹ���1mm����5�����壬ʱ��Ϊ0.1s����λ��mm/s)
  
  //if(car.SpeedAverage-CarSpeedAverageOld>1500 || car.SpeedAverage-CarSpeedAverageOld<-1500)///////��ȥͻ��
    //car.SpeedAverage = CarSpeedAverageOld;
  
  car.SpeedError = car.SpeedAverage - car.SpeedBase; //ʵ��ֵ - ����ֵ

  car.Distance = car.Distance + car.SpeedError*0.1;//Distance���Ǿ��룬�����ٶ����Ļ��֡�

  car.Acceleration = car.SpeedError - SpeedErrorLast;//Acceleration���Ǽ��ٶȣ������ٶ�����΢�֡� 

  SpeedErrorLast = car.SpeedError;

  if(car.Distance>=200)  //���ֱ�������
  {
    car.Distance=200;
  }
  else if(car.Distance<=-200)
  {
    car.Distance=-200;
  }  

  motor_l.speed_pwm_old = motor_l.speed_pwm_new; 
  motor_r.speed_pwm_old = motor_r.speed_pwm_new;

  motor_l.speed_pwm_new = car.SpeedError*sp_p + car.Distance*sp_i;  //Ҫ�����������ٶ�������ʱ��ʵ��ֵ-����ֵ
  motor_r.speed_pwm_new = car.SpeedError*sp_p + car.Distance*sp_i; 
}

/*********************************************************
----------------------------------------------------------
�������ƣSspeed_control_output(void)
�������ܣ��ú������ٶ�����仯��ƽ����20������������  
          ���� SpeedControl����ÿ100MSһ�Σ��ж�5MS  
          �ֳ�20�����Ե���
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