#include "include.h"   

  
float Q1_1 = 0.00001;   //���ٶ�������(��˹����) 
float Q2_2 = 0.00001;   //������������(��˹����)
float R = 1;            //���ƿ��������棬RԽС���Ƕȸ���Խ�죬���˲�Խ��

//float wending =0;     //�˲��ȶ����        
 
/***********************************************************
------------------------------------------------------------
�������˲���
����ADֵ����Ϊ�������������ں�

**����ǰ�Ѿ�ʹ��jt_average_filter();�����ɼ�����ƽ���˲�
------------------------------------------------------------
************************************************************/
void kalman_filter(void)    //��ʱ630us
{ 
  //static float acc_y=0,acc_z=0;
  static float Xest1_1,Xest2_1 = -67,Pest1_1=0.001,Pest1_2=1,Pest2_1,Pest2_2,K1_1,K2_1;
  static float Xsta1_1=0;
  static float Xsta2_1=0;
  static float Psta1_1=1;
  static float Psta1_2=1;
  static float Psta2_1=1;
  static float Psta2_2=1;
  
  /*acc_y=ad_accy-accy_offset; 
  acc_z=ad_accz-accz_offset
  acc_th=atan2(acc_y,acc_z);
  acc_th=acc_th*57.296; */
  
  acc_th =  ( (ad_accz - accz_offset)*(ad_basic_value/4096.0) )/790;    //������787mv/g  0.8057
  if(acc_th>1.0)
    acc_th=1.0;
  if(acc_th<-1.0)
    acc_th=-1.0;
  acc_th =  asin(acc_th)*57.296;  
  gyr_xw =  (ad_gyrx - gyrx_offset)*(ad_basic_value/4096.0/3.417);//  =0.8057/0.67/5.1=0.2358
  
  /*�����˲�����
  gyr_xw_integral += gyr_xw/200; //200HZ����ʱ��(5ms)
  if(gyr_xw_integral>200 || gyr_xw_integral<-200)
    gyr_xw_integral=0;*/
  
  
  
  
  Xest1_1=(A1_1*Xsta1_1+A1_2*Xsta2_1) + (gyr_xw*B1_1);   //Ԥ��ֵ
  Xest2_1=(A2_1*Xsta1_1+A2_2*Xsta2_1) + (gyr_xw*B2_1);

  Pest1_1=(A1_1*Psta1_1+A1_2*Psta2_1)*A1_1 + (A1_1*Psta1_2+A1_2*Psta2_2)*A1_2 + Q1_1;//Ԥ��ֵЭ����
  Pest1_2=(A1_1*Psta1_1+A1_2*Psta2_1)*A2_1 + (A1_1*Psta1_2+A1_2*Psta2_2)*A2_2 + Q1_2;
  Pest2_1=(A2_1*Psta1_1+A2_2*Psta2_1)*A1_1 + (A2_1*Psta1_2+A2_2*Psta2_2)*A1_2 + Q2_1;
  Pest2_2=(A2_1*Psta1_1+A2_2*Psta2_1)*A2_1 + (A2_1*Psta1_2+A2_2*Psta2_2)*A2_2 + Q2_2;

  K1_1=(Pest1_1*C1_1+Pest1_2*C1_2)/((C1_1*Pest1_1+C1_2*Pest2_1)*C1_1+(C1_1*Pest1_2+C1_2*Pest2_2)*C1_2+R); //����
  K2_1=(Pest2_1*C1_1+Pest2_2*C1_2)/((C1_1*Pest1_1+C1_2*Pest2_1)*C1_1+(C1_1*Pest1_2+C1_2*Pest2_2)*C1_2+R);

  Xsta1_1=Xest1_1+K1_1*(acc_th-(C1_1*Xest1_1+C1_2*Xest2_1));   //ʵ��ֵ
  Xsta2_1=Xest2_1+K2_1*(acc_th-(C1_1*Xest1_1+C1_2*Xest2_1));   

  Psta1_1=(I1_1-K1_1*C1_1)*Pest1_1+(I1_2-K1_1*C1_2)*Pest2_1;   //ʵ��ֵЭ����
  Psta1_2=(I1_1-K1_1*C1_1)*Pest1_2+(I1_2-K1_1*C1_2)*Pest2_2;
  Psta2_1=(I2_1-K2_1*C1_1)*Pest1_1+(I2_2-K2_1*C1_2)*Pest2_1;
  Psta2_2=(I2_1-K2_1*C1_1)*Pest1_2+(I2_2-K2_1*C1_2)*Pest2_2;

  car.th=Xsta1_1;     //�Ƕ��˲�ֵ
  car.w=gyr_xw-Xsta2_1;//���ٶ��˲�ֵ
                                                    
  //temp = fabs(Xsta2_1/Xest2_1 - 1);
  //wending = (temp + wending)/2;//�˲��ȶ����
}                            

/***********************************************************
------------------------------------------------------------
�ȴ��������˲��ȶ�
------------------------------------------------------------
************************************************************/
void wait_kalman(void)   
{
  while(kalman_count<800);
  Q1_1 = 0.00001;   
  Q2_2 = 0.000001; 
  R = 6;        
}

/*################��ȡ���ٶ���������ƫֵ###############*/
void get_gyr_offset(void)  //�ɼ�250����ƽ��
{
  gyrx_offset= ad_ave(ADC1,SE12,ADC_12bit,250);
  //accy_offset= ad_ave(ADC1,SE8,ADC_12bit,250);
  accz_offset= ad_ave(ADC1,SE10,ADC_12bit,250);
}

/***********************************************************
------------------------------------------------------------
���ٶ�������ADֵ��20�μ�Ȩƽ���˲�����
ʵ�ּ��ٶ������ǵ�AD���ݲɼ���Σ��ҶԶ�βɼ������ݽ������˲���
�˲����ADֵ�����ad_value[i]�С�

**����ǰAD�Ѿ���ʼ��
------------------------------------------------------------
************************************************************/
void jt_average_filter(void) 
{
  ad_gyrx= ad_ave(ADC1,SE12,ADC_12bit,15);
  //ad_accy= ad_ave(ADC1,SE8,ADC_12bit,10);
  ad_accz= ad_ave(ADC1,SE10,ADC_12bit,15);
}

/**************************************************************************
--------------------------------------------------------------------------
* �������ƣ� void motor_set(unsigned char motor_choose, signed int PWM)
* ���������� �������PWM���������δ��������
* ��������� motor_choose:���ѡ��(motor_left��motor_right)
             pwm ������pwmֵ
* �� �� ֵ�� ��     
--------------------------------------------------------------------------
**************************************************************************/
void motor_set(unsigned char motor_choose, s16  pwm)
{
  if(pwm > 599)
  {
    pwm= 599;
  }
  if(pwm < -599)
  {
    pwm = -599;  
  }
  
  if(motor_choose == motor_left)//����
  {
    if(pwm > 0)
    { 
      FTM_PWM_Duty(FTM0,CH3,pwm);
      FTM_PWM_Duty(FTM0,CH4,0);
    }
    else
    {
      FTM_PWM_Duty(FTM0,CH3,0);
      FTM_PWM_Duty(FTM0,CH4,0-pwm); 
    }
  }
  else//�ҵ��
  {
    if(pwm > 0)
    { 
      FTM_PWM_Duty(FTM0,CH1,pwm);
      FTM_PWM_Duty(FTM0,CH2,0);
    }
    else
    {
      FTM_PWM_Duty(FTM0,CH1,0);
      FTM_PWM_Duty(FTM0,CH2,0-pwm); 
    }
  }
}  

/************************************************************************
-------------------------------------------------------------------------
���״̬������������Ѽ�������
------------------------------------------------------------------------- 
*************************************************************************/
void motor_control_output(void)
{
  if(motor_l.pwm>0) //����
  {
     motor_l.pwm = motor_l.pwm + dead_value_l_forward;
  }
  else
  {
     motor_l.pwm = motor_l.pwm - dead_value_l_back;
  }

  if(motor_r.pwm>0)//�ҵ��
  {
     motor_r.pwm = motor_r.pwm + dead_value_r_forward;
  }
  else
  {
     motor_r.pwm = motor_r.pwm - dead_value_r_back;
  }
  if(car_states == 0)
  {
    FTM_PWM_Duty(FTM0,CH1,0);
    FTM_PWM_Duty(FTM0,CH2,0);
    FTM_PWM_Duty(FTM0,CH3,0);
    FTM_PWM_Duty(FTM0,CH4,0);
  }
  else
  {
    motor_set(motor_left, motor_l.pwm);
    motor_set(motor_right,motor_r.pwm); 
  }
     
}    

/***********************************************************************
------------------------------------------------------------------------
ֱ�����ƣPPD����
ע��w��th�ķ�������
------------------------------------------------------------------------
************************************************************************/
void stand_control(void)
{
  motor_l.stand_pwm = sd_p*(car.th-g_th) + sd_d*car.w;
  motor_r.stand_pwm = motor_l.stand_pwm ;  
}
