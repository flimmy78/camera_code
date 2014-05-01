/*
  板级使用的函数，便于理解和配置
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

int right_dead = 20;  //电机死区
int left_dead  = 20;

/*******************************************
 *
 * 按键、拨码开关相关函数
 *
********************************************/

//拨码开关8位数据获取
uint8_t sw8_data_get(void)
{
  uint32_t data;
  data = SW8_DATA_IN;
  //宏选择拨码开关拨上on是1还是0
  //拨码开关拨上是1
#define ON_MEAN_ONE
#ifdef ON_MEAN_ONE
  return((uint8_t)(((~data)&0x0000ff00)>>SW8_MBITS));
#endif
  
  //拨码开关拨上是0
#ifdef ON_MEAN_ZERO
  return((uint8_t)(((data)&0x0000ff00)>>SW8_MBITS));
#endif
  
  //取消宏定义
#ifdef ON_MEAN_ONE
#undef ON_MEAN_ONE
#endif
#ifdef ON_MEAN_ZERO
#undef ON_MEAN_ZERO
#endif
}


 //无限等待按键1按下
void wait_key1(void)
{
  while(KEY1_IN == 1 );
}


//无限等待按键2按下
void wait_key2(void)
{
  while(KEY2_IN == 1 );
}

//无限等待按键3按下
void wait_key3(void)
{
  while(KEY3_IN == 1 );
}


//按键任务
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
 * 角度获取函数配置
 *
********************************************/
//陀螺仪数据获取,返回角速度
u16 GYRO_ZERO;
float gyro_data_get(void)
{
    u16 gyro_ad;
    float angular;
    
    gyro_ad = ad_ave(ADC1,SE10,ADC_16bit,20);     //120为补偿值
    angular = (gyro_ad - GYRO_ZERO - 30) / GYRO_SCALE;
    
    return  (angular);

}

//加速度计数据获取，返回角度
float acc_data_get(void)
{
    float z_ad = ACC_ZOUT_0 + 75 - ad_ave(ADC1,SE14,ADC_16bit,10);
    float x_ad = ad_ave(ADC0,SE14,ADC_16bit,10) - ACC_XOUT_0 - 45;
    
    return(57.296*atan2(z_ad,x_ad));
   
}


//****陀螺仪和加速度计初始化
void angle_get_init()
{

  adc_init(ADC1,SE14); 	    //ZOUT初始化
  adc_init(ADC0,SE14);      //XOUT初始化
  adc_init(ADC1,SE10);      //AR1初始化
//  adc_init(ADC0,AD8);     //AR2初始化
//  adc_init(ADC0,SE15);    //YOUT初始化,Y轴是坏的
  
}



/*************电机驱动初始化*****************/
void motor_init(void)
{
  FTM_PWM_init(RIGHT_A_FTM,RIGHT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(RIGHT_B_FTM,RIGHT_B_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_A_FTM,LEFT_A_CH,MOTOR_FRE,INIT_DUTY);
  FTM_PWM_init(LEFT_B_FTM,LEFT_B_CH,MOTOR_FRE,INIT_DUTY);
}



/*************右电机速度控制，包含方向*****************/

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


void right_run_s(int32_t speed)       //speed的符号体现方向
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

void left_run_s(int32_t speed)   //speed的符号体现方向
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
*   编码器初始化
*************************************/
void speed_init()
{
    FTM2_QUAD_init();       //右轮测速
    FTM1_QUAD_init();       //左轮测速
}

s16 right_speed()
{
    s16 count = -FTM2_CNT;
    FTM2_CNT = 0;
    
    return count;
}

s16 left_speed()
{
    s16 count = FTM1_CNT;
    FTM1_CNT = 0;
    
    return count;
}

//s16 pulse_cnt_left(void)
//{
//  s16 cnt;
//  cnt = FTM1_CNT;
//  FTM1_CNT = 0;
//  return cnt;
//}
//
//s16 pulse_cnt_right(void)
//{
//  s16 cnt;
//  cnt = FTM2_CNT;
//  FTM2_CNT = 0;
//  return cnt;
//}

/*
 *************************************************************************************************************
*
*  函数名   : sent_to_computer
*  功能    : 向Labvew上位机发送数据用于观察滤波效果。
*  参数   ：data1,data2,data3.数据类型均为 uint16_t，三个数据按顺发送，并按顺序在上位机上显示
*  注意  ：在上位机显示时，第一数据位陀螺仪的输出数据，第二个数据为加速度计的输出数据，第三个数据为
*         联合滤波后的输出，虽然数据发送顺序没有任何关系，但为了便于观察数据请在按上位机的显示的顺序发送数据。
*       
*************************************************************************************************************
*/

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3)
{
  
     uart_putchar(UART0,0xff);                //  发送第一个字节
     uart_putchar(UART0,0xff);                //  发送第二个个字节
     uart_putchar(UART0,(char)(data1 >> 8));       //  发送第一个数据的高八位
     uart_putchar(UART0,(char)(data1 & 0x00ff));   //  发送第一个数据的低第八位
     uart_putchar(UART0,(char)(data2 >> 8));        //  发送第二个数据的高八位
     uart_putchar(UART0,(char)(data2 & 0x00ff));    //  发送第二个数据的低第八位
     uart_putchar(UART0,(char)(data3 >> 8));        //  发送第三个数据的高八位
     uart_putchar(UART0,(char)(data3 & 0x00ff));    //  发送第三个数据的低第八位
     
}



//float PID_implement(float data_in,pid_struct pid)
//{
//  pid->SetDate = data_in;
//  pid->err =pid->SetData-pid->ActualData;
//  pid->intergal +=pid->err;
//  pid->Implement
//}


//extern cars_status car;
//使用互补滤波进行直立控制。
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



//使用卡尔曼滤波直立控制。
void blance_kalman_filter(cars_status car)
{
   
    Kalman_filter(car);

    car->blance_duty = (car->angle - car->angle_set)*car->angle_p + (car->gyro - car->gyro_set)*car->gyro_d;
  
}
                          
                          
                          
                          
/*********************************************************************************************
*       速度控制函数，根据设定速度计算占空比。
*
*
**********************************************************************************************/
void speed_control(cars_status car)
{
  float speed_err;
  static float speed_integral;
  speed_err   = car->speed_set - ((float)(car->speed_left_m)  + (float)(car->speed_right_m))/2.0;
  speed_integral  += speed_err;
  if(speed_integral >= 200)                     //防止出现积分饱和，参数设置有待检验。
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

//这个函数可以省掉，在直立控制是可直接实现，执行速率应该更高。100ms可以算出一个value，每5ms平滑输出速度控制占空比。
void speed_control_output(cars_status car) 
{    
  float value;  
  value=(car->speed_duty_new - car->speed_duty_old)/20.0;
  car->speed_duty += value;         
}






/*********************************************************************************************
*       电机控制。
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

//增量式速度pid控制。
//void speed_pid(cars_status car)
//{
//   static float err[3];
//   err[0]           =  err[1];
//   err[1]           =  err[2];
//   err[2]           =  car->speed_set - (car->speed_left_m + car->speed_right_m)/2.0;
//   car->speed_duty += (car->speed_p)*((err[2] - err[1]) + (car->speed_i)*err[2] + (car->speed_d)*(err[2] - 2 * err[1] + err[0]));   
//}


//打印车体运行参数。
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
//  printf("speed_d:%f\n",car->speed_d);
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
/***********************方向控制********************************/
void direction_control(cars_status car)
{
  float direction_diff;
         direction_diff = ( car->direction_err_new - car->direction_err_old);
        if(direction_diff > 30)
           direction_diff = 30;
        if(direction_diff < -30)
           direction_diff = -30;
        car->direction = (((car->direction_p) *(car->direction_err_new)  + (car->direction_d) * direction_diff) - car->direction_left_duty ) / 4.0;  
}

void direction_control_output(cars_status car)
{
    car->direction_left_duty   = car->direction_left_duty + car->direction;
    car->direction_right_duty  = car->direction_left_duty;
}







/******************发送滤波值给虚拟示波器visual scope*******************/
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
  uart_putchar(UART4,databuf[i]); 

}

/**************摄像头延时函数**************/
void camera_wait()
{
    u8 i,j;
    
    for(i=0;i<25;i++)
        for(j=0;j<25;j++)
            asm("nop");
}