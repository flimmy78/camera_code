
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************配置区**>>***********/

/************电机配置**************/
//右电机前进的FTM模块
#define RIGHT_A_FTM FTM0
#define RIGHT_A_CH  CH2

//右电机后退的FTM模块
#define RIGHT_B_FTM FTM0
#define RIGHT_B_CH  CH0

//左电机前进的FTM模块
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH3

//左电机后退的FTM模块
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH1

//电机驱动频率
#define MOTOR_FRE 15000

//电机驱动初始化的占空比，【强烈要求为【0】】
#define INIT_DUTY (0)


//拨码开关【输入】寄存器宏和【移位】位数的宏
#define SW8_DATA_IN  GPIOC_PDIR
#define SW8_MBITS    8

//按键相关的宏
#define KEY1_IN       PTE1_IN
#define KEY2_IN       PTE3_IN
#define KEY3_IN       PTE5_IN

/*****************编码器配置*********************/
#define     TRANSFER        0.000383    //传送比,由脉冲数对应速度，单位是m，(pi*r齿*R轮)/(100*R齿),(3.14 * 40 * 0.032)/(100 * 105),轮胎半径0.032m
#define     SPEED_PER       200.0       //编码器每转脉冲数。
#define     SPEED_SAMPLING_TIME     100  //采样时间  20ms

//*****算的是轮子的线速度，单位m/s

void    speed_init();
float   left_speed_get();
float   right_speed_get();

/********加速度计标准值配置*******/
//加速度计 		  800mV/g ,对应为AD值 15887/g , 1605 * m/s^2

#define	ACC_ZERO	28694.0	//0度对应的AD值 27790

#define	ACC_GRA		14925.0  //15887.0      -12865

/********陀螺仪标准值配置********/
#define	GYRO_ZERO	27099.0	//陀螺仪零值
#define	GYRO_SCALE	65.17  // 13.3*4.9/deg.c  82.46

/*******************<<*********************/




/*************数据结构定义区**********>>**/
typedef enum 
{
   ahead=1,
   back=0
}direction;




typedef struct {
  float SetData;      //目标数据
  float ActualData;   // 实测数据
  float err;         //偏差
  float err_last;    //上一次的偏差
  float Kp,Ki,Kd;    //比例、积分、微分系数
  float Implement;     //执行器的值
  float intergal;  //积分值
}* pid_struct;


/*******************函数声明********************/

//******陀螺仪和加速度计函数声明*****//

float gyro_data_get(void);	//陀螺仪数据获取

float acc_data_get(void);	//加速度计数据获取

void angle_get_init();			//陀螺仪和加速度计数据获取初始化

//******电机函数声明********//
void motor_init(void);

void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void right_run_s(int32_t speed);

void left_run_s(int32_t speed);

uint8_t sw8_data_get(void);

void wait_key1(void);

void wait_key2(void);

void wait_key3(void);


void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);


extern float OutData[4];
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void send_toscope(void);


float str2num(char * str,u8 n);
void blance_comp_filter(float tg,float dt,cars_status car);
void blance_comp_filter_pid(float tg,float dt,cars_status car);
void blance_kalman_filter(cars_status car);
void speed_control(cars_status car);
//设置电机。
void motor_set(cars_status car);

void speed_pid(cars_status car);

void print(cars_status car);

//编码器正交解码脉冲计数值返回并清除 有符号16位
s16 pulse_cnt_left(void);

s16 pulse_cnt_right(void);

void speed_control_output(cars_status car) ; //电机平滑输出。

float left_speed();
float right_speed();


/************摄像头配置************/
#define    ROW_START    120
#define    ROW_END      159
#define    ROW          40      //ROW_END-ROW_START
#define    COL          200

void camera_wait();


#endif