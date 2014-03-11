/*
  �弶ʹ�õĺ�����������������
*/


#include "common.h"
#include "stdint.h"
#include "board_config.h"
#include "adc.h"
//#include "FTM.h"
#include "uart.h"
#include "board.h"

direction dir_flag;

const int dead_right = 9;  //�������
const int dead_left  = 9;

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

//****�����Ǻͼ��ٶȼƳ�ʼ��
void angle_get_init()
{

  adc_init(ADC0,AD8); 	//���ٶȼƵ�ADͨ����ʼ��

  adc_init(ADC1,AD9);  //������ADͨ����ʼ��
}

//*****���ٶȼƽǶȻ�ȡ*****
//float	acc_angle_get()
//{
//	
////	return( (ACC_ZERO - ad_once(ADC0,AD8,ADC_16bit))/ACC_GRA );   //sin = (ad_once(ADC1,AD9,ADC_16bit) - ACC_ZERO)/ACC_GRA , ���������
//	return( arcsin[
//               (u8) 
//                 ( 
//                    (100*
//                      (
//                       ACC_ZERO - ad_once(ADC0,AD8,ADC_16bit)
//                         )/ACC_GRA) + 100
//                  )
//                 ]
//               ); 	//  �Ƕ������
//}

//*****�����ǽ��ٶȻ�ȡ*****
float  gyro_angular_get()
{
	
	return((GYRO_ZERO - ad_once(ADC1,AD9,ADC_16bit))/GYRO_SCALE);		//(ad_once(ADC1,AD9,ADC_16bit) - GRYO_ZERO)/GYRO_SCALE,��λdeg/sec,�Ƕ���
}



/*************���������ʼ��*****************/
void motor_init(void)
{
  FTM_PWM_init(RIGHT_A_FTM,RIGHT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(RIGHT_B_FTM,RIGHT_B_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_A_FTM,LEFT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_B_FTM,LEFT_B_CH,MOTOR_FRE,INIT_DUTY);
}



/*************�����ٶȿ��ƣ���������*****************/

void right_run(uint32_t speed,direction d)
{
  if(d == ahead)
  {
    FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,0);
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,speed+dead_right);
  }
  else
  {
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,0);
    FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,speed+dead_right);
  }
}


void right_run_s(int32_t speed)       //speed�ķ������ַ���
{
  char dir;
  dir = (speed > 0) ? 1 :((speed<0)? -1 : 0);
  switch(dir)
  {
    case 1:
      {
        FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,0);
        FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,speed+dead_right);
      }break;
    case -1:
    {
      FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,0);
      FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,-speed+dead_right);
    }break;
    case 0:
      {
        FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,0);
        FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,0);
      }break;
  }
}










/*************�����ٶȿ��ƣ���������*****************/

void left_run(uint32_t speed,direction d)
{
  if(d == ahead)
  {
    FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,0);
    FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,speed+dead_left);
  }
  else
  {
    FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,0);
    FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,speed+dead_left);
  }
}



void left_run_s(int32_t speed)   //speed�ķ������ַ���
{
  char dir;
  dir = (speed > 0) ? 1 :((speed<0)? -1 : 0);
  switch(dir)
  {
    case 1:
      {
        FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,0);
        FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,speed+dead_left);
      }break;
    case -1:
    {
      FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,0);
      FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,-speed+dead_left);
    }break;
    case 0:
      {
        FTM_PWM_Duty(LEFT_A_FTM,LEFT_A_CH,0);
        FTM_PWM_Duty(LEFT_B_FTM,LEFT_B_CH,0);
      }break;
    default:
      {
        
      }
  }
}





/*
 *************************************************************************************************************
*
*  ������   : sent_to_computer
*  ����    : ��Labvew��λ�������������ڹ۲��˲�Ч����
*  ����   ��data1,data2,data3.�������;�Ϊ uint16_t���������ݰ�˳���ͣ�����˳������λ������ʾ
*  ע��  ������λ����ʾʱ����һ����λ�����ǵ�������ݣ��ڶ�������Ϊ���ٶȼƵ�������ݣ�����������Ϊ
*         �����˲�����������Ȼ���ݷ���˳��û���κι�ϵ����Ϊ�˱��ڹ۲��������ڰ���λ������ʾ��˳�������ݡ�
*       
 *************************************************************************************************************
*/

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3)
{
  
     uart_putchar(UART0,0xff);                //  ���͵�һ���ֽ�
     uart_putchar(UART0,0xff);                //  ���͵ڶ������ֽ�
     uart_putchar(UART0,(char)(data1 >> 8));       //  ���͵�һ�����ݵĸ߰�λ
     uart_putchar(UART0,(char)(data1 & 0x00ff));   //  ���͵�һ�����ݵĵ͵ڰ�λ
     uart_putchar(UART0,(char)(data2 >> 8));        //  ���͵ڶ������ݵĸ߰�λ
     uart_putchar(UART0,(char)(data2 & 0x00ff));    //  ���͵ڶ������ݵĵ͵ڰ�λ
     uart_putchar(UART0,(char)(data3 >> 8));        //  ���͵��������ݵĸ߰�λ
     uart_putchar(UART0,(char)(data3 & 0x00ff));    //  ���͵��������ݵĵ͵ڰ�λ
}



//float PID_implement(float data_in,pid_struct pid)
//{
//  pid->SetDate = data_in;
//  pid->err =pid->SetData-pid->ActualData;
//  pid->intergal +=pid->err;
//  pid->Implement
//}

/***********************************************
*   ���ַ���ת��Ϊ������
*   strΪ�ַ����׵�ַ��nΪ�ַ�������
***********************************************/

float str2num(char * str,u8 n)
{
    u8 point = 0;       //��ѯС���㣬����С�����λ�ã�������Ϊ0
    int len = 0;               //�ַ�������
    float inc = 1;
    u8 p,l;
    float num;
    
    for(len=0; len<n; len++)
    {
        if(str[point] != '.')
           point ++;
    }
    
    if(point > len)      //��С����
    {
        for(len=n-1; len>=0; len--)       //���һ���ַ��ı��
        {
            num = num + ((u8)str[len]-48)*inc;
            
            inc = inc*10;
        }
    }
    else
    {
        len = n-1;              //���һ���ַ��ı��
        p = point;
        l = len;
        while(point < len)
        {
            point++;
            inc = inc*0.1;
            
            num = num + ((u8)str[point]-48)*inc;
        }
        point = p;
        len = l;
        inc = 1;
        while(point > 0)
        {
            point--;
            
            num = num+((u8)str[point]-48)*inc;
            
            inc = inc*10;
        }
    }
    
    return num;
}