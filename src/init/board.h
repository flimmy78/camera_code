
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************������**>>***********/
//�ҵ��ǰ����FTMģ��
#define RIGHT_A_FTM FTM1
#define RIGHT_A_CH  CH0

//�ҵ�����˵�FTMģ��
#define RIGHT_B_FTM FTM1
#define RIGHT_B_CH  CH1

//����ǰ����FTMģ��
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH4

//�������˵�FTMģ��
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH3

//�������Ƶ��
#define MOTOR_FRE 15000

//���������ʼ����ռ�ձȣ���ǿ��Ҫ��Ϊ��0����
#define INIT_DUTY (0)

/********���ٶȼƱ�׼ֵ����*******/
//���ٶȼ� 		  800mV/g ,��ӦΪADֵ 15887/g , 1605 * m/s^2

#define	ACC_ZERO	26420.0	//0�ȶ�Ӧ��ADֵ
#define	ACC_90		11200.0	//ʵ��Ϊ���ٶȼ�-90�ȵ�ֵ
#define	ACC_NEG_90	42650.0	//ʵ��Ϊ���ٶȼ�90�ȵ�ֵ

#define	ACC_GRA		15887.0   

/********�����Ǳ�׼ֵ����********/
#define	GYRO_ZERO	27583.0	//��������ֵ
#define	GYRO_SCALE	119.7    // 13.3*9/deg./sec
/*******************<<*********************/




/*************���ݽṹ������**********>>**/
typedef enum 
{
   ahead=1,
   back=0
}direction;




typedef struct {
  float SetData;      //Ŀ������
  float ActualData;   // ʵ������
  float err;         //ƫ��
  float err_last;    //��һ�ε�ƫ��
  float Kp,Ki,Kd;    //���������֡�΢��ϵ��
  float Implement;     //ִ������ֵ
  float intergal;  //����ֵ
}* pid_struct;


/*******************��������********************/

//******�����Ǻͼ��ٶȼƺ�������*****//

float gyro_data_get(void);	//���������ݻ�ȡ

float acc_data_get(void);	//���ٶȼ����ݻ�ȡ

void angle_get_init();			//�����Ǻͼ��ٶȼ����ݻ�ȡ��ʼ��

//******�����������********//
void motor_init(void);

void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void right_run_s(int32_t speed);

void left_run_s(int32_t speed);


void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

float str2num(char * str,u8 n);
void blance_comp_filter(float tg,float dt,cars_status car);
void speed_control(car_status car)

#endif