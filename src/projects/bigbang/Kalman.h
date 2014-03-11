#ifndef _KALMAN_H
#define _KALMAN_H

extern float angle, angle_dot; 
void Kalman_Filter(float angle_m,float gyro_m);


#endif