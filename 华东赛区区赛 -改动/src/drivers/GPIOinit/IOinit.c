#include  "include.h"

void IO_init(void)
{
//   PORTC_PCR19=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//C19引脚设置为GPIO模式，下升沿中断,场中断
//   PORTD_PCR1=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10);//D1引脚设置为GPIO模式，下降沿中断,行中断
//   PORTC_PCR11=PORT_PCR_MUX(1)|PORT_PCR_IRQC(1);//C11引脚设置为GPIO模式，上升沿触发DMA请求
   
//   DMA_count_Init(DMA_CH4, PTA13 , 0x7FFF, DMA_rising_up);//dma计编码器脉冲
	gpio_init(PORTD,0,GPO,HIGH);
	gpio_init(PORTD,1,GPO,HIGH);
/*   gpio_init(PORTD,2,GPI,HIGH);
   gpio_init(PORTD,3,GPI,HIGH);
   gpio_init(PORTD,4,GPI,HIGH);
   gpio_init(PORTD,5,GPI,HIGH);
   gpio_init(PORTD,6,GPI,HIGH);
   gpio_init(PORTD,7,GPI,HIGH);//D0~D7 作为拨码开关的入口

   gpio_init(PORTB,0,GPO,LOW);
   gpio_init(PORTB,1,GPO,LOW);
   gpio_init(PORTB,2,GPO,LOW);
   gpio_init(PORTB,3,GPO,LOW);
   gpio_init(PORTB,4,GPO,LOW);
   gpio_init(PORTB,5,GPO,LOW);
   gpio_init(PORTB,6,GPO,LOW);
   gpio_init(PORTB,7,GPO,LOW);//B0~B7 作为TLC5510转换后的灰度值的入口

*/

//   set_irq_priority (89,1);  //设置场中断优先级
//   set_irq_priority (90,2);  //设置行中断优先级
//   set_irq_priority (0,3);   //设置DMA中断优先级

}