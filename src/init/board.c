/*
  �弶ʹ�õĺ�����������������
*/


#include "common.h"
#include "stdint.h"
#include "board_config.h"
#include "adc.h"
//#include "FTM.h"
#include "board.h"

direction dir_flag;

/*******************************************
 *
 * �ǶȻ�ȡ��������
 *
********************************************/

//���������ݻ�ȡ,��ȡADֵ
uint16_t gyro_data_get(void)
{
  return(ad_once(ADC1,AD9,ADC_16bit));
}


//���ٶȼ����ݻ�ȡ����ȡADֵ
uint16_t acc_data_get(void)
{
  return(ad_once(ADC0,AD8,ADC_16bit));
}


//**********���ݻ�ȡ��ʼ��**********//

void	angle_get_init()
{
	adc_init(ADC0,AD8);		//���ٶȼ����ݻ�ȡ��ʼ��
	adc_init(ADC1,AD9);		//���������ݻ�ȡ��ʼ��
}

//**********���ٶȼƽǶȻ�ȡ************
float	acc_angle_get()
{
	u16	 data;
	data = ad_once(ADC0,AD8,ADC_16bit);
	
//	return( (ACC_ZERO - data)/ACC_GRA );   //sin = (data - ACC_ZERO)/ACC_GRA , ���������
	return( 57.3 * (ACC_ZERO - data)/ACC_GRA ); 	//  180/pi = 57.3 , �Ƕ������
}




/*************�����ٶȿ��ƣ���������*****************/

void right_run(uint32_t speed,direction d)
{
  if(d == ahead)
  {
    FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,0);
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,speed);
  }
  else
  {
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,0);
    FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,speed);
  }
}







/*************�����ٶȿ��ƣ���������*****************/

void left_run(uint32_t speed,direction d)
{
  if(d == ahead)
  {
    FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,0);
    FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,speed);
  }
  else
  {
    FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,0);
    FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,speed);
  }
}