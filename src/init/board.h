
#ifndef _BOARD_H_
#define _BOARD_H_

#include "FTM.h"
/******************������**>>***********/
//�ҵ��ǰ����FTMģ��
#define RIGHT_A_FTM FTM1
#define RIGHT_A_CH  CH1

//�ҵ�����˵�FTMģ��
#define RIGHT_B_FTM FTM1
#define RIGHT_B_CH  CH0

//����ǰ����FTMģ��
#define LEFT_A_FTM FTM0
#define LEFT_A_CH  CH3

//�������˵�FTMģ��
#define LEFT_B_FTM  FTM0
#define LEFT_B_CH   CH4

//�������Ƶ��
#define MOTOR_FRE 15000

//���������ʼ����ռ�ձȣ���ǿ��Ҫ��Ϊ��0����
#define INIT_DUTY (0)

/*******************<<*********************/

/*************���ݽṹ������**********>>**/
typedef enum 
{
  ahead=1,
  back=0
}direction;



/**************��������*****************/
//���������ݻ�ȡ
uint16_t gyro_data_get(void);

//���ٶȼ����ݻ�ȡ
uint16_t acc_data_get(void);



void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

#endif