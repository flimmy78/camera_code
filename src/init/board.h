
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
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

#define	ACC_ZERO	26420.p	//0�ȶ�Ӧ��ADֵ
#define	ACC_90		11200.0	//ʵ��Ϊ���ٶȼ�-90�ȵ�ֵ
#define	ACC_NEG_90	42650.0	//ʵ��Ϊ���ٶȼ�90�ȵ�ֵ

#define	ACC_GRA		15887.0   

/********�����Ǳ�׼ֵ����********/
#define	GYRO_ZERO	24360.0	//��������ֵ
#define	GYRO_SCALE	13.3    // 13.3/deg./sec
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

uint16_t gyro_data_get(void);	//���������ݻ�ȡ

uint16_t acc_data_get(void);	//���ٶȼ����ݻ�ȡ

void angle_get_init();			//�����Ǻͼ��ٶȼ����ݻ�ȡ��ʼ��

//float acc_angle_get();			//���ٶȼƽǶȻ�ȡ��ֱ�ӻ�ȡ�Ƕ�ֵ

float gyro_angular_get();		//�����ǽ��ٶȻ�ȡ

//******�����������********//
void motor_init(void);

void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void right_run_s(int32_t speed);

void left_run_s(int32_t speed);


void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);


float str2num(char * str,u8 n);

#endif