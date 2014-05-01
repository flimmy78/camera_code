#ifndef _GLOBAL_H
#define _GLOBAL_H

/**********************************************************
-----------------------------------------------------------
                        �궨��
-----------------------------------------------------------
***********************************************************/

#define ad_basic_value  3320   //ad�ο���ѹmv
#define time_run   800

 

#define dead_value_l_forward 0	//������ǰ����ֵ,(������28)
#define dead_value_l_back 5     //�����������ֵ,(������28)
#define dead_value_r_forward 5  //������ǰ����ֵ,(������27)  
#define dead_value_r_back 0     //�����������ֵ,(������28)    
#define motor_left    1  //�������ų���
#define motor_right   2  //�ҵ�����ų���
   


/**********************************************************
-----------------------------------------------------------
                        �ṹ��
-----------------------------------------------------------
***********************************************************/ 


typedef struct //��ģ�����ṹ��
{
  float th; //���ӽǶ�
  float w;  //���ӽ��ٶ�
  float SpeedAverage; //ƽ���ٶ�
  float SpeedBase;    //�����ٶ�
  float SpeedError;   //�ٶ����
  float Distance;     //�ٶ����Ļ���
  float Acceleration; //�ٶ�����΢��    
}carparameters;

typedef struct //��������ṹ��
{
  float pwm;			//�ܵ�pwmֵ
  float stand_pwm;		//ֱ������pwmֵ
  float speed_pwm;		//�ٶȿ���pwmֵ
  float direction_pwm;	//�������pwmֵ
  float speed_pwm_old;
  float speed_pwm_new;
  float direction_pwm_old;
  float direction_pwm_new;
  signed  int pulse;
}motorparameters;

 

/**********************��������****************************/
//extern s16 lanya_direction_offset[500];

extern u16  kalman_count;
extern float gyr_xw_integral;
extern float g_th; //��ģ���ĽǶȲ���

extern float watch1;   //�۲����1
extern float watch2;   //�۲����2

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


extern unsigned char car_states;//��ģ״̬ 1��ʾrun
extern unsigned int max_speed;//��ģ����ٶ�
extern unsigned int min_speed;//��ģ��С�ٶ�



extern carparameters  car; //��ģ��������
extern motorparameters  motor_l; //������������
extern motorparameters  motor_r; //�ҵ����������

extern s16  gyrx_offset;  //��������ƫֵ
extern s16  accy_offset;
extern s16  accz_offset;  //���ٶȼ���ƫֵ
extern s16  ad_gyrx;
extern s16  ad_accy;
extern s16  ad_accz;


extern float acc_th;   //���ٶȼƽǶ�
extern float gyr_xw;   //�����ǽ��ٶ�


extern float sd_p;    //ֱ������P����
extern float sd_d;    //ֱ������D����

extern float sp_p;    //�ٶȿ���P����
extern float sp_i;    //�ٶȿ���I����

extern float dr_p;    //�������P����
extern float dr_d;    //�������D����

extern float sp_p_old;
extern float sp_i_old;
extern s16   zhangai_speed;
extern s16   jiansu_time;
extern s16   zhangai_time;




/**********************��������****************************/
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