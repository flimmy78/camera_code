
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************配置区**>>***********/
//右电机前进的FTM模块
#define RIGHT_A_FTM FTM1
#define RIGHT_A_CH  CH0

//右电机后退的FTM模块
#define RIGHT_B_FTM FTM1
#define RIGHT_B_CH  CH1

//左电机前进的FTM模块
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH4

//左电机后退的FTM模块
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH3

//电机驱动频率
#define MOTOR_FRE 15000

//电机驱动初始化的占空比，【强烈要求为【0】】
#define INIT_DUTY (0)

/********加速度计标准值配置*******/
//加速度计 		  800mV/g ,对应为AD值 15887/g , 1605 * m/s^2

#define	ACC_ZERO	26420.0	//0度对应的AD值
#define	ACC_90		11200.0	//实际为加速度计-90度的值
#define	ACC_NEG_90	42650.0	//实际为加速度计90度的值

#define	ACC_GRA		15887.0   

/********陀螺仪标准值配置********/
#define	GYRO_ZERO	27583.0	//陀螺仪零值
#define	GYRO_SCALE	119.7    // 13.3*9/deg./sec
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


void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

float str2num(char * str,u8 n);
void blance_comp_filter(float tg,float dt,cars_status car);
void speed_control(car_status car)

#endif