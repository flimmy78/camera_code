/*
  板级使用的函数，便于理解和配置
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
 * 角度获取函数配置
 *
********************************************/

//陀螺仪数据获取,获取AD值
uint16_t gyro_data_get(void)
{
  return(ad_once(ADC1,AD9,ADC_16bit));
}


//加速度计数据获取，获取AD值
uint16_t acc_data_get(void)
{
  return(ad_once(ADC0,AD8,ADC_16bit));
}


//**********数据获取初始化**********//

void	angle_get_init()
{
	adc_init(ADC0,AD8);		//加速度计数据获取初始化
	adc_init(ADC1,AD9);		//陀螺仪数据获取初始化
}

//**********加速度计角度获取************
float	acc_angle_get()
{
	u16	 data;
	data = ad_once(ADC0,AD8,ADC_16bit);
	
//	return( (ACC_ZERO - data)/ACC_GRA );   //sin = (data - ACC_ZERO)/ACC_GRA , 弧度制输出
	return( 57.3 * (ACC_ZERO - data)/ACC_GRA ); 	//  180/pi = 57.3 , 角度制输出
}




/*************左电机速度控制，包含方向*****************/

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







/*************左电机速度控制，包含方向*****************/

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