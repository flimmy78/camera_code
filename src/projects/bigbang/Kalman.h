#ifndef _KALMAN_H
#define _KALMAN_H


typedef struct cars_status
{
  //车体占空比
  
  float left_duty;      //车左电机占空比
  float right_duty;     //车右电机占空比
  
  //车体初始化参数
  float angle_p;        //直立控制积分后角度参数p的控制。
  float gyro_d;         //直立控制角加速度参数d。
  float angle_set;      //设置平衡倾角。
  float gyro_set;       //设置目标加速度。
  
  float speed_p;        //速度控制kP参数。
  float speed_i;         //速度控制ki参数
  float speed_d;        //速度控制kd参数
  float speed_set;      //设置目标加速度。
  
  //加速度陀螺仪测量控制与直立相关的值
  float angle_m;        // 加速度计测量角度
  float gyro_m;         //陀螺仪角速度测量值。
  float angle;          //滤波后计算得出的车体角度
  float blance_duty;    //直立控制所需的占空比。
  
  //编码器测量与速度控制
  
  float speed_left_m;           //左电机编码器速度采集值。
  float speed_right_m;          //右电机编码器速度采集值
  float speed_duty;             //速度控制输出值。
  float speed_duty_old;         //速度控制计算出的占空比上一次值。
  float speed_duty_new;         //速度控制占空比输出更新值
  
  //摄像头采集，角度控制
  float direction_left_duty;     //电机转向左轮占空比。
  float direction_right_duty;    //电机转向游轮占空比。
  
  
}*cars_status;

extern float  angle_dot; 
void Kalman_Filter(float angle_m,float gyro_m,float dt,cars_status car);
void comp_filter(float tg,float dt,cars_status car);
/******************************************
 *
 *定于车体状态结构体carstatus
 *
 ******************************************/


#endif