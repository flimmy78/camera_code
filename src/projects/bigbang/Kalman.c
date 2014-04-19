#include "Kalman.h"
#include "board.h"
#include "common.h"
/*******************************************************************************
 *  �����˲�����
 *  inpput��   angle_m,���ٶȼƲ���ԭʼֵ��gyro_m�������ǲ������ݣ�tg������������
 *  ���飺    tg��������һЩ������������1��ʱ��Ч����Ϊ���ԣ��������Ƚ�С��ʱ��
 *            ����ɣ��˲��������С��ʵ��ֵ���Ұ�tg�����趨��2.5��4.5֮�����
 *            Ч���ȽϺã�û����������ԭ��������������2.5.
 *  �������ܣ����ü��ٶȼƲ����ĽǶ����У�������ǻ��ֵó��ĽǶ���
 *  �����������������ǽ��˲��ó��ĽǶȴ��복��״̬�ṹ��car��car.angle.
 *
********************************************************************************/
void comp_filter(float tg,float dt,cars_status car)
{
//  static float  angle[8];
    float angle_err;//averge;
//    static unsigned int i;
//    angle[(i)&0x07] = car->angle_m;
//    averge = (angle[i%8+0] + angle[i%8+1]+ angle[i%8+2]+ \
//      angle[i%8+3]+ angle[i%8+4]+ angle[i%8+5]+ angle[i%8+6]+ angle[i%8+7])*0.125;  //*0.125�ǳ˷��ȳ����졣
  angle_err      = tg*(car->angle_m - car->angle);   //�������
  car->angle    += (angle_err + car->gyro_m)*dt;   //���ٶȻ��ֵó��Ƕȡ�
 //i++;
  
}




/******************************************************
*   �������˲�
*******************************************************/

//�������ʱ��5����
#define   Kalman_time   0.005   
//״̬�任��
     float A1_1 = 1;
     float A1_2 = -1*Kalman_time;
     float A2_1 = 0;
     float A2_2 = 1;
     float B1_1 = Kalman_time;
     float B2_1 = 0;
     float H1_1 = 1;
     float H1_2 = 0;
    //ϵ����  
     float Pest1_1;          //�������Э������ 
     float Pest1_2;
     float Pest2_1;
     float Pest2_2;    
     float Psta1_1 = 1;      //�������Э������
     float Psta1_2 = 1;  
     float Psta2_1 = 1;  
     float Psta2_2 = 1;   
     float Q1_1 = 0.00001;   //���̼�������Э����
     float Q1_2 = 0;  
     float Q2_1 = 0;  
     float Q2_2 = 0.00001;   
     float K1_1;             //����������
     float K2_1;    
     float R = 0.1;          //�۲�����Э����
     float I1_1 = 1;         //��λ��
     float I1_2 = 0;  
     float I2_1 = 0;  
     float I2_2 = 1;   
    //״̬��
     float Xest1_1;          //����״̬����
     float Xest2_1;          
     float Xsta1_1 = 0;      //����״̬����
     float Xsta2_1 = 0;    


void Kalman_filter(cars_status car)
{
    
    float th_acc,w_gyro;
    th_acc = car->angle_m;
    w_gyro = car->gyro_d;
    
    
    /**************�������˲�**************/
    Xest1_1 = Xsta1_1 + A1_2*Xsta2_1 + B1_1*w_gyro;
    Xest2_1 = Xsta2_1;
    
    Pest1_1 = (Psta1_1 + A1_2*Psta2_1) + (Psta1_2 + A1_2*Psta2_2)*A1_2 + Q1_1;
    Pest1_2 =  Psta1_2 + A1_2*Psta2_2;
    Pest2_1 = Psta2_1 + Psta2_2*A1_2;
	Pest2_2 = Psta2_2 + Q2_2;

	K1_1 = Pest1_1/(Pest1_1 + R);
	K2_1 = Pest2_1/(Pest1_1 + R);

	Xsta1_1 = Xest1_1+K1_1*(th_acc - Xest1_1);        
	Xsta2_1 = Xest2_1+K2_1*(th_acc - Xest1_1);        

	Psta1_1 = (I1_1-K1_1) * Pest1_1;   
	Psta1_2 = (I1_1-K1_1) * Pest1_2;
	Psta2_1 =  -K2_1*Pest1_1 + Pest2_1; 
	Psta2_2 =  -K2_1*Pest1_2+  Pest2_2; 
    
    
    
    car->angle = Xsta1_1;
    car->gyro = w_gyro - Xsta2_1;
    
    
//    printf("%f  %f\t\t%f  %f\n",car.angle_m,th_acc,car.angular_m,w_gyro);
}