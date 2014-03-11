#ifndef _KALMAN_H
#define _KALMAN_H


typedef struct cars_status
{
  float angle;            //����Ƕ�
  float left_duty;       //������ռ�ձ�
  float right_duty;     //���ҵ��ռ�ձ� 
  float angle_p;        //ֱ�����ƻ��ֺ�ǶȲ���p�Ŀ��ơ�
  float gyro_d;         //ֱ�����ƽǼ��ٶȲ���d��
  float angle_set;      //����ƽ����ǡ�
  float gyro_set;       //����Ŀ����ٶȡ�
  
}*cars_status;

extern float  angle_dot; 
void Kalman_Filter(float angle_m,float gyro_m,float dt,cars_status car);
void comp_filter(float angle_m,float gyro_m,float tg,float dt,cars_status car);
/******************************************
 *
 *���ڳ���״̬�ṹ��carstatus
 *
 ******************************************/


#endif