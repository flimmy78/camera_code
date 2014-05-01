#ifndef _BALANCE_H
#define _BALANCE_H


#define A1_1 1
#define A1_2 -0.005 //采样时间
#define A2_1 0
#define A2_2 1

#define B1_1 0.005  //采样时间
#define B2_1 0

#define C1_1 1
#define C1_2 0

#define I1_1 1
#define I1_2 0
#define I2_1 0
#define I2_2 1


#define Q1_2 0
#define Q2_1 0

extern float Q1_1;
extern float Q2_2;
extern float R;

//extern float wending;



extern void kalman_filter(void);
extern void wait_kalman(void);
extern void get_gyr_offset(void);
extern void jt_average_filter(void);
extern void motor_set(unsigned char motor_choose, s16 pwm);
extern void motor_control_output(void);
extern void stand_control(void); 


#endif

 
 

 
 
 