
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
#include "Kalman.h"
/******************������**>>***********/

/************�������**************/
//�ҵ��ǰ����FTMģ��
#define RIGHT_A_FTM FTM0
#define RIGHT_A_CH  CH7

//�ҵ�����˵�FTMģ��
#define RIGHT_B_FTM FTM0
#define RIGHT_B_CH  CH6

//����ǰ����FTMģ��
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH5

//�������˵�FTMģ��
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH4

//�������Ƶ��
#define MOTOR_FRE 10000

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

void  speed_init();
s16   left_speed();
s16   right_speed();


/********���ٶȼƱ�׼ֵ����*******/
//���ٶȼ� 		  800mV/g ,��ӦΪADֵ 15887/g , 1605 * m/s^2

#define   ACC_ZOUT_0    29900     //75�Ĳ���ֵ
#define   ACC_XOUT_0    34120     //45�Ĳ���ֵ

//#define	ACC_ZERO	29010.0	    //  0�ȶ�Ӧ��ADֵ
//#define	ACC_GRA		16040.0     //  -12970

/********�����Ǳ�׼ֵ����********/
#define	GYRO_ZERO	26155.0	//��������ֵ,����280�Ĳ���ֵ
//extern  u16 GYRO_ZERO;
#define	GYRO_SCALE	71.28    // 13.3*6/deg.c  
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

//void speed_pid(cars_status car);

void print(cars_status car);

//���������������������ֵ���ز���� �з���16λ
//s16 pulse_cnt_left(void);
//
//s16 pulse_cnt_right(void);

void speed_control_output(cars_status car) ; //���ƽ�������

void direction_control(cars_status car) ; //������������
void direction_control_output(cars_status car); //�ٶȿ���ƽ�������


void camera_wait();


#endif