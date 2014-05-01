#include "include.h"



/**********************��������************************/
//s16 lanya_direction_offset[500]={0};


u16  kalman_count=0;
float gyr_xw_integral=0;
float g_th = 25.5;

float watch1;    //�۲����1
float watch2;    //�۲����2

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

unsigned char car_states;//��ģ״̬ 1��ʾrun
unsigned int max_speed;//��ģ����ٶ�
unsigned int min_speed;//��ģ��С�ٶ�

 
carparameters   car = {0,0,0,0,0,0,0};//��ģ��������
motorparameters motor_l = {0,0,0,0,0,0,0,0,0};//������������
motorparameters motor_r = {0,0,0,0,0,0,0,0,0};//�ҵ����������

s16  gyrx_offset;  //��������ƫֵ
s16  accy_offset;
s16  accz_offset;  //���ٶȼ���ƫֵ
s16  ad_gyrx;
s16  ad_accy;
s16  ad_accz;

float acc_th;
float gyr_xw;



float sd_p = 180;    //ֱ������P����    60        70        80    //�ڶ���˫CCD���Բ���120 //���������غ���Բ���170 ���Ĵε���200
float sd_d = 3.5;   //ֱ������D����    1.2       1.5       1.7                         2.2                       2.5           4.0

float sp_p = 1.2;  //�ٶȿ���P����//1.0                        //�ڶ���˫CCD���Բ���1.1
float sp_i = 1.0;  //�ٶȿ���I����//1.0                                             0.8
                                      
float dr_p = 0;
float dr_d = 0;

float sp_p_old;
float sp_i_old;
s16   zhangai_speed;
s16   jiansu_time;
s16   zhangai_time;
                                      


/***************************************************************
----------------------------------------------------------------                    
 ��ʱ����
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
 ϵͳ��ʼ������
----------------------------------------------------------------
****************************************************************/ 
void IO_INIT(void)
{
        gpio_init(PORTA,8 ,GPO,LOW);	//����CCD1�ṩʱ���ź�--OUT
	gpio_init(PORTA,10,GPO,LOW);
        gpio_init(PORTE,10,GPO,LOW);	//����CCD2�ṩʱ���ź�--OUT
	gpio_init(PORTE,12,GPO,LOW);
	
	//gpio_init(PORTA,14,GPI,HIGH);	//������������--IN
	gpio_init(PORTA,16,GPI,HIGH);
	
	gpio_init(PORTA,26,GPI,LOW);	//�����������ж�--IN
	gpio_init(PORTA,27,GPI,LOW);
	
	gpio_init(PORTC,5,GPO,HIGH);	//ָʾ��--OUT
	gpio_init(PORTC,7,GPO,HIGH);

	gpio_init(PORTE,0,GPI,LOW);     //���뿪������--IN
	gpio_init(PORTE,1,GPI,LOW);
	gpio_init(PORTE,2,GPI,LOW);	
	gpio_init(PORTE,4,GPI,LOW);
	gpio_init(PORTE,6,GPI,LOW);	
	gpio_init(PORTE,8,GPI,LOW);
	
/*	data = PTE28_IN;  //��ȡPTE28�����ƽ
	PTB16_OUT  = 1;   //PTB16����ߵ�ƽ	
*/

	
//   PORTC_PCR19=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//C19��������ΪGPIOģʽ���������ж�,���ж�
//   PORTD_PCR1=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//D1��������ΪGPIOģʽ���½����ж�,���ж�
//   PORTC_PCR11=PORT_PCR_MUX(1)|PORT_PCR_IRQC(1);//C11��������ΪGPIOģʽ�������ش���DMA����	
//   set_irq_priority (89,1);  //���ó��ж����ȼ�
//   set_irq_priority (90,2);  //�������ж����ȼ�
//   set_irq_priority (0,3);   //����DMA�ж����ȼ�

}

void ADC_INIT(void)
{
	adc_init(ADC0,SE12);//----PORTB2----����CCD1
        adc_init(ADC0,SE13);//----PORTB3----����CCD2
        
	adc_init(ADC1,SE8); //----PORTB0----���ٶȼ�������
	adc_init(ADC1,SE10);//----PORTB4
	adc_init(ADC1,SE12);//----PORTB6
/*	show[0]= ad_ave(ADC0,SE12,ADC_8bit,10);
	show[1]= ad_ave(ADC1,SE8,ADC_16bit,10);
	show[2]= ad_ave(ADC1,SE10,ADC_16bit,10);
	show[3]= ad_ave(ADC1,SE12,ADC_16bit,10);
*/
}

void PWM_INIT(void)					//���ڼĴ�����FTM.h�ļ���#define FTM_PRECISON 600u
{
	FTM_PWM_init(FTM0,CH1,10000,0);	//PTA4����10000Hz����PWM00��ռ�ձ�Ϊ0�����ڼĴ���Ϊ600
	FTM_PWM_init(FTM0,CH2,10000,0);	//PTA5����PWM01
	FTM_PWM_init(FTM0,CH3,10000,0);	//PTA6����PWM10
	FTM_PWM_init(FTM0,CH4,10000,0);	//PTA7����PWM11
        
//	FTM_PWM_Duty(FTM0,CH1,duty);	//�������ã�����CH1��ռ�ձȼĴ���Ϊduty
}
void UART_INIT(void)
{
	uart_init(UART1,9600);
	uart_sendStr (UART1,"\nsystem start ......\n");
        
//      uart_putchar (UART1,'F');     //����һ���ַ�
//      uart_sendStr (UART1,"STRING");//����һ���ַ���
//      UART_IRQ_EN(UART1);  //���� 1   �������ж�
//      UART_IRQ_DIS(UART1); //���� 1   �ؽ����ж�
}
void DMA_INIT(void)
{
	DMA_count_Init(DMA_CH0, PTA24 , 0x7FFF, DMA_falling);	//dma�Ʊ���������
	DMA_count_Init(DMA_CH2, PTD2 , 0x7FFF, DMA_falling);	//dma�Ʊ���������
        
//	count_bmq = DMA_count_get(DMA_CH0);	//----��������
//	DMA_count_reset(DMA_CH0);	        //----��������	
}
void PIT_INIT(void)
{
	pit_init_ms(PIT0,1);		//��ʼ��PIT0����ʱʱ��Ϊ�� 1ms
	pit_init_ms(PIT1,100);		//��ʼ��PIT1����ʱʱ��Ϊ�� 100ms
        
        disable_irq(68); //��pit0�ж�
        disable_irq(69); //��pit1�ж�
	
//	enable_irq(68); //��pit0�ж�
//      disable_irq(68);//��pit0�ж�
//	enable_irq(69); //��pit1�ж�
//      disable_irq(69);//��pit1�ж�
}

void SYS_INIT(void)
{
	IO_INIT();
	ADC_INIT();
	PWM_INIT();
	UART_INIT();
	DMA_INIT();
	PIT_INIT();
        
        set_irq_priority (63,1);//����1 �����ж�
        set_irq_priority (68,2);//pit0 ��ʱ�ж�
        set_irq_priority (69,3);//pit1 ��ʱ�ж�
        
    
}