#ifndef _GLOBAL_H
#define _GLOBAL_H

/**********************************************************
-----------------------------------------------------------
                        宏定义
-----------------------------------------------------------
***********************************************************/

#define ad_basic_value  3320   //ad参考电压mv
#define time_run   800

 

#define dead_value_l_forward 0	//左轮向前死区值,(悬空下28)
#define dead_value_l_back 5     //左轮向后死区值,(悬空下28)
#define dead_value_r_forward 5  //右轮向前死区值,(悬空下27)  
#define dead_value_r_back 0     //右轮向后死区值,(悬空无28)    
#define motor_left    1  //左电机符号常量
#define motor_right   2  //右电机符号常量
   


/**********************************************************
-----------------------------------------------------------
                        结构体
-----------------------------------------------------------
***********************************************************/ 


typedef struct //车模参数结构体
{
  float th; //车子角度
  float w;  //车子角速度
  float SpeedAverage; //平均速度
  float SpeedBase;    //给定速度
  float SpeedError;   //速度误差
  float Distance;     //速度误差的积分
  float Acceleration; //速度误差的微分    
}carparameters;

typedef struct //电机参数结构体
{
  float pwm;			//总的pwm值
  float stand_pwm;		//直立控制pwm值
  float speed_pwm;		//速度控制pwm值
  float direction_pwm;	//方向控制pwm值
  float speed_pwm_old;
  float speed_pwm_new;
  float direction_pwm_old;
  float direction_pwm_new;
  signed  int pulse;
}motorparameters;

 

/**********************变量声明****************************/
//extern s16 lanya_direction_offset[500];

extern u16  kalman_count;
extern float gyr_xw_integral;
extern float g_th; //车模重心角度补偿

extern float watch1;   //观测变量1
extern float watch2;   //观测变量2

extern unsigned char stop_line;
extern unsigned char stop_flag;
extern unsigned char run_flag;
extern unsigned char straight_flag;
extern unsigned char zhangai;
extern unsigned char zhangai_flag;
extern unsigned char flag1;
extern unsigned char flag2;
extern unsigned char flag3;
extern unsigned char flag4;
extern unsigned char flag5;
extern unsigned char flag6;

extern u32 time;  
extern u32 time_zhangai;
extern u32 time_stop;


extern unsigned char car_states;//车模状态 1表示run
extern unsigned int max_speed;//车模最大速度
extern unsigned int min_speed;//车模最小速度



extern carparameters  car; //车模参数变量
extern motorparameters  motor_l; //左电机参数变量
extern motorparameters  motor_r; //右电机参数变量

extern s16  gyrx_offset;  //陀螺仪零偏值
extern s16  accy_offset;
extern s16  accz_offset;  //加速度计零偏值
extern s16  ad_gyrx;
extern s16  ad_accy;
extern s16  ad_accz;


extern float acc_th;   //加速度计角度
extern float gyr_xw;   //陀螺仪角速度


extern float sd_p;    //直立控制P参数
extern float sd_d;    //直立控制D参数

extern float sp_p;    //速度控制P参数
extern float sp_i;    //速度控制I参数

extern float dr_p;    //方向控制P参数
extern float dr_d;    //方向控制D参数

extern float sp_p_old;
extern float sp_i_old;
extern s16   zhangai_speed;
extern s16   jiansu_time;
extern s16   zhangai_time;




/**********************函数声明****************************/
extern void delay_200ns();
extern void delay_us(u16 n);
extern void delay_ms(u16 n);
extern void delay_s(u8 n);

extern void IO_INIT(void);
extern void	ADC_INIT(void);
extern void	PWM_INIT(void);
extern void	UART_INIT(void);
extern void	DMA_INIT(void);
extern void	PIT_INIT(void);
extern void SYS_INIT(void);
 
#endif