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
#include "Kalman.h"
direction dir_flag;


#define RIGHT_DEAD 10
#define LEFT_DEAD  10
const int right_dead = 10;  //�������
const int left_dead  = 10;


/*******************************************
 *
 * �ǶȻ�ȡ��������
 *
********************************************/

//���������ݻ�ȡ,��ȡADֵ
float gyro_data_get(void)
{
  
  return(-((GYRO_ZERO - ad_once(ADC1,AD9,ADC_16bit)) / GYRO_SCALE));
  
}


//���ٶȼ����ݻ�ȡ����ȡADֵ
float acc_data_get(void)
{
  
   return(180*(ACC_ZERO-ad_once(ADC0,AD8,ADC_16bit))/(3.1416*ACC_GRA));
  
}

//****�����Ǻͼ��ٶȼƳ�ʼ��
void angle_get_init()
{

  adc_init(ADC0,AD8); 	//���ٶȼƵ�ADͨ����ʼ��

  adc_init(ADC1,AD9);  //������ADͨ����ʼ��
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
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,speed);
  }
  else
  {
    FTM_PWM_Duty(RIGHT_A_FTM,RIGHT_A_CH,0);
    FTM_PWM_Duty(RIGHT_B_FTM,RIGHT_B_CH,speed);
  }
}


void right_run_s(int32_t speed)       //speed�ķ������ַ���
{
  direction dir;
  if(speed>0)
  {
    dir = ahead;
    speed = speed +right_dead;
  }
  else if(speed <0)
  {
    dir = back;
    speed = -speed + right_dead;
  }
  else
  {
    speed = 0;
  }
  right_run(speed,dir);
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



void left_run_s(int32_t speed)   //speed�ķ������ַ���
{
  direction dir;
  if(speed > 0)
  {
    dir = ahead;
    speed = speed +right_dead;
  }
  else if(speed <0)
  {
    dir = back;
    speed = -speed + right_dead;
  }
  else
  {
    speed = 0;
  }
  left_run(speed,dir);
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


//extern cars_status car;
void blance_comp_filter(float tg,float dt,cars_status car)
{
  
  car->angle_m = acc_data_get();
  car->gyro_m  = gyro_data_get();
  comp_filter(car->angle_m,car->gyro_m,tg, dt,car);
  (car->left_duty)=(car->right_duty)= (car->angle - car->angle_set)*car->angle_p + (car->gyro_m - car->gyro_set)*car->gyro_d;
  if(car->left_duty>990||car->right_duty<-990)
    {
        (car->left_duty)=(car->right_duty) = 0;
    }
  
   right_run_s((int32_t)car->right_duty);
   left_run_s((int32_t)car->left_duty);
}
/*********************************************************************************************
*       �ٶȿ��ƺ����������趨�ٶȶ�ռ�ձȽ������á�
*
*
**********************************************************************************************/
void speed_control(car_status car)
{
  float speed_err,speed_integral;
  speed_err        = car->speed_set - car->speed_m;
  speed_integral  += (car->speed_p)*speed_err;
  car->left_duty   = car->left_duty + speed_integral + (car->speed_d)*speed_err;
  car->right_duty  = car->right_duty + speed_integral + (car->speed_d)*speed_err;
  
  
}