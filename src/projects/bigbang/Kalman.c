#include "Kalman.h"

//*
//-------------------------------------------------------
//Kalman滤波，8MHz的处理时间约1.8ms；
//-------------------------------------------------------

float  angle_dot; 		//外部需要引用的变量
//cars_status car;  //声明车体状态函数.

//-------------------------------------------------------
float Q_angle=0.001, Q_gyro=0.003, R_angle=0.5;
			//注意：dt的取值为kalman滤波器采样时间;
float P[2][2] = {{ 1, 0 },{ 0, 1 }};
	
float Pdot[4] ={0,0,0,0};

const char C_0 = 1;

float q_bias=25000, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------

void Kalman_Filter(float angle_m,float gyro_m,float dt,cars_status car)			//gyro_m:gyro_measure
{
	

	car->angle +=(gyro_m-q_bias) * dt;//先验估计
	
	Pdot[0]=Q_angle - P[0][1] - P[1][0];// Pk-' 先验估计误差协方差的微分
	Pdot[1]=- P[1][1];
	Pdot[2]=- P[1][1];
	Pdot[3]=Q_gyro;
	
	P[0][0] += Pdot[0] * dt;// Pk- 先验估计误差协方差微分的积分 = 先验估计误差协方差
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;
	
	
	angle_err = angle_m - car->angle;//zk-先验估计
	
	
	PCt_0 = C_0 * P[0][0];
	PCt_1 = C_0 * P[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;//Kk
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * P[0][1];

	P[0][0] -= K_0 * t_0;//后验估计误差协方差
	P[0][1] -= K_0 * t_1;
	P[1][0] -= K_1 * t_0;
	P[1][1] -= K_1 * t_1;
	
	
	car->angle	+= K_0 * angle_err;//后验估计
	q_bias	+= K_1 * angle_err;//后验估计
	angle_dot = gyro_m-q_bias;//输出值（后验估计）的微分 = 角速度
        

}

/*******************************************************************************
 *  互补滤波函数
 *  inpput：   angle_m,加速度计测量原始值，gyro_m，陀螺仪测量数据，tg，比例参数。
 *  建议：    tg参数当大一些，当参数大于1的时候效果较为明显，当参数比较小的时后
 *            会造成，滤波结果整体小于实测值，我把tg参数设定在2.5到4.5之间跟踪
 *            效果比较好，没有明显区别，原理不明，建议设置2.5.
 *  函数功能：利用加速度计测量的角度误差校正陀螺仪积分得出的角度误差。
 *  函数输出：函数输出是将滤波得出的角度存入车体状态结构体car的car.angle.
 *
********************************************************************************/
void comp_filter(float angle_m,float gyro_m,float tg,float dt,cars_status car)
{
  
  float angle_err;
  angle_err     = tg*(angle_m - car->angle);   //计算误差
  car->angle    += (angle_err + gyro_m)*dt;   //角速度积分得出角度。
 
  
}