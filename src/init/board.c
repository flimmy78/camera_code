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
const int right_dead = 55;  //�������
const int left_dead  = 50;

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

//���������ݻ�ȡ,��ȡADֵ��������ٶȡ�
float gyro_data_get(void)
{
  
  //return(((GYRO_ZERO - ad_once(ADC0,SE16,ADC_16bit)) / GYRO_SCALE));
  return(((GYRO_ZERO - (ad_ave(ADC0,SE16,ADC_16bit,20))) / GYRO_SCALE));
  
}


//���ٶȼ����ݻ�ȡ����ȡADֵ��������ǡ�
float acc_data_get(void)
{
   u16 acc_ad;
   
   acc_ad = ad_ave(ADC1,SE16,ADC_16bit,20);
   if(acc_ad<=13769)   //10301
       acc_ad = 13769;
   else if(acc_ad>= 43619)  //41870
       acc_ad =  43619; 
   return   ( 57.296*asin((acc_ad-ACC_ZERO)/ACC_GRA) );
   
}
//****�����Ǻͼ��ٶȼƳ�ʼ��
void angle_get_init()
{

  adc_init(ADC1,SE16); 	//���ٶȼƵ�ADͨ����ʼ��

  adc_init(ADC0,SE16);  //������ADͨ����ʼ��
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
    FTM1_QUAD_init();
    FTM2_QUAD_init();
    
   //pit_init_ms(PIT0,SPEED_SAMPLING_TIME);
  // pit_init_ms(PIT1,1);
}

float left_speed()
{
    s16 temp;
    temp = FTM2_CNT;
    FTM2_CNT=0;
    
    return((temp*TRANSFER)/(SPEED_SAMPLING_TIME*0.001));
}

float right_speed()
{
    s16 temp;
    temp = FTM1_CNT;
    FTM1_CNT = 0;
    
    return((temp*TRANSFER)/(SPEED_SAMPLING_TIME*0.001));
}

s16 pulse_cnt_left(void)
{
  s16 cnt;
  cnt = FTM1_CNT;
  FTM1_CNT = 0;
  return cnt;
}

s16 pulse_cnt_right(void)
{
  s16 cnt;
  cnt = FTM2_CNT;
  FTM2_CNT = 0;
  return cnt;
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
//ʹ�û����˲�����ֱ�����ơ�
void blance_comp_filter(float tg,float dt,cars_status car)
{
  comp_filter(tg, dt,car);
  car->blance_duty = (car->angle - car->angle_set)*car->angle_p + (car->gyro_m - car->gyro_set)*car->gyro_d ;
 // printf("%f\t%f\n",car->angle_m,car->angle);
  
}

void blance_comp_filter_pid(float tg,float dt,cars_status car)
{
  static float err[3];
  comp_filter(tg, dt,car);
  err[0]            =  err[1];
  err[1]            =  err[2];
  err[2]            =  car->angle_set -  car->angle;
  car->blance_duty += car->angle_p*((err[2] - err[1]) + 0 *err[2] + car->gyro_d*(err[2] - 2 * err[1] + err[0]));
}



//ʹ�ÿ������˲�ֱ�����ơ�
void blance_kalman_filter(cars_status car)
{
   
    Kalman_filter(car);
    car->blance_duty = (car->angle - car->angle_set)*car->angle_p + (car->gyro - car->gyro_set)*car->gyro_d;
  
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
  speed_err        = car->speed_set - ((float)(car->speed_left_m)  + (float)(car->speed_right_m))/2.0;
  speed_integral  += speed_err;
  if(speed_integral >= 200)                     //��ֹ���ֻ��ֱ��ͣ����������д����顣
  {
      speed_integral = 200;
  }
  if(speed_integral <=-200)
  {
    speed_integral = -200;
  }
  car->speed_duty_old = car->speed_duty_new;
  car->speed_duty_new = (car->speed_p)*speed_err + (car->speed_i)*speed_integral;
 // printf("speed_duty:%f\n",car->speed_duty);
  
}

//�����������ʡ������ֱ�������ǿ�ֱ��ʵ�֣�ִ������Ӧ�ø��ߡ�100ms�������һ��value��ÿ5msƽ������ٶȿ���ռ�ձȡ�
void speed_control_output(cars_status car) 
{    
  float value;  
  value=(car->speed_duty_new - car->speed_duty_old)/20.0;
  car->speed_duty += value;         
}






/*********************************************************************************************
*       ������ơ�
*
*
**********************************************************************************************/


void motor_set(cars_status car)
{
 
  if( (car->left_duty>0) && (car->left_duty + left_dead >=1000))
        car->left_duty = 1000 -left_dead ;
  if( (car->left_duty<0) && (car->left_duty - left_dead< -1000))
        car->left_duty = -1000 + left_dead;
  if( (car->right_duty>0) && (car->right_duty + right_dead >=1000))
        car->right_duty = 1000 - right_dead;
  if( (car->right_duty<0) && (car->right_duty - right_dead<= -1000))
        car->right_duty = -1000 + right_dead;
  left_run_s((int32_t)(car->left_duty));
  right_run_s((int32_t)(car->right_duty));
  
}

//����ʽ�ٶ�pid���ơ�
void speed_pid(cars_status car)
{
   static float err[3];
   err[0]           =  err[1];
   err[1]           =  err[2];
   err[2]           =  car->speed_set - (car->speed_left_m + car->speed_right_m)/2.0;
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
/***********************�������********************************/
void direction_control(cars_status car)
{
  float direction_diff = ( car->direction_err_new - car->direction_err_old);
        car->direction = ( car->direction_left_duty- ((car->direction_d) * (car->direction_d) * direction_diff)) / 4.0;  
}

void direction_control_output(cars_status car)
{
  car->direction_left_duty = car->direction_right_duty  = car->direction_left_duty + car->direction;
}







/******************�����˲�ֵ������ʾ����visual scope*******************/
float OutData[4];
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void send_toscope(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int16)OutData[i];
    temp1[i] = (uint16)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (int8)(temp1[i]%256);
    databuf[i*2+1] = (int8)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
  uart_putchar(UART0,databuf[i]); 

}

/**************����ͷ��ʱ����**************/
void camera_wait()
{
    u8 i,j;
    
    for(i=0;i<25;i++)
        for(j=0;j<25;j++)
            asm("nop");
}