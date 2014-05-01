#ifndef _DIRECTION_H
#define _DIRECTION_H

#define CCD1_SI  PTA8_OUT
#define CCD1_CLK PTA10_OUT
#define CCD2_SI  PTE10_OUT
#define CCD2_CLK PTE12_OUT


#define  ccd1_left_dot    11     //左黑线原点
#define  ccd1_right_dot   115   //右黑线原点
#define  ccd1_dot         63 //中线原点

#define  ccd2_left_dot    0     //左黑线原点
#define  ccd2_right_dot   115   //右黑线原点
#define  ccd2_dot         58 //中线原点



/*CCD1变量*/
extern unsigned char ccd1_line[8]; //黑线变量
extern unsigned char ccd1_left_line;//左黑线
extern unsigned char ccd1_right_line;//右黑线
extern unsigned char ccd1_left_line_old[2];//左黑线历史
extern unsigned char ccd1_right_line_old[2];//右黑线历史

extern unsigned char ccd1_threshold;//阈值
extern unsigned char ccd1_IntegrationTime;/* 曝光时间，单位ms */
extern unsigned char ccd1_TimerFlag10ms;//10ms标志位
extern unsigned char ccd1_TimerCount10ms; //10ms计时器
extern unsigned char ccd1_Pixel[128];
extern unsigned char ccd1_PixelAverageValue[5];/* 平均AD值 */

extern s16           ccd1_direction_offset_new;//新的方向偏差
extern s16           ccd1_direction_offset_old[50];//旧的方向偏差



/*CCD2变量*/
extern unsigned char ccd2_line[8]; //黑线变量
extern unsigned char ccd2_left_line;   //左黑线
extern unsigned char ccd2_right_line; //右黑线
extern unsigned char ccd2_left_line_old[2];  //左黑线历史
extern unsigned char ccd2_right_line_old[2];  //右黑线历史

extern unsigned char ccd2_threshold;   
extern unsigned char ccd2_IntegrationTime;/* 曝光时间，单位ms */
extern unsigned char ccd2_TimerFlag15ms;  //10ms标志位
extern unsigned char ccd2_TimerCount15ms; //10ms计时器
extern unsigned char ccd2_Pixel[128];
extern unsigned char ccd2_PixelAverageValue[3];/* 平均AD值 */
extern s16           ccd2_direction_offset_new;//新的方向偏差

extern unsigned char zhangai_ad_change;
extern unsigned char zhangai_ad;



/*公共变量*/
extern s16           direction_offset_dot;//方向偏差的微分


 

/*函数声明*/
extern void          ccd1_ImageCapture(void);    //采集像素AD值
extern void          ccd2_ImageCapture(void);    //采集像素AD值
extern void          ccd1_CalculateIntegrationTime(void);   //计算曝光时间
extern void          ccd2_CalculateIntegrationTime(void);   //计算曝光时间
extern void          ccd1_StartIntegration(void); //开始曝光
extern void          ccd2_StartIntegration(void); //开始曝光
extern void          ccd1_ControlInterrupt(void); // 曝光及采集控制函数
extern void          ccd2_ControlInterrupt(void); // 曝光及采集控制函数

extern unsigned char PixelAverage(unsigned char m,unsigned char n,unsigned char *data); //计算像素平均AD值
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