#include "include.h"


/*CCD1变量*/
unsigned char ccd1_line[8] = {240,240,240,240,240,240,240,240}; //黑线变量
unsigned char ccd1_left_line=240;   //左黑线
unsigned char ccd1_right_line=240;  //右黑线
unsigned char ccd1_left_line_old[2]={ccd1_left_dot,ccd1_left_dot};  //左黑线历史
unsigned char ccd1_right_line_old[2]={ccd1_right_dot,ccd1_right_dot};//右黑线历史

unsigned char ccd1_threshold=30;   //阈值 晚上20 白天30
unsigned char ccd1_IntegrationTime = 7;/* 曝光时间，单位ms */
unsigned char ccd1_TimerFlag10ms = 0;  //10ms标志位
unsigned char ccd1_TimerCount10ms = 0; //10ms计时器
unsigned char ccd1_Pixel[128];
unsigned char ccd1_PixelAverageValue[5]={0};/* 平均AD值 */

s16           ccd1_direction_offset_new=0;//新的方向偏差
s16           ccd1_direction_offset_old[50]={0};//旧的方向偏差


/*CCD2变量*/
unsigned char ccd2_line[8] = {240,240,240,240,240,240,240,240}; //黑线变量
unsigned char ccd2_left_line=240;   //左黑线
unsigned char ccd2_right_line=240;  //右黑线
unsigned char ccd2_left_line_old[2]={ccd2_left_dot,ccd2_left_dot};  //左黑线历史
unsigned char ccd2_right_line_old[2]={ccd2_right_dot,ccd2_right_dot};//右黑线历史

unsigned char ccd2_threshold=15;   
unsigned char ccd2_IntegrationTime = 5;/* 曝光时间，单位ms */
unsigned char ccd2_TimerFlag15ms = 0;  //15ms标志位
unsigned char ccd2_TimerCount15ms = 0; //15ms计时器
unsigned char ccd2_Pixel[128];
unsigned char ccd2_PixelAverageValue[3];/* 平均AD值 */
unsigned char zhangai_ad_change=30;
unsigned char zhangai_ad=125;

s16           ccd2_direction_offset_new=0;//新的方向偏差


/*公共变量*/
s16           direction_offset_dot=0;//方向偏差的微分



                   

/****************************************************************
-----------------------------------------------------------------
采集像素AD值
-----------------------------------------------------------------
*****************************************************************/

void ccd1_ImageCapture(void) 
{
    unsigned char i;
     
    CCD1_SI = 1;             
    delay_200ns();
    CCD1_CLK = 1;            
    delay_200ns();
    CCD1_SI = 0;             
    
    delay_us(10); 

    ccd1_Pixel[0] = ad_ave(ADC0,SE12,ADC_8bit,1);
    CCD1_CLK = 0; 
     
    for(i=1; i<128; i++)     //采集 Pixel 2~128
    {
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 1;         
        delay_200ns();
        delay_200ns();
         
        ccd1_Pixel[i] = ad_ave(ADC0,SE12,ADC_8bit,1);
        CCD1_CLK = 0;        
    }
    delay_200ns();           //产生第129个时钟作为结束标志
    delay_200ns();
    CCD1_CLK = 1;       
    delay_200ns();
    delay_200ns();
    CCD1_CLK = 0;           
}

void ccd2_ImageCapture(void) 
{
    unsigned char i;
     
    CCD2_SI = 1;             
    delay_200ns();
    CCD2_CLK = 1;            
    delay_200ns();
    CCD2_SI = 0;             
    
    delay_us(10); 

    ccd2_Pixel[0] = ad_ave(ADC0,SE13,ADC_8bit,1);
    CCD2_CLK = 0; 
     
    for(i=1; i<128; i++)     //采集 Pixel 2~128
    {
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 1;         
        delay_200ns();
        delay_200ns();
         
        ccd2_Pixel[i] = ad_ave(ADC0,SE13,ADC_8bit,1);
        CCD2_CLK = 0;        
    }
    delay_200ns();           //产生第129个时钟作为结束标志
    delay_200ns();
    CCD2_CLK = 1;       
    delay_200ns();
    delay_200ns();
    CCD2_CLK = 0;           
}

/****************************************************************
-----------------------------------------------------------------
计算曝光时间
-----------------------------------------------------------------
*****************************************************************/
void ccd1_CalculateIntegrationTime(void)   //自调整曝光时间
{
    unsigned char i;
    unsigned char TargetPixelAverageValue = 150;/* 设定目标平均AD值*/
    s16           PixelAverageValueError;/* 目标值与实际值的偏差*/
    unsigned char PixelAverageValueAllowError = 15;/* 目标值与实际值允许的偏差 */
    
    for(i=4;i>=1;i--)
      ccd1_PixelAverageValue[i]=ccd1_PixelAverageValue[i-1];
    
    ccd1_PixelAverageValue[0] = PixelAverage(11,115,ccd1_Pixel);/*计算平均AD值 */
    PixelAverageValueError = (s16)( (s16)TargetPixelAverageValue - (s16)ccd1_PixelAverageValue[0] );
    if(PixelAverageValueError < -PixelAverageValueAllowError)
        ccd1_IntegrationTime--;
    if(PixelAverageValueError > PixelAverageValueAllowError)
        ccd1_IntegrationTime++;
    if(ccd1_IntegrationTime <= 1)
        ccd1_IntegrationTime = 1;
    if(ccd1_IntegrationTime >= 7)
        ccd1_IntegrationTime = 7;
}

void ccd2_CalculateIntegrationTime(void)   //自调整曝光时间
{
    unsigned char TargetPixelAverageValue = 145;/* 设定目标平均AD值*/
    s16           PixelAverageValueError;/* 目标值与实际值的偏差*/
    unsigned char PixelAverageValueAllowError = 13;/* 目标值与实际值允许的偏差 */
    
    ccd2_PixelAverageValue[0] = PixelAverage(10,100,ccd2_Pixel);/* 计算128个像素点的平均AD值 */
    PixelAverageValueError = (s16)( (s16)TargetPixelAverageValue - (s16)ccd2_PixelAverageValue[0] );
    if(PixelAverageValueError < -PixelAverageValueAllowError)
        ccd2_IntegrationTime--;
    if(PixelAverageValueError > PixelAverageValueAllowError)
        ccd2_IntegrationTime++;
    if(ccd2_IntegrationTime <= 1)
        ccd2_IntegrationTime = 1;
    if(ccd2_IntegrationTime >= 13)
        ccd2_IntegrationTime = 13;
}

/****************************************************************
-----------------------------------------------------------------
开始曝光函数
-----------------------------------------------------------------
*****************************************************************/
void ccd1_StartIntegration(void)   //***产生SI信号，复位并启动本次曝光，在此期间必须输出129个CLK脉冲，
                                   //***以丢弃上次曝光的像素值，本次曝光时间约为：本次SI信号至下个SI信号
{
    unsigned char i;

    CCD1_SI = 1;        /* SI  = 1 */  //***产生SI信号并输出第1个CLK
    delay_200ns();
    CCD1_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    CCD1_SI = 0;        /* SI  = 0 */
    delay_200ns();
    CCD1_CLK = 0;       /* CLK = 0 */

    for(i=0; i<127; i++)              //***输出第2~128个CLK
    {
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 1;   /* CLK = 1 */
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 0;   /* CLK = 0 */
    }
    delay_200ns();                    //***输出第129个CLK
    delay_200ns();
    CCD1_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    delay_200ns();
    CCD1_CLK = 0;       /* CLK = 0 */
}

void ccd2_StartIntegration(void)   //***产生SI信号，复位并启动本次曝光，在此期间必须输出129个CLK脉冲，
                                   //***以丢弃上次曝光的像素值，本次曝光时间约为：本次SI信号至下个SI信号
{
    unsigned char i;

    CCD2_SI = 1;        /* SI  = 1 */  //***产生SI信号并输出第1个CLK
    delay_200ns();
    CCD2_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    CCD2_SI = 0;        /* SI  = 0 */
    delay_200ns();
    CCD2_CLK = 0;       /* CLK = 0 */

    for(i=0; i<127; i++)              //***输出第2~128个CLK
    {
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 1;   /* CLK = 1 */
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 0;   /* CLK = 0 */
    }
    delay_200ns();                    //***输出第129个CLK
    delay_200ns();
    CCD2_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    delay_200ns();
    CCD2_CLK = 0;       /* CLK = 0 */
}


/****************************************************************
-----------------------------------------------------------------
 曝光及采集控制函数
-----------------------------------------------------------------
*****************************************************************/
void ccd1_ControlInterrupt(void)       //1ms中断函数中处理
{
  unsigned char integration_piont;

  ccd1_TimerCount10ms++;   //10ms计时

  integration_piont = 10 - ccd1_IntegrationTime;
     
  if(ccd1_TimerCount10ms == integration_piont) //曝光点到了
    ccd1_StartIntegration();           /* 曝光开始 */
  
  if(ccd1_TimerCount10ms >= 10)  //10ms周期到了，曝光结束标志位TimerFlag10ms置1
  {
    ccd1_TimerCount10ms = 0;
    ccd1_TimerFlag10ms = 1;
  }
}

void ccd2_ControlInterrupt(void)       //1ms中断函数中处理
{
  unsigned char integration_piont;

  ccd2_TimerCount15ms++;   //15ms计时

  integration_piont = 15 - ccd2_IntegrationTime;
     
  if(ccd2_TimerCount15ms == integration_piont) //曝光点到了
    ccd2_StartIntegration();           /* 曝光开始 */
  
  if(ccd2_TimerCount15ms >= 15)  //10ms周期到了，曝光结束标志位TimerFlag10ms置1
  {
    ccd2_TimerCount15ms = 0;
    ccd2_TimerFlag15ms = 1;
  }
}


/****************************************************************
-----------------------------------------------------------------
计算平均值
-----------------------------------------------------------------
*****************************************************************/
unsigned char PixelAverage(unsigned char m,unsigned char n,unsigned char *data) 
{
  unsigned char i;
  unsigned int sum = 0;
  for(i = m; i<=n; i++) 
  {
    sum = sum + data[i];
  }
  return ( (unsigned char)(sum/(n-m+1)) );
}
s16 average(unsigned char m, unsigned char n, s16 *data )
{
  unsigned char i;
  s16  sum = 0;
  for(i=m; i<=n; i++) 
  {
    sum = sum + data[i];
  }
  return ( (s16)(sum/(n-m+1)) );
}
/****************************************************************
-----------------------------------------------------------------
求两个数的差值
-----------------------------------------------------------------
*****************************************************************/
unsigned char my_dif(unsigned char a, unsigned char b)
{
  if(a>b)
    return(a-b);
  else
    return(b-a);
}
s16 my_dif2(s16 a, s16 b)
{
  if(a>b)
    return(a-b);
  else
    return(b-a);
}

/****************************************************************
-----------------------------------------------------------------
求一个数的绝对值
-----------------------------------------------------------------
*****************************************************************/
s16 my_abs(s16 a)
{
  if(a>0)
    return a;
  else
    return (-a);
}
/****************************************************************
-----------------------------------------------------------------
提取数组中的非零值
-----------------------------------------------------------------
*****************************************************************/
unsigned char my_one(unsigned char *line,unsigned char n)
{
  unsigned char i;
  if(n==0)
  {
    for(i=0;i<=3;i++)
    {
      if(line[i]!=240)
        return line[i];
    }
  }
  else
  {
    for(i=4;i<=7;i++)
    {
      if(line[i]!=240)
        return line[i];
    }
  }
  return (240);
}
void straight_yn(void)//判断历史偏差记录是否为直道
{
  unsigned char m,i,j=0;
  
  m=200/(min_speed/100);//判断ccd1 400mm历史的偏差
  if(m>=50)
    m=50;
  
  for(i=0; i<m; i++)
  {
    if(ccd1_direction_offset_old[i]>=10)
      j++;
  }
  if( j<=2 && ccd1_direction_offset_old[0]<=10 )
    straight_flag=1;
  else
    straight_flag=0;
}

















/****************************************************************
-----------------------------------------------------------------
获取黑线
-----------------------------------------------------------------
*****************************************************************/
void ccd1_get_black_line(void)
{
  unsigned char i=0,j=0,k=0;
  signed char m=0,n=4;
  //存储历史
  ccd1_left_line_old[1]=ccd1_left_line_old[0];
  ccd1_left_line_old[0]=ccd1_left_line;
  ccd1_right_line_old[1]=ccd1_right_line_old[0];
  ccd1_right_line_old[0]=ccd1_right_line;
  
  stop_line = 0;
  ccd1_line[0] = 240;  //line[0~3]存储上跳变
  ccd1_line[1] = 240;
  ccd1_line[2] = 240;
  ccd1_line[3] = 240;  
  ccd1_line[4] = 240;  //line[4~7]存储下跳变
  ccd1_line[5] = 240;  
  ccd1_line[6] = 240; 
  ccd1_line[7] = 240; 
  ccd1_left_line = 240;
  ccd1_right_line = 240;

  
  for(i=1; i<=121; i++)//提取跳变，扫描步长为5
  {
      if( my_dif(ccd1_Pixel[i],ccd1_Pixel[i+5]) > ccd1_threshold )//跳变
      {
          if(ccd1_Pixel[i] < ccd1_Pixel[i+5])//上跳变
          {
            ccd1_line[m]=i;
            if( (i+5+4<=126 && my_dif(ccd1_Pixel[i],ccd1_Pixel[i+5+4])<ccd1_threshold-7) || (m>=1 && my_dif(ccd1_line[m],ccd1_line[m-1])<13 ) )//消除白色噪点或者同一跳变重叠
            {
              ccd1_line[m]=240;
              m--;
            }
            m++;
            if(m>=3)
              m=3;
          }
          else//下跳变
          {
            ccd1_line[n]=i;
            if( (i-4>=1 && my_dif(ccd1_Pixel[i-4],ccd1_Pixel[i+5])<ccd1_threshold-7) || (n>=5 && my_dif(ccd1_line[n],ccd1_line[n-1])<13) )//消除白色噪点或者同一跳变重叠
            {
              ccd1_line[n]=240;
              n--;
            }
            n++;
            if(n>=7)
              n=7;
          }
      }
  }
  
  for(j=4;j<=7;j++)//消除来自同一根黑线的跳变
  {
      for(i=0;i<=3;i++)
      {                                                                                                    //16小于起跑线的黑线宽度
        if( ccd1_line[j]!=240 && ccd1_line[i]!=240 && ccd1_line[j]<ccd1_line[i] && ccd1_line[i]-ccd1_line[j]<16 )
        {
          if( my_dif(ccd1_Pixel[ccd1_line[j]],ccd1_Pixel[ccd1_line[j]+5]) < my_dif(ccd1_Pixel[ccd1_line[i]],ccd1_Pixel[ccd1_line[i]+5]) )
            ccd1_line[j]=240;
          else
            ccd1_line[i]=240;
        }
      }
  }
  
  j=0;
  k=0;
  for(i=0;i<=3;i++)//计算上跳变个数
  {
      if(ccd1_line[i] != 240)
        j++;
  }
  for(i=4;i<=7;i++)//计算下跳变个数
  {
      if(ccd1_line[i] != 240)
        k++;
  }
   
  
  if(j+k>=4)//判断起跑线(跳变个数大于等于4个即为起跑线)
  {
    ccd1_left_line=ccd1_left_line_old[0];
    ccd1_right_line=ccd1_right_line_old[0];
    if(j+k>=5)
      stop_line=1;
  }
  else//提取左右黑线
  {
    if(j==0&&k==0)
    {
      ccd1_left_line = 240;
      ccd1_right_line = 240;
    }
    if(j==0&&k==1)
    {
      ccd1_left_line = 240;
      ccd1_right_line = my_one(ccd1_line,1)+5; 
    }
    if(j==1&&k==0)
    {
      ccd1_left_line = my_one(ccd1_line,0);
      ccd1_right_line=240;
    }
    if(j==1&&k==1)
    {
      ccd1_left_line = my_one(ccd1_line,0);
      ccd1_right_line = my_one(ccd1_line,1)+5; 
      if( ccd1_left_line > ccd1_right_line )//排除邻近跑道的线
      {
        if(ccd1_left_line_old[0]==240)
          ccd1_left_line=240;
        if(ccd1_right_line_old[0]==240)
          ccd1_right_line=240;
        if(ccd1_left_line_old[0]==240 && ccd1_right_line_old[0]==240)//十字路口
          ;
      }  
    }
    if(j==1&&k==2)//凑全跑道
    {
      ccd1_left_line = my_one(ccd1_line,0);
      for(i=4;i<=7;i++)
      {
        if(ccd1_line[i]!=240 && ccd1_line[i]>ccd1_left_line)
          ccd1_right_line = ccd1_line[i]+5;
      }
    }
    if(j==2&&k==1)//凑全跑道
    {
      ccd1_right_line = my_one(ccd1_line,1)+5;
      for(i=0;i<=3;i++)
      {
        if(ccd1_line[i]!=240 && ccd1_line[i]<ccd1_right_line)
          ccd1_left_line = ccd1_line[i];
      }
    }
    
    
    if(j==0 && k==2)
    {
      ccd1_left_line=ccd1_left_line_old[0];
      ccd1_right_line=ccd1_right_line_old[0];
    }
    if(j==2 && k==0)
    {
      ccd1_left_line=ccd1_left_line_old[0];
      ccd1_right_line=ccd1_right_line_old[0];
    }
    if(ccd1_left_line!=240 && ccd1_right_line!=240 && ccd1_right_line-ccd1_left_line<=40 && ccd1_right_line-ccd1_left_line>=0)
    {
      ccd1_left_line = ccd1_left_line_old[0];
      ccd1_right_line = ccd1_right_line_old[0];
    } 
  }
}
void ccd2_get_black_line(void)
{ 
  ccd2_PixelAverageValue[2] = ccd2_PixelAverageValue[1];
  ccd2_PixelAverageValue[1] = ccd2_PixelAverageValue[0];
  ccd2_PixelAverageValue[0] = PixelAverage(15,100,ccd2_Pixel);
      
  zhangai=0;
  if(ccd2_PixelAverageValue[2]>ccd2_PixelAverageValue[0] && ccd2_PixelAverageValue[2]-ccd2_PixelAverageValue[0]>=zhangai_ad_change && ccd2_PixelAverageValue[0]<=zhangai_ad )
    zhangai=1;
}
/****************************************************************
-----------------------------------------------------------------
计算方向 10ms周期
-----------------------------------------------------------------
*****************************************************************/
void ccd1_DirectionCalculate(void)
{
  unsigned char i;
  
  for(i=49; i>0; i--) //存储50次的历史偏差
    ccd1_direction_offset_old[i] = ccd1_direction_offset_old[i-1];
  ccd1_direction_offset_old[0] = ccd1_direction_offset_new;
  
  ccd1_get_black_line();
  
  if(ccd1_left_line == 240 && ccd1_right_line == 240)  //没有扫到黑线
    ccd1_direction_offset_new = 0;
  if(ccd1_left_line != 240 && ccd1_right_line != 240)  //扫到两条黑线
    ccd1_direction_offset_new = (s16)ccd1_dot - (s16)(ccd1_left_line+ccd1_right_line)/2; //向左偏离为负，向右偏离为正
  if(ccd1_left_line != 240 && ccd1_right_line == 240)  //扫到左边黑线
    ccd1_direction_offset_new = (s16)ccd1_left_dot - (s16)ccd1_left_line; 
  if(ccd1_left_line == 240 && ccd1_right_line != 240)  //扫到右边黑线
    ccd1_direction_offset_new = (s16)ccd1_right_dot - (s16)ccd1_right_line; 

  //if( ccd1_direction_offset_new<=2 && ccd1_direction_offset_new>=-2 ) //消去死区
    //ccd1_direction_offset_new = 0;
  
  if( my_dif2(ccd1_direction_offset_old[0],ccd1_direction_offset_new)>=50 && time>time_run+10 )//消去突变
    ccd1_direction_offset_new = ccd1_direction_offset_old[0]; 
}
                 
/****************************************************************
-----------------------------------------------------------------
方向控制 20ms周期
-----------------------------------------------------------------
*****************************************************************/
void DirectionControl(void)
{ 
  unsigned char m,i;
 
  straight_flag=0;
  straight_yn();
  
  direction_offset_dot = ccd1_direction_offset_new - ccd1_direction_offset_old[0];
  if( my_abs(direction_offset_dot)>30&&time>time_run+20 )//消去突变
    direction_offset_dot = 30;

  motor_l.direction_pwm_old = motor_l.direction_pwm_new; 
  motor_r.direction_pwm_old = motor_r.direction_pwm_new;
  
  motor_r.direction_pwm_new = dr_p*ccd1_direction_offset_new + dr_d*direction_offset_dot; 
  motor_l.direction_pwm_new =  -motor_l.direction_pwm ;
  
  /*蓝牙数据存储
  static s16 lanya_i=0;
  if(car.SpeedAverage>200)
  {
    lanya_direction_offset[lanya_i]=ccd2_PixelAverageValue[0];
    lanya_i++;
    if(lanya_i>=499)
      lanya_i=499;
  }*/
}

/*********************************************************
----------------------------------------------------------
方向控制输出 5ms周期
----------------------------------------------------------
**********************************************************/
void DirectionControlOutput(void) 
{    
  float value_l,value_r;    
  value_l=(motor_l.direction_pwm_new - motor_l.direction_pwm_old)/2.0;
  value_r=(motor_r.direction_pwm_new - motor_r.direction_pwm_old)/2.0;
  motor_l.direction_pwm += value_l;
  motor_r.direction_pwm += value_r;              
}