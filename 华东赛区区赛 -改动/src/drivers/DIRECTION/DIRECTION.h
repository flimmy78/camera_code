#ifndef _DIRECTION_H
#define _DIRECTION_H

#define CCD1_SI  PTA8_OUT
#define CCD1_CLK PTA10_OUT
#define CCD2_SI  PTE10_OUT
#define CCD2_CLK PTE12_OUT


#define  ccd1_left_dot    11     //�����ԭ��
#define  ccd1_right_dot   115   //�Һ���ԭ��
#define  ccd1_dot         63 //����ԭ��

#define  ccd2_left_dot    0     //�����ԭ��
#define  ccd2_right_dot   115   //�Һ���ԭ��
#define  ccd2_dot         58 //����ԭ��



/*CCD1����*/
extern unsigned char ccd1_line[8]; //���߱���
extern unsigned char ccd1_left_line;//�����
extern unsigned char ccd1_right_line;//�Һ���
extern unsigned char ccd1_left_line_old[2];//�������ʷ
extern unsigned char ccd1_right_line_old[2];//�Һ�����ʷ

extern unsigned char ccd1_threshold;//��ֵ
extern unsigned char ccd1_IntegrationTime;/* �ع�ʱ�䣬��λms */
extern unsigned char ccd1_TimerFlag10ms;//10ms��־λ
extern unsigned char ccd1_TimerCount10ms; //10ms��ʱ��
extern unsigned char ccd1_Pixel[128];
extern unsigned char ccd1_PixelAverageValue[5];/* ƽ��ADֵ */

extern s16           ccd1_direction_offset_new;//�µķ���ƫ��
extern s16           ccd1_direction_offset_old[50];//�ɵķ���ƫ��



/*CCD2����*/
extern unsigned char ccd2_line[8]; //���߱���
extern unsigned char ccd2_left_line;   //�����
extern unsigned char ccd2_right_line; //�Һ���
extern unsigned char ccd2_left_line_old[2];  //�������ʷ
extern unsigned char ccd2_right_line_old[2];  //�Һ�����ʷ

extern unsigned char ccd2_threshold;   
extern unsigned char ccd2_IntegrationTime;/* �ع�ʱ�䣬��λms */
extern unsigned char ccd2_TimerFlag15ms;  //10ms��־λ
extern unsigned char ccd2_TimerCount15ms; //10ms��ʱ��
extern unsigned char ccd2_Pixel[128];
extern unsigned char ccd2_PixelAverageValue[3];/* ƽ��ADֵ */
extern s16           ccd2_direction_offset_new;//�µķ���ƫ��

extern unsigned char zhangai_ad_change;
extern unsigned char zhangai_ad;



/*��������*/
extern s16           direction_offset_dot;//����ƫ���΢��


 

/*��������*/
extern void          ccd1_ImageCapture(void);    //�ɼ�����ADֵ
extern void          ccd2_ImageCapture(void);    //�ɼ�����ADֵ
extern void          ccd1_CalculateIntegrationTime(void);   //�����ع�ʱ��
extern void          ccd2_CalculateIntegrationTime(void);   //�����ع�ʱ��
extern void          ccd1_StartIntegration(void); //��ʼ�ع�
extern void          ccd2_StartIntegration(void); //��ʼ�ع�
extern void          ccd1_ControlInterrupt(void); // �ع⼰�ɼ����ƺ���
extern void          ccd2_ControlInterrupt(void); // �ع⼰�ɼ����ƺ���

extern unsigned char PixelAverage(unsigned char m,unsigned char n,unsigned char *data); //��������ƽ��ADֵ
extern s16           average(unsigned char m, unsigned char n, s16 *data );
extern unsigned char my_dif(unsigned char a, unsigned char b);
extern s16           my_dif2(s16 a, s16 b);
extern s16           my_abs(s16 a);
extern unsigned char my_one(unsigned char *line,unsigned char n);
extern void straight_yn(void);
extern void zhangai_yn(void);



extern void ccd1_get_black_line(void);
extern void ccd2_get_black_line(void);
extern void ccd2_DirectionCalculate(void);
extern void DirectionControl(void);
extern void DirectionControlOutput(void);



#endif