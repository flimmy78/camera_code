#include "include.h"


/*CCD1����*/
unsigned char ccd1_line[8] = {240,240,240,240,240,240,240,240}; //���߱���
unsigned char ccd1_left_line=240;   //�����
unsigned char ccd1_right_line=240;  //�Һ���
unsigned char ccd1_left_line_old[2]={ccd1_left_dot,ccd1_left_dot};  //�������ʷ
unsigned char ccd1_right_line_old[2]={ccd1_right_dot,ccd1_right_dot};//�Һ�����ʷ

unsigned char ccd1_threshold=30;   //��ֵ ����20 ����30
unsigned char ccd1_IntegrationTime = 7;/* �ع�ʱ�䣬��λms */
unsigned char ccd1_TimerFlag10ms = 0;  //10ms��־λ
unsigned char ccd1_TimerCount10ms = 0; //10ms��ʱ��
unsigned char ccd1_Pixel[128];
unsigned char ccd1_PixelAverageValue[5]={0};/* ƽ��ADֵ */

s16           ccd1_direction_offset_new=0;//�µķ���ƫ��
s16           ccd1_direction_offset_old[50]={0};//�ɵķ���ƫ��


/*CCD2����*/
unsigned char ccd2_line[8] = {240,240,240,240,240,240,240,240}; //���߱���
unsigned char ccd2_left_line=240;   //�����
unsigned char ccd2_right_line=240;  //�Һ���
unsigned char ccd2_left_line_old[2]={ccd2_left_dot,ccd2_left_dot};  //�������ʷ
unsigned char ccd2_right_line_old[2]={ccd2_right_dot,ccd2_right_dot};//�Һ�����ʷ

unsigned char ccd2_threshold=15;   
unsigned char ccd2_IntegrationTime = 5;/* �ع�ʱ�䣬��λms */
unsigned char ccd2_TimerFlag15ms = 0;  //15ms��־λ
unsigned char ccd2_TimerCount15ms = 0; //15ms��ʱ��
unsigned char ccd2_Pixel[128];
unsigned char ccd2_PixelAverageValue[3];/* ƽ��ADֵ */
unsigned char zhangai_ad_change=30;
unsigned char zhangai_ad=125;

s16           ccd2_direction_offset_new=0;//�µķ���ƫ��


/*��������*/
s16           direction_offset_dot=0;//����ƫ���΢��



                   

/****************************************************************
-----------------------------------------------------------------
�ɼ�����ADֵ
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
     
    for(i=1; i<128; i++)     //�ɼ� Pixel 2~128
    {
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 1;         
        delay_200ns();
        delay_200ns();
         
        ccd1_Pixel[i] = ad_ave(ADC0,SE12,ADC_8bit,1);
        CCD1_CLK = 0;        
    }
    delay_200ns();           //������129��ʱ����Ϊ������־
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
     
    for(i=1; i<128; i++)     //�ɼ� Pixel 2~128
    {
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 1;         
        delay_200ns();
        delay_200ns();
         
        ccd2_Pixel[i] = ad_ave(ADC0,SE13,ADC_8bit,1);
        CCD2_CLK = 0;        
    }
    delay_200ns();           //������129��ʱ����Ϊ������־
    delay_200ns();
    CCD2_CLK = 1;       
    delay_200ns();
    delay_200ns();
    CCD2_CLK = 0;           
}

/****************************************************************
-----------------------------------------------------------------
�����ع�ʱ��
-----------------------------------------------------------------
*****************************************************************/
void ccd1_CalculateIntegrationTime(void)   //�Ե����ع�ʱ��
{
    unsigned char i;
    unsigned char TargetPixelAverageValue = 150;/* �趨Ŀ��ƽ��ADֵ*/
    s16           PixelAverageValueError;/* Ŀ��ֵ��ʵ��ֵ��ƫ��*/
    unsigned char PixelAverageValueAllowError = 15;/* Ŀ��ֵ��ʵ��ֵ�����ƫ�� */
    
    for(i=4;i>=1;i--)
      ccd1_PixelAverageValue[i]=ccd1_PixelAverageValue[i-1];
    
    ccd1_PixelAverageValue[0] = PixelAverage(11,115,ccd1_Pixel);/*����ƽ��ADֵ */
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

void ccd2_CalculateIntegrationTime(void)   //�Ե����ع�ʱ��
{
    unsigned char TargetPixelAverageValue = 145;/* �趨Ŀ��ƽ��ADֵ*/
    s16           PixelAverageValueError;/* Ŀ��ֵ��ʵ��ֵ��ƫ��*/
    unsigned char PixelAverageValueAllowError = 13;/* Ŀ��ֵ��ʵ��ֵ�����ƫ�� */
    
    ccd2_PixelAverageValue[0] = PixelAverage(10,100,ccd2_Pixel);/* ����128�����ص��ƽ��ADֵ */
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
��ʼ�ع⺯��
-----------------------------------------------------------------
*****************************************************************/
void ccd1_StartIntegration(void)   //***����SI�źţ���λ�����������ع⣬�ڴ��ڼ�������129��CLK���壬
                                   //***�Զ����ϴ��ع������ֵ�������ع�ʱ��ԼΪ������SI�ź����¸�SI�ź�
{
    unsigned char i;

    CCD1_SI = 1;        /* SI  = 1 */  //***����SI�źŲ������1��CLK
    delay_200ns();
    CCD1_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    CCD1_SI = 0;        /* SI  = 0 */
    delay_200ns();
    CCD1_CLK = 0;       /* CLK = 0 */

    for(i=0; i<127; i++)              //***�����2~128��CLK
    {
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 1;   /* CLK = 1 */
        delay_200ns();
        delay_200ns();
        CCD1_CLK = 0;   /* CLK = 0 */
    }
    delay_200ns();                    //***�����129��CLK
    delay_200ns();
    CCD1_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    delay_200ns();
    CCD1_CLK = 0;       /* CLK = 0 */
}

void ccd2_StartIntegration(void)   //***����SI�źţ���λ�����������ع⣬�ڴ��ڼ�������129��CLK���壬
                                   //***�Զ����ϴ��ع������ֵ�������ع�ʱ��ԼΪ������SI�ź����¸�SI�ź�
{
    unsigned char i;

    CCD2_SI = 1;        /* SI  = 1 */  //***����SI�źŲ������1��CLK
    delay_200ns();
    CCD2_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    CCD2_SI = 0;        /* SI  = 0 */
    delay_200ns();
    CCD2_CLK = 0;       /* CLK = 0 */

    for(i=0; i<127; i++)              //***�����2~128��CLK
    {
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 1;   /* CLK = 1 */
        delay_200ns();
        delay_200ns();
        CCD2_CLK = 0;   /* CLK = 0 */
    }
    delay_200ns();                    //***�����129��CLK
    delay_200ns();
    CCD2_CLK = 1;       /* CLK = 1 */
    delay_200ns();
    delay_200ns();
    CCD2_CLK = 0;       /* CLK = 0 */
}


/****************************************************************
-----------------------------------------------------------------
 �ع⼰�ɼ����ƺ���
-----------------------------------------------------------------
*****************************************************************/
void ccd1_ControlInterrupt(void)       //1ms�жϺ����д���
{
  unsigned char integration_piont;

  ccd1_TimerCount10ms++;   //10ms��ʱ

  integration_piont = 10 - ccd1_IntegrationTime;
     
  if(ccd1_TimerCount10ms == integration_piont) //�ع�㵽��
    ccd1_StartIntegration();           /* �ع⿪ʼ */
  
  if(ccd1_TimerCount10ms >= 10)  //10ms���ڵ��ˣ��ع�����T��־λTimerFlag10ms��1
  {
    ccd1_TimerCount10ms = 0;
    ccd1_TimerFlag10ms = 1;
  }
}

void ccd2_ControlInterrupt(void)       //1ms�жϺ����д���
{
  unsigned char integration_piont;

  ccd2_TimerCount15ms++;   //15ms��ʱ

  integration_piont = 15 - ccd2_IntegrationTime;
     
  if(ccd2_TimerCount15ms == integration_piont) //�ع�㵽��
    ccd2_StartIntegration();           /* �ع⿪ʼ */
  
  if(ccd2_TimerCount15ms >= 15)  //10ms���ڵ��ˣ��ع�����T��־λTimerFlag10ms��1
  {
    ccd2_TimerCount15ms = 0;
    ccd2_TimerFlag15ms = 1;
  }
}


/****************************************************************
-----------------------------------------------------------------
����ƽ��ֵ
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
���������Ĳ�ֵ
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
��һ�����ľ���ֵ
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
��ȡ�����еķ���ֵ
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
void straight_yn(void)//�ж���ʷƫ���¼�Ƿ�Ϊֱ��
{
  unsigned char m,i,j=0;
  
  m=200/(min_speed/100);//�ж�ccd1 400mm��ʷ��ƫ��
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
��ȡ����
-----------------------------------------------------------------
*****************************************************************/
void ccd1_get_black_line(void)
{
  unsigned char i=0,j=0,k=0;
  signed char m=0,n=4;
  //�洢��ʷ
  ccd1_left_line_old[1]=ccd1_left_line_old[0];
  ccd1_left_line_old[0]=ccd1_left_line;
  ccd1_right_line_old[1]=ccd1_right_line_old[0];
  ccd1_right_line_old[0]=ccd1_right_line;
  
  stop_line = 0;
  ccd1_line[0] = 240;  //line[0~3]�洢������
  ccd1_line[1] = 240;
  ccd1_line[2] = 240;
  ccd1_line[3] = 240;  
  ccd1_line[4] = 240;  //line[4~7]�洢������
  ccd1_line[5] = 240;  
  ccd1_line[6] = 240; 
  ccd1_line[7] = 240; 
  ccd1_left_line = 240;
  ccd1_right_line = 240;

  
  for(i=1; i<=121; i++)//��ȡ���䣬ɨ�貽��Ϊ5
  {
      if( my_dif(ccd1_Pixel[i],ccd1_Pixel[i+5]) > ccd1_threshold )//����
      {
          if(ccd1_Pixel[i] < ccd1_Pixel[i+5])//������
          {
            ccd1_line[m]=i;
            if( (i+5+4<=126 && my_dif(ccd1_Pixel[i],ccd1_Pixel[i+5+4])<ccd1_threshold-7) || (m>=1 && my_dif(ccd1_line[m],ccd1_line[m-1])<13 ) )//������ɫ������ͬһ�����ص�
            {
              ccd1_line[m]=240;
              m--;
            }
            m++;
            if(m>=3)
              m=3;
          }
          else//������
          {
            ccd1_line[n]=i;
            if( (i-4>=1 && my_dif(ccd1_Pixel[i-4],ccd1_Pixel[i+5])<ccd1_threshold-7) || (n>=5 && my_dif(ccd1_line[n],ccd1_line[n-1])<13) )//������ɫ������ͬһ�����ص�
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
  
  for(j=4;j<=7;j++)//��������ͬһ�����ߵ�����
  {
      for(i=0;i<=3;i++)
      {                                                                                                    //16С�������ߵĺ��߿��
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
  for(i=0;i<=3;i++)//�������������
  {
      if(ccd1_line[i] != 240)
        j++;
  }
  for(i=4;i<=7;i++)//�������������
  {
      if(ccd1_line[i] != 240)
        k++;
  }
   
  
  if(j+k>=4)//�ж�������(����������ڵ���4����Ϊ������)
  {
    ccd1_left_line=ccd1_left_line_old[0];
    ccd1_right_line=ccd1_right_line_old[0];
    if(j+k>=5)
      stop_line=1;
  }
  else//��ȡ���Һ���
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
      if( ccd1_left_line > ccd1_right_line )//�ų��ڽ��ܵ�����
      {
        if(ccd1_left_line_old[0]==240)
          ccd1_left_line=240;
        if(ccd1_right_line_old[0]==240)
          ccd1_right_line=240;
        if(ccd1_left_line_old[0]==240 && ccd1_right_line_old[0]==240)//ʮ��·��
          ;
      }  
    }
    if(j==1&&k==2)//��ȫ�ܵ�
    {
      ccd1_left_line = my_one(ccd1_line,0);
      for(i=4;i<=7;i++)
      {
        if(ccd1_line[i]!=240 && ccd1_line[i]>ccd1_left_line)
          ccd1_right_line = ccd1_line[i]+5;
      }
    }
    if(j==2&&k==1)//��ȫ�ܵ�
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
���㷽�� 10ms����
-----------------------------------------------------------------
*****************************************************************/
void ccd1_DirectionCalculate(void)
{
  unsigned char i;
  
  for(i=49; i>0; i--) //�洢50�ε���ʷƫ��
    ccd1_direction_offset_old[i] = ccd1_direction_offset_old[i-1];
  ccd1_direction_offset_old[0] = ccd1_direction_offset_new;
  
  ccd1_get_black_line();
  
  if(ccd1_left_line == 240 && ccd1_right_line == 240)  //û��ɨ������
    ccd1_direction_offset_new = 0;
  if(ccd1_left_line != 240 && ccd1_right_line != 240)  //ɨ����������
    ccd1_direction_offset_new = (s16)ccd1_dot - (s16)(ccd1_left_line+ccd1_right_line)/2; //����ƫ��Ϊ��������ƫ��Ϊ��
  if(ccd1_left_line != 240 && ccd1_right_line == 240)  //ɨ����ߺ���
    ccd1_direction_offset_new = (s16)ccd1_left_dot - (s16)ccd1_left_line; 
  if(ccd1_left_line == 240 && ccd1_right_line != 240)  //ɨ���ұߺ���
    ccd1_direction_offset_new = (s16)ccd1_right_dot - (s16)ccd1_right_line; 

  //if( ccd1_direction_offset_new<=2 && ccd1_direction_offset_new>=-2 ) //��ȥ����
    //ccd1_direction_offset_new = 0;
  
  if( my_dif2(ccd1_direction_offset_old[0],ccd1_direction_offset_new)>=50 && time>time_run+10 )//��ȥͻ��
    ccd1_direction_offset_new = ccd1_direction_offset_old[0]; 
}
                 
/****************************************************************
-----------------------------------------------------------------
������� 20ms����
-----------------------------------------------------------------
*****************************************************************/
void DirectionControl(void)
{ 
  unsigned char m,i;
 
  straight_flag=0;
  straight_yn();
  
  direction_offset_dot = ccd1_direction_offset_new - ccd1_direction_offset_old[0];
  if( my_abs(direction_offset_dot)>30&&time>time_run+20 )//��ȥͻ��
    direction_offset_dot = 30;

  motor_l.direction_pwm_old = motor_l.direction_pwm_new; 
  motor_r.direction_pwm_old = motor_r.direction_pwm_new;
  
  motor_r.direction_pwm_new = dr_p*ccd1_direction_offset_new + dr_d*direction_offset_dot; 
  motor_l.direction_pwm_new =  -motor_l.direction_pwm ;
  
  /*�������ݴ洢
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
���������� 5ms����
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