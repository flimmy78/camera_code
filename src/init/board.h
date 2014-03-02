
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
/******************配置区**>>***********/
//右电机前进的FTM模块
#define RIGHT_A_FTM FTM1
#define RIGHT_A_CH  CH1

//右电机后退的FTM模块
#define RIGHT_B_FTM FTM1
#define RIGHT_B_CH  CH0

//左电机前进的FTM模块
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH3

//左电机后退的FTM模块
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH4

//电机驱动频率
#define MOTOR_FRE 15000

//电机驱动初始化的占空比，【强烈要求为【0】】
#define INIT_DUTY (0)

/*******************<<*********************/

/*************数据结构定义区**********>>**/
typedef enum 
{
  ahead=1,
  back=0
}direction;



/**************函数声明*****************/
//陀螺仪数据获取
uint16_t gyro_data_get(void);

//加速度计数据获取
uint16_t acc_data_get(void);



void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

#endif