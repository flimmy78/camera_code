
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

/********���ٶȼƱ�׼ֵ����*******/
//�����ʱ���ٶȼ� 770mV/g ,��ӦΪADֵ 15220/g , 1553 * m/s^2
//�����ʱ���ٶȼ� 820mV/g ,��ӦΪADֵ 16230/g , 1656 * m/s^2
//���ٶȼ� 		  794mV/g ,��ӦΪADֵ 15725/g , 1605 * m/s^2

#define	ACC_ZERO	26420	//0�ȶ�Ӧ��ADֵ
#define	ACC_90		11200	//ʵ��Ϊ���ٶȼ�-90�ȵ�ֵ
#define	ACC_NEG_90	42650	//ʵ��Ϊ���ٶȼ�90�ȵ�ֵ

#define	ACC_GRA		15725   
/*******************<<*********************/

/*************���ݽṹ������**********>>**/
typedef enum 
{
  ahead=1,
  back=0
}direction;



/*******************��������********************/

//******�����Ǻͼ��ٶȼƺ�������*****//

uint16_t gyro_data_get(void);	//���������ݻ�ȡ

uint16_t acc_data_get(void);	//���ٶȼ����ݻ�ȡ

void angle_get_init();			//�����Ǻͼ��ٶȼ����ݻ�ȡ��ʼ��

float acc_angle_get();			//���ٶȼƽǶȻ�ȡ��ֱ�ӻ�ȡ�Ƕ�ֵ



//******�����������********//
void right_run(uint32_t speed,direction direct);

void left_run(uint32_t speed,direction direct);

void sent_to_computer(uint16_t data1 , uint16_t data2 , uint16_t  data3);

#endif