#ifndef _KALMAN_H
#define _KALMAN_H


typedef struct cars_status
{
  //����ռ�ձ�
  
  float left_duty;      //������ռ�ձ�
  float right_duty;     //���ҵ��ռ�ձ�
  
  //�����ʼ������
  float angle_p;        //ֱ�����ƻ��ֺ�ǶȲ���p�Ŀ��ơ�
  float gyro_d;         //ֱ�����ƽǼ��ٶȲ���d��
  float angle_set;      //����ƽ����ǡ�
  float gyro_set;       //����Ŀ����ٶȡ�
  
  float speed_p;        //�ٶȿ���kP������
  float speed_i;         //�ٶȿ���ki����
  float speed_d;        //�ٶȿ���kd����
  float speed_set;      //����Ŀ����ٶȡ�
  
  //���ٶ������ǲ���������ֱ����ص�ֵ
  float angle_m;        // ���ٶȼƲ����Ƕ�
  float gyro_m;         //�����ǽ��ٶȲ���ֵ��
  float angle;          //�˲������ó��ĳ���Ƕ�
  float blance_duty;    //ֱ�����������ռ�ձȡ�
  
  //�������������ٶȿ���
  
  float speed_left_m;           //�����������ٶȲɼ�ֵ��
  float speed_right_m;          //�ҵ���������ٶȲɼ�ֵ
  float speed_duty;             //�ٶȿ������ֵ��
  float speed_duty_old;         //�ٶȿ��Ƽ������ռ�ձ���һ��ֵ��
  float speed_duty_new;         //�ٶȿ���ռ�ձ��������ֵ
  
  //����ͷ�ɼ����Ƕȿ���
  float direction_left_duty;     //���ת������ռ�ձȡ�
  float direction_right_duty;    //���ת������ռ�ձȡ�
  
  
}*cars_status;

extern float  angle_dot; 
void Kalman_Filter(float angle_m,float gyro_m,float dt,cars_status car);
void comp_filter(float tg,float dt,cars_status car);
/******************************************
 *
 *���ڳ���״̬�ṹ��carstatus
 *
 ******************************************/


#endif