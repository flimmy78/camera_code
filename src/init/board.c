/*
  �弶ʹ�õĺ�����������������
*/
#include "include.h"
#include "common.h"
#include "stdint.h"
#include "board_config.h"
#include "adc.h"
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
 * ���������뿪����غ���
 *
********************************************/

//���뿪��8λ���ݻ�ȡ
uint8_t sw8_data_get(void)
{
  uint32_t data;
  data = SW8_DATA_IN;
  //��ѡ���뿪�ز���on��1����0
  //���뿪�ز�����1
#define ON_MEAN_ONE
#ifdef ON_MEAN_ONE
  return((uint8_t)(((~data)&0x0000ff00)>>SW8_MBITS));
#endif
  
  //���뿪�ز�����0
#ifdef ON_MEAN_ZERO
  return((uint8_t)(((data)&0x0000ff00)>>SW8_MBITS));
#endif
  
  //ȡ���궨��
#ifdef ON_MEAN_ONE
#undef ON_MEAN_ONE
#endif
#ifdef ON_MEAN_ZERO
#undef ON_MEAN_ZERO
#endif
  

  
}


 //���޵ȴ�����1����
void wait_key1(void)
{
  while(KEY1_IN == 1 );
}


//���޵ȴ�����2����
void wait_key2(void)
{
  while(KEY2_IN == 1 );
}

//���޵ȴ�����3����
void wait_key3(void)
{
  while(KEY3_IN == 1 );
}


//��������
void key1_task(void (*task)())
{
  if(KEY1_IN == 0)
  {
    task();
  }
}

void key2_task(void (*task)())
{
  if(KEY2_IN == 0)
  {
    task();
  }
}

void key3_task(void (*task)())
{
  if(KEY3_IN == 0)
  {
    task();
  }
}


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
    speed = speed +left_dead;
  }
  else if(speed <0)
  {
    dir = back;
    speed = -speed + left_dead;
  }
  else
  {
    speed = 0;
  }
  left_run(speed,dir);
}


/*************************************
*   ��������ʼ��
*************************************/
void speed_init()
{
//    DMA_count_Init(DMA_CH4, PTA24, 10000, DMA_falling_up);
//    DMA_count_Init(DMA_CH5, PTA26, 10000, DMA_falling_up);
      DMA_count_Init(DMA_CH1, PTA28, 10000, DMA_falling_up);
//    DMA_count_Init(DMA_CH2, PTA29, 10000, DMA_falling_up);
    
    pit_init_ms(SPEED_PIT,SPEED_SAMPLING_TIME);
    pit_init_ms(PIT1,5);
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


//extern cars_status car;
void blance_comp_filter(float tg,float dt,cars_status car)
{
  
  float angle_m,gyro_m;
  angle_m = acc_data_get();
  gyro_m  = gyro_data_get();
  comp_filter(angle_m,gyro_m,tg, dt,car);
  car->blance_duty = (car->angle - car->angle_set)*car->angle_p + (gyro_m - car->gyro_set)*car->gyro_d ;
  //printf("blance_duty:%f\n",car->blance_duty);
   motor_set(car);
}


/*********************************************************************************************
*       �ٶȿ��ƺ����������趨�ٶȼ���ռ�ձȡ�
*
*
**********************************************************************************************/
void speed_control(cars_status car)
{
  float speed_err;
  static float speed_integral;
  speed_err        = car->speed_set - (car->speed_left_m +  car->speed_right_m)/2.0 ;
  speed_integral  += (car->speed_i)*speed_err;
  car->speed_duty  =  speed_integral + (car->speed_p)*speed_err;
 // printf("speed_duty:%f\n",car->speed_duty);
  
}

/*********************************************************************************************
*       ������ơ�
*
*
**********************************************************************************************/


void motor_set(cars_status car)
{
  car->left_duty  = (car->blance_duty) - (car->speed_duty) - (car->direction_left_duty);
  car->right_duty = (car->blance_duty) - (car->speed_duty) + (car->direction_right_duty);
 // printf("%f\t%f\n",car->left_duty,car->right_duty);
  if(((car->left_duty)>990)||((car->left_duty)<-990)||((car->right_duty)>990)||((car->right_duty)<-990))
    {
      (car->left_duty) = (car->right_duty) = 0;
    }
  left_run_s((int32_t)(car->left_duty));
  right_run_s((int32_t)(car->right_duty));
}





void speed_pid(cars_status car)
{
   static float err[3];
   err[0]           = err[1];
   err[1]           = err[2];
   err[2]           = car->speed_set - (car->speed_left_m + car->speed_right_m)/2.0;
   car->speed_duty += (car->speed_p)*((err[2] - err[1]) + (car->speed_i)*err[2] + (car->speed_d)*(err[2] - 2 * err[1] + err[0]));
   
}



//��ӡ�������в�����
void print(cars_status car)
{
  printf("left_duty:%f\n",car->left_duty);
  printf("right_duty:%f\n",car->right_duty);
  printf("angle_p:%f\n",car->angle_p);  
  printf("gyro_d:%f\n",car->gyro_d);
  printf("angle_set:%f\n",car->angle_set);
  printf("gyro_set:%f\n",car->gyro_set);
  printf("speed_p:%f\n",car->speed_p);
  printf("speed_i:%f\n",car->speed_i);
  printf("speed_d:%f\n",car->speed_d);
  printf("speed_set:%f\n",car->speed_set);
  printf("angle_m:%f\n",car->angle_m);
  printf(" gyro_m; :%f\n",car-> gyro_m);
  printf("angle:%f\n",car->angle);
  printf("angle:%f\n",car->angle);
  printf("blance_duty:%f\n",car->blance_duty);
  printf("speed_left_m:%f\n",car->speed_left_m);
  printf("speed_right_m:%f\n",car->speed_right_m);
  printf("speed_duty:%f\n",car->speed_duty);
  printf("direction_left_duty:%f\n",car->direction_left_duty);
  printf("direction_right_duty:%f\n",car->direction_right_duty);
      
 
}



