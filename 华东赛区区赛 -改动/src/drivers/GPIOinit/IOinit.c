#include  "include.h"

void IO_init(void)
{
//   PORTC_PCR19=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//C19��������ΪGPIOģʽ���������ж�,���ж�
//   PORTD_PCR1=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//D1��������ΪGPIOģʽ���½����ж�,���ж�
//   PORTC_PCR11=PORT_PCR_MUX(1)|PORT_PCR_IRQC(1);//C11��������ΪGPIOģʽ�������ش���DMA����
   
//   DMA_count_Init(DMA_CH4, PTA13 , 0x7FFF, DMA_rising_up);//dma�Ʊ���������
	gpio_init(PORTD,0,GPO,HIGH);
	gpio_init(PORTD,1,GPO,HIGH);
/*   gpio_init(PORTD,2,GPI,HIGH);
   gpio_init(PORTD,3,GPI,HIGH);
   gpio_init(PORTD,4,GPI,HIGH);
   gpio_init(PORTD,5,GPI,HIGH);
   gpio_init(PORTD,6,GPI,HIGH);
   gpio_init(PORTD,7,GPI,HIGH);//D0~D7 ��Ϊ���뿪�ص����

   gpio_init(PORTB,0,GPO,LOW);
   gpio_init(PORTB,1,GPO,LOW);
   gpio_init(PORTB,2,GPO,LOW);
   gpio_init(PORTB,3,GPO,LOW);
   gpio_init(PORTB,4,GPO,LOW);
   gpio_init(PORTB,5,GPO,LOW);
   gpio_init(PORTB,6,GPO,LOW);
   gpio_init(PORTB,7,GPO,LOW);//B0~B7 ��ΪTLC5510ת����ĻҶ�ֵ�����

*/

//   set_irq_priority (89,1);  //���ó��ж����ȼ�
//   set_irq_priority (90,2);  //�������ж����ȼ�
//   set_irq_priority (0,3);   //����DMA�ж����ȼ�

}