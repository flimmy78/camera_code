#ifndef _KALMAN_H
#define _KALMAN_H


typedef struct cars_status
{
  float angle;            //车体角度
  float left_duty;       //车左电机占空比
  float right_duty;     //车右电机占空比 
  float angle_p;        //直立控制积分后角度参数p的控制。
  float gyro_d;         //直立控制角加速度参数d。
  float angle_set;      //设置平衡晴角。
  float gyro_set;       //设置目标加速度。
  
}*cars_status;

extern float  angle_dot; 
void Kalman_Filter(float angle_m,float gyro_m,float dt,cars_status car);
void comp_filter(float angle_m,float gyro_m,float tg,float dt,cars_status car);
/******************************************
 *
 *定于车体状态结构体carstatus
 *
 ******************************************/


#endif