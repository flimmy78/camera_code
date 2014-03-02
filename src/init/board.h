
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

/********加速度计标准值配置*******/
//正倾角时加速度计 770mV/g ,对应为AD值 15220/g , 1553 * m/s^2
//负倾角时加速度计 820mV/g ,对应为AD值 16230/g , 1656 * m/s^2
//加速度计 		  794mV/g ,对应为AD值 15725/g , 1605 * m/s^2

#define	ACC_ZERO	26420	//0度对应的AD值
#define	ACC_90		11200	//实际为加速度计-90度的值
#define	ACC_NEG_90	42650	//实际为加速度计90度的值

#define	ACC_GRA		15725   
/*******************<<*********************/

/*************数据结构定义区**********>>**/
typedef enum 
{
  ahead=1,
  back=0
}direction;



/*******************函数声明********************/

//******陀螺仪和加速度计函数声明*****//

uint16_t gyro_data_get(void);	//陀螺仪数据获取

uint16_t acc_data_get(void);	//加速度计数据获取

void angle_get_init();			//陀螺仪和加速度计数据获取初始化

float acc_angle_get();			//加速度计角度获取，直接获取角度值



//******电机函数声明********//
void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

#endif