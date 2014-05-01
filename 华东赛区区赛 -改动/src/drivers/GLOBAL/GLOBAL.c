#include "include.h"



/**********************变量定义************************/
//s16 lanya_direction_offset[500]={0};


u16  kalman_count=0;
float gyr_xw_integral=0;
float g_th = 25.5;

float watch1;    //观测变量1
float watch2;    //观测变量2

unsigned char stop_line=0;
unsigned char stop_flag=0;
unsigned char run_flag=0;
unsigned char straight_flag=0;
unsigned char zhangai=0;
unsigned char zhangai_flag=0;
unsigned char flag1=0;
unsigned char flag2=0;
unsigned char flag3=0;
unsigned char flag4=0;
unsigned char flag5=0;
unsigned char flag6=0;


u32 time = 0;
u32 time_zhangai = 0;
u32 time_stop=0;

unsigned char car_states;//车模状态 1表示run
unsigned int max_speed;//车模最大速度
unsigned int min_speed;//车模最小速度

 
carparameters   car = {0,0,0,0,0,0,0};//车模参数变量
motorparameters motor_l = {0,0,0,0,0,0,0,0,0};//左电机参数变量
motorparameters motor_r = {0,0,0,0,0,0,0,0,0};//右电机参数变量

s16  gyrx_offset;  //陀螺仪零偏值
s16  accy_offset;
s16  accz_offset;  //加速度计零偏值
s16  ad_gyrx;
s16  ad_accy;
s16  ad_accz;

float acc_th;
float gyr_xw;



float sd_p = 180;    //直立控制P参数    60        70        80    //第二次双CCD调试参数120 //第三次配重后调试参数170 第四次调试200
float sd_d = 3.5;   //直立控制D参数    1.2       1.5       1.7                         2.2                       2.5           4.0

float sp_p = 1.2;  //速度控制P参数//1.0                        //第二次双CCD调试参数1.1
float sp_i = 1.0;  //速度控制I参数//1.0                                             0.8
                                      
float dr_p = 0;
float dr_d = 0;

float sp_p_old;
float sp_i_old;
s16   zhangai_speed;
s16   jiansu_time;
s16   zhangai_time;
                                      


/***************************************************************
----------------------------------------------------------------                    
 延时函数
----------------------------------------------------------------
****************************************************************/ 
void delay_200ns()
{
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
}
void delay_us(u16 n)
{
	u16 i=0;
	for(i=0;i<n;i++)
	{
		delay_200ns();
		delay_200ns();
		delay_200ns();
		delay_200ns();
		delay_200ns();
	}
}
void delay_ms(u16 n)
{
	u16 i;
	for(i=0;i<n;i++)
	{
		delay_us(1000);
	}
}
void delay_s(u8 n)
{
	u8 i;
	for(i=0;i<n;i++)
	{
		delay_ms(1000);
	}
}

/***************************************************************
----------------------------------------------------------------                    
 系统初始化函数
----------------------------------------------------------------
****************************************************************/ 
void IO_INIT(void)
{
        gpio_init(PORTA,8 ,GPO,LOW);	//线性CCD1提供时钟信号--OUT
	gpio_init(PORTA,10,GPO,LOW);
        gpio_init(PORTE,10,GPO,LOW);	//线性CCD2提供时钟信号--OUT
	gpio_init(PORTE,12,GPO,LOW);
	
	//gpio_init(PORTA,14,GPI,HIGH);	//按键调试输入--IN
	gpio_init(PORTA,16,GPI,HIGH);
	
	gpio_init(PORTA,26,GPI,LOW);	//编码器方向判定--IN
	gpio_init(PORTA,27,GPI,LOW);
	
	gpio_init(PORTC,5,GPO,HIGH);	//指示灯--OUT
	gpio_init(PORTC,7,GPO,HIGH);

	gpio_init(PORTE,0,GPI,LOW);     //拨码开关输入--IN
	gpio_init(PORTE,1,GPI,LOW);
	gpio_init(PORTE,2,GPI,LOW);	
	gpio_init(PORTE,4,GPI,LOW);
	gpio_init(PORTE,6,GPI,LOW);	
	gpio_init(PORTE,8,GPI,LOW);
	
/*	data = PTE28_IN;  //读取PTE28输入电平
	PTB16_OUT  = 1;   //PTB16输出高电平	
*/

	
//   PORTC_PCR19=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//C19引脚设置为GPIO模式，下升沿中断,场中断
//   PORTD_PCR1=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//D1引脚设置为GPIO模式，下降沿中断,行中断
//   PORTC_PCR11=PORT_PCR_MUX(1)|PORT_PCR_IRQC(1);//C11引脚设置为GPIO模式，上升沿触发DMA请求	
//   set_irq_priority (89,1);  //设置场中断优先级
//   set_irq_priority (90,2);  //设置行中断优先级
//   set_irq_priority (0,3);   //设置DMA中断优先级

}

void ADC_INIT(void)
{
	adc_init(ADC0,SE12);//----PORTB2----线性CCD1
        adc_init(ADC0,SE13);//----PORTB3----线性CCD2
        
	adc_init(ADC1,SE8); //----PORTB0----加速度计陀螺仪
	adc_init(ADC1,SE10);//----PORTB4
	adc_init(ADC1,SE12);//----PORTB6
/*	show[0]= ad_ave(ADC0,SE12,ADC_8bit,10);
	show[1]= ad_ave(ADC1,SE8,ADC_16bit,10);
	show[2]= ad_ave(ADC1,SE10,ADC_16bit,10);
	show[3]= ad_ave(ADC1,SE12,ADC_16bit,10);
*/
}

void PWM_INIT(void)					//周期寄存器在FTM.h文件中#define FTM_PRECISON 600u
{
	FTM_PWM_init(FTM0,CH1,10000,0);	//PTA4产生10000Hz方波PWM00，占空比为0，周期寄存器为600
	FTM_PWM_init(FTM0,CH2,10000,0);	//PTA5产生PWM01
	FTM_PWM_init(FTM0,CH3,10000,0);	//PTA6产生PWM10
	FTM_PWM_init(FTM0,CH4,10000,0);	//PTA7产生PWM11
        
//	FTM_PWM_Duty(FTM0,CH1,duty);	//函数调用，设置CH1的占空比寄存器为duty
}
void UART_INIT(void)
{
	uart_init(UART1,9600);
	uart_sendStr (UART1,"\nsystem start ......\n");
        
//      uart_putchar (UART1,'F');     //发送一个字符
//      uart_sendStr (UART1,"STRING");//发送一个字符串
//      UART_IRQ_EN(UART1);  //串口 1   开接收中断
//      UART_IRQ_DIS(UART1); //串口 1   关接收中断
}
void DMA_INIT(void)
{
	DMA_count_Init(DMA_CH0, PTA24 , 0x7FFF, DMA_falling);	//dma计编码器脉冲
	DMA_count_Init(DMA_CH2, PTD2 , 0x7FFF, DMA_falling);	//dma计编码器脉冲
        
//	count_bmq = DMA_count_get(DMA_CH0);	//----读脉冲数
//	DMA_count_reset(DMA_CH0);	        //----计数清零	
}
void PIT_INIT(void)
{
	pit_init_ms(PIT0,1);		//初始化PIT0，定时时间为： 1ms
	pit_init_ms(PIT1,100);		//初始化PIT1，定时时间为： 100ms
        
        disable_irq(68); //关pit0中断
        disable_irq(69); //关pit1中断
	
//	enable_irq(68); //开pit0中断
//      disable_irq(68);//关pit0中断
//	enable_irq(69); //开pit1中断
//      disable_irq(69);//关pit1中断
}

void SYS_INIT(void)
{
	IO_INIT();
	ADC_INIT();
	PWM_INIT();
	UART_INIT();
	DMA_INIT();
	PIT_INIT();
        
        set_irq_priority (63,1);//串口1 接收中断
        set_irq_priority (68,2);//pit0 定时中断
        set_irq_priority (69,3);//pit1 定时中断
        
    
}