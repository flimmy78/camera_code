
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************������**>>***********/

/************�������**************/
//�ҵ��ǰ����FTMģ��
#define RIGHT_A_FTM FTM0
#define RIGHT_A_CH  CH2

//�ҵ�����˵�FTMģ��
#define RIGHT_B_FTM FTM0
#define RIGHT_B_CH  CH0

//����ǰ����FTMģ��
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH3

//�������˵�FTMģ��
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH1

//�������Ƶ��
#define MOTOR_FRE 15000

//���������ʼ����ռ�ձȣ���ǿ��Ҫ��Ϊ��0����
#define INIT_DUTY (0)


//���뿪�ء����롿�Ĵ�����͡���λ��λ���ĺ�
#define SW8_DATA_IN  GPIOC_PDIR
#define SW8_MBITS    8

//������صĺ�
#define KEY1_IN       PTE1_IN
#define KEY2_IN       PTE3_IN
#define KEY3_IN       PTE5_IN

/*****************����������*********************/
#define     TRANSFER        0.000383    //���ͱ�,����������Ӧ�ٶȣ���λ��m��(pi*r��*R��)/(100*R��),(3.14 * 40 * 0.032)/(100 * 105),��̥�뾶0.032m
#define     SPEED_PER       200.0       //������ÿת��������
#define     SPEED_SAMPLING_TIME     100  //����ʱ��  20ms

//*****��������ӵ����ٶȣ���λm/s

void    speed_init();
float   left_speed_get();
float   right_speed_get();

/********���ٶȼƱ�׼ֵ����*******/
//���ٶȼ� 		  800mV/g ,��ӦΪADֵ 15887/g , 1605 * m/s^2

#define	ACC_ZERO	28694.0	//0�ȶ�Ӧ��ADֵ 27790

#define	ACC_GRA		14925.0  //15887.0      -12865

/********�����Ǳ�׼ֵ����********/
#define	GYRO_ZERO	27099.0	//��������ֵ
#define	GYRO_SCALE	65.17  // 13.3*4.9/deg.c  82.46

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

uint8_t sw8_data_get(void);

void wait_key1(void);

void wait_key2(void);

void wait_key3(void);


void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);


extern float OutData[4];
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void send_toscope(void);


float str2num(char * str,u8 n);
void blance_comp_filter(float tg,float dt,cars_status car);
void blance_comp_filter_pid(float tg,float dt,cars_status car);
void blance_kalman_filter(cars_status car);
void speed_control(cars_status car);
//���õ����
void motor_set(cars_status car);

void speed_pid(cars_status car);

void print(cars_status car);

//���������������������ֵ���ز���� �з���16λ
s16 pulse_cnt_left(void);

s16 pulse_cnt_right(void);

void speed_control_output(cars_status car) ; //���ƽ�������

float left_speed();
float right_speed();


/************����ͷ����************/
#define    ROW_START    120
#define    ROW_END      159
#define    ROW          40      //ROW_END-ROW_START
#define    COL          200

void camera_wait();


#endif