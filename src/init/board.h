
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************������**>>***********/

/************�������**************/
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

/*****************����������*********************/
#define     TRANSFER        0.000383         //���ͱ�,(pi*r��*R��)/(100*R��),(3.14 * 40 * 0.032)/(100 * 105),��̥�뾶0.032cm
#define     SPEED_PER       200.0       //������ÿת��������
#define     SPEED_PIT       PIT0        //����������ʹ�õĶ�ʱ��
#define     SPEED_SAMPLING_TIME     20   //����ʱ��  5ms

//*****��������ӵ����ٶȣ���λm/s
#define     SPEED_LA_GET    (DMA_count_get(DMA_CH4)*TRANSFER)/(SPEED_SAMPLING_TIME*0.001)     //����ǰ��
#define     SPEED_LB_GET    (DMA_count_get(DMA_CH5)*TRANSFER)/(SPEED_SAMPLING_TIME*0.001)     //���ֺ���
#define     SPEED_RA_GET    (DMA_count_get(DMA_CH6)*TRANSFER)/(SPEED_SAMPLING_TIME*0.001)
#define     SPEED_RB_GET    (DMA_count_get(DMA_CH7)*TRANSFER)/(SPEED_SAMPLING_TIME*0.001)

#define     SPEED_LA_CLEAR  DMA_count_reset(DMA_CH4)
#define     SPEED_LB_CLEAR  DMA_count_reset(DMA_CH5)
#define     SPEED_RA_CLEAR  DMA_count_reset(DMA_CH6)
#define     SPEED_RB_CLEAR  DMA_count_reset(DMA_CH7)

void    speed_init();

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
void speed_control(cars_status car);
//���õ����
void motor_set(cars_status car);
#endif