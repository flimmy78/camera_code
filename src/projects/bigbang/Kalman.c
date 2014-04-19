#include "Kalman.h"
#include "board.h"
#include "common.h"
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
void comp_filter(float tg,float dt,cars_status car)
{
//  static float  angle[8];
    float angle_err;//averge;
//    static unsigned int i;
//    angle[(i)&0x07] = car->angle_m;
//    averge = (angle[i%8+0] + angle[i%8+1]+ angle[i%8+2]+ \
//      angle[i%8+3]+ angle[i%8+4]+ angle[i%8+5]+ angle[i%8+6]+ angle[i%8+7])*0.125;  //*0.125是乘法比除法快。
  angle_err      = tg*(car->angle_m - car->angle);   //计算误差
  car->angle    += (angle_err + car->gyro_m)*dt;   //角速度积分得出角度。
 //i++;
  
}




/******************************************************
*   卡尔曼滤波
*******************************************************/

//定义采样时间5毫秒
#define   Kalman_time   0.005   
//状态变换阵
     float A1_1 = 1;
     float A1_2 = -1*Kalman_time;
     float A2_1 = 0;
     float A2_2 = 1;
     float B1_1 = Kalman_time;
     float B2_1 = 0;
     float H1_1 = 1;
     float H1_2 = 0;
    //系数阵  
     float Pest1_1;          //先验估计协方差阵 
     float Pest1_2;
     float Pest2_1;
     float Pest2_2;    
     float Psta1_1 = 1;      //后验估计协方差阵
     float Psta1_2 = 1;  
     float Psta2_1 = 1;  
     float Psta2_2 = 1;   
     float Q1_1 = 0.00001;   //过程激励噪声协方差
     float Q1_2 = 0;  
     float Q2_1 = 0;  
     float Q2_2 = 0.00001;   
     float K1_1;             //卡尔曼增益
     float K2_1;    
     float R = 0.1;          //观测噪声协方差
     float I1_1 = 1;         //单位阵
     float I1_2 = 0;  
     float I2_1 = 0;  
     float I2_2 = 1;   
    //状态阵
     float Xest1_1;          //先验状态估计
     float Xest2_1;          
     float Xsta1_1 = 0;      //后验状态估计
     float Xsta2_1 = 0;    


void Kalman_filter(cars_status car)
{
    
    float th_acc,w_gyro;
    th_acc = car->angle_m;
    w_gyro = car->gyro_d;
    
    
    /**************卡尔曼滤波**************/
    Xest1_1 = Xsta1_1 + A1_2*Xsta2_1 + B1_1*w_gyro;
    Xest2_1 = Xsta2_1;
    
    Pest1_1 = (Psta1_1 + A1_2*Psta2_1) + (Psta1_2 + A1_2*Psta2_2)*A1_2 + Q1_1;
    Pest1_2 =  Psta1_2 + A1_2*Psta2_2;
    Pest2_1 = Psta2_1 + Psta2_2*A1_2;
	Pest2_2 = Psta2_2 + Q2_2;

	K1_1 = Pest1_1/(Pest1_1 + R);
	K2_1 = Pest2_1/(Pest1_1 + R);

	Xsta1_1 = Xest1_1+K1_1*(th_acc - Xest1_1);        
	Xsta2_1 = Xest2_1+K2_1*(th_acc - Xest1_1);        

	Psta1_1 = (I1_1-K1_1) * Pest1_1;   
	Psta1_2 = (I1_1-K1_1) * Pest1_2;
	Psta2_1 =  -K2_1*Pest1_1 + Pest2_1; 
	Psta2_2 =  -K2_1*Pest1_2+  Pest2_2; 
    
    
    
    car->angle = Xsta1_1;
    car->gyro = w_gyro - Xsta2_1;
    
    
//    printf("%f  %f\t\t%f  %f\n",car.angle_m,th_acc,car.angular_m,w_gyro);
}