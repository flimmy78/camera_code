
#include "include.h"
#include "assert.h"

volatile struct UART_MemMap *UARTx[6] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //�������ָ�����鱣�� UARTx �ĵ�ַ

unsigned char buf[18]=      //��������ͼ����ʾ��һ֡���ݶ���
{
  0,     // ���ȸ��ֽڣ�����Ϊ0
  16,    // 16
  0,     // �����ֽڣ�����Ϊ0
  0,     // �����ֽڣ�����Ϊ0
  0,     // �����ֽڣ�����Ϊ0
  0,     // �����ֽڣ�����Ϊ0
  0,0,   // ͨ��1�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096)
  0,0,   // ͨ��2�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096)
  0,0,   // ͨ��3�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096)
  0,0,   // ͨ��4�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096) 
  0,0,   // ͨ��5�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096) 
  0,0,   // ͨ��6�ĸߡ����ֽ����ݣ� ���û�����(�ֵĴ�С������4096)
}; 


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_init
*  ����˵������ʼ�����ڣ����ò�����
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            baud        �����ʣ���9600��19200��56000��115200��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ��������޸�
*************************************************************************/
void uart_init (UARTn uratn, u32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    u32 sysclk;     //ʱ��

    /* ���� UART���ܵ� GPIO �ӿ� ����ʱ�� */
    switch(uratn)
    {
    case UART0:
        if(UART0_RX == PTA1)
            PORTA_PCR1 = PORT_PCR_MUX(0x2);      //��PTA1��ʹ��UART0_RXD
        else if(UART0_RX == PTA15)
            PORTA_PCR15 = PORT_PCR_MUX(0x3);     //��PTA15��ʹ��UART0_RXD
        else if(UART0_RX == PTB16)
            PORTB_PCR16 = PORT_PCR_MUX(0x3);     //��PTB16��ʹ��UART0_RXD
        else if(UART0_RX == PTD6)
            PORTD_PCR6 = PORT_PCR_MUX(0x3);      //��PTD6��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);   //���ùܽ�����

        if(UART0_TX == PTA2)
            PORTA_PCR2 = PORT_PCR_MUX(0x2);     //��PTA2��ʹ��UART0_RXD
        else if(UART0_TX == PTA14)
            PORTA_PCR14 = PORT_PCR_MUX(0x3);     //��PTA14��ʹ��UART0_RXD
        else if(UART0_TX == PTB17)
            PORTB_PCR17 = PORT_PCR_MUX(0x3);     //��PTB17��ʹ��UART0_RXD
        else if(UART0_TX == PTD7)
            PORTD_PCR7 = PORT_PCR_MUX(0x3);     //��PTD7��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����


        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UARTn ʱ��
        break;

    case UART1:
        if(UART1_RX == PTC3)
            PORTC_PCR3 = PORT_PCR_MUX(0x3);     //��PTC3��ʹ��UART1_RXD
        else if(UART1_RX == PTE1)
            PORTE_PCR1 = PORT_PCR_MUX(0x3);     //��PTE1��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        if(UART1_TX == PTC4)
            PORTC_PCR4 = PORT_PCR_MUX(0x3);     //��PTC4��ʹ��UART1_RXD
        else if(UART1_TX == PTE0)
            PORTE_PCR0 = PORT_PCR_MUX(0x3);     //��PTE0��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

    case UART2:
        PORTD_PCR3 = PORT_PCR_MUX(0x3);         //��PTD3��ʹ��UART2_TXD����
        PORTD_PCR2 = PORT_PCR_MUX(0x3);         //��PTD2��ʹ��UART2_RXD
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

    case UART3:
        if(UART3_RX == PTB10)
            PORTB_PCR10 = PORT_PCR_MUX(0x3);     //��PTB10��ʹ��UART3_RXD
        else if(UART3_RX == PTC16)
            PORTC_PCR16 = PORT_PCR_MUX(0x3);     //��PTC16��ʹ��UART3_RXD
        else if(UART3_RX == PTE5)
            PORTE_PCR5 = PORT_PCR_MUX(0x3);      //��PTE5��ʹ��UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART3_TX == PTB11)
            PORTB_PCR11 = PORT_PCR_MUX(0x3);     //��PTB11��ʹ��UART3_RXD
        else if(UART3_TX == PTC17)
            PORTC_PCR17 = PORT_PCR_MUX(0x3);     //��PTC17��ʹ��UART3_RXD
        else if(UART3_TX == PTE4)
            PORTE_PCR4 = PORT_PCR_MUX(0x3);     //��PTE4��ʹ��UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

    case UART4:
        if(UART4_RX == PTC14)
            PORTC_PCR14 = PORT_PCR_MUX(0x3);     //��PTC14��ʹ��UART4_RXD
        else if(UART4_RX == PTE25)
            PORTE_PCR25 = PORT_PCR_MUX(0x3);     //��PTE25��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART4_TX == PTC15)
            PORTC_PCR15 = PORT_PCR_MUX(0x3);     //��PTC15��ʹ��UART4_RXD
        else if(UART4_TX == PTE24)
            PORTE_PCR24 = PORT_PCR_MUX(0x3);     //��PTE24��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

    case UART5:
        if(UART5_RX == PTD8)
            PORTD_PCR8 = PORT_PCR_MUX(0x3);     //��PTD8��ʹ��UART5_RXD
        else if(UART5_RX == PTE9)
            PORTE_PCR9 = PORT_PCR_MUX(0x3);     //��PTE9��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART5_TX == PTD9)
            PORTD_PCR9 = PORT_PCR_MUX(0x3);     //��PTD9��ʹ��UART5_RXD
        else if(UART5_TX == PTE8)
            PORTE_PCR8 = PORT_PCR_MUX(0x3);     //��PTE8��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
    default:
        break;
    }


    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UART_C1_REG(UARTx[uratn]) = 0;	// ȫ��ֱ��ʹ��Ĭ�����þ��У�����ֱ����0

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ������ʱ��
    if ((uratn == UART0) | (uratn == UART1))
        sysclk = core_clk_khz * 1000;            //�ں�ʱ��
    else
        sysclk = bus_clk_khz * 1000;  //����ʱ��

    //���� UART ����ͨѶ�����ʡ�ͨ������ UART ģ��Ĳ����ʼĴ���
    sbr = (u16)(sysclk / (baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(UARTx[uratn]) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(UARTx[uratn]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UARTx[uratn]) = (u8)(sbr & UART_BDL_SBR_MASK);

    //brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
    brfa = (((sysclk << 5) / (baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(UARTx[uratn]) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(UARTx[uratn]) = temp |  UART_C4_BRFA(brfa);

    /* �����ͺͽ��� */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );

    //�����Ƿ�������պͷ����жϡ�ͨ������ UART ģ��� C2 �Ĵ�����
    //RIE �� TIE λʵ�֡����ʹ���жϣ���������ʵ���жϷ������
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_getchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
char uart_getchar (UARTn uratn)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn]);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendchar (UARTn uratn, char *ch)
{
    u32 i = 0;

    while(++i < 0xffffff)                                         //ʱ������
    {
        if(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK)         //��ѯ�Ƿ���ܵ�����
        {
            *ch  =   UART_D_REG(UARTx[uratn]);                    //���ܵ�8λ������
            return  1;                                            //���� 1 ��ʾ���ճɹ�
        }
    }

    *ch = 0;                                                     //���ղ�����Ӧ������˽�����
    return 0;                                                    //����0��ʾ����ʧ��
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendstr
*  ����˵��������ʱ��ȴ����ڽ����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendstr (UARTn uratn, char *str)
{
    u32 i = 0;
    while(uart_pendchar(uratn, str + i++));

    return (i <= 1 ? 0 : 1);
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_putchar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ����̣�printf������⺯��
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTx[uratn]) & UART_S1_TDRE_MASK));
    //��������
    UART_D_REG(UARTx[uratn]) = (u8)ch;
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_query
*  ����˵������ѯ�Ƿ���ܵ�һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ�1           ���յ�һ���ֽ���
*            0           û�н��յ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
int uart_query (UARTn uratn)
{
    return (UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�������� ����ָ��len���ֽڳ����ַ��� ������ NULL Ҳ�ᷢ�ͣ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            buff        ���ͻ�����
*            len         ���ͳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendN (UARTn uratn, uint8 *buff, uint16 len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        uart_putchar(uratn, buff[i]);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendStr
*  ����˵�������з����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            str         �ַ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendStr (UARTn uratn, const u8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_EN
*  ����˵���������ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_EN(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) |= UART_C2_RIE_MASK;    //����UART�����ж�
    enable_irq((uratn << 1) + 45);			        //���������ŵ�IRQ�ж�
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_DIS
*  ����˵�����ش��ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_DIS(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) &= ~UART_C2_RIE_MASK;   //��ֹUART�����ж�
    disable_irq((uratn << 1) + 45);			        //�ؽ������ŵ�IRQ�ж�
}



/***********************************************************
------------------------------------------------------------ 
�������ƣ� void uart_send_3data(signed data)
�������ܣ� sci0ģ�鷢��һ��3λ����
��ڲ����� data ����Ҫ���͵�����
���ڲ����� �� 
------------------------------------------------------------
************************************************************/
void uart_send_3data(signed int data)
{
  unsigned char bai = 0, shi = 0, ge = 0;
  if(data >= 0) 
  {       
    bai = data /100;
    shi = (data -100*bai)/10;
    ge = data -100*bai- 10*shi;
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
  } 
  else 
  {
    data = -data;
    bai = data /100;
    shi = (data -100*bai)/10;
    ge = data -100*bai- 10*shi;
    uart_putchar (UART1,'-');
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
  }

}

/***********************************************************
------------------------------------------------------------ 
�������ƣ� void uart_send_4data(signed data)
�������ܣ� sci0ģ�鷢��һ��4λ����
��ڲ����� data ����Ҫ���͵�����
���ڲ����� �� 
------------------------------------------------------------
************************************************************/
void uart_send_4data(signed int data)
{
  unsigned char qian =0, bai = 0, shi = 0, ge = 0;
  if(data >= 0) 
  { 
    qian = data/1000;      
    bai = (data-1000*qian)/100;
    shi = (data-1000*qian-100*bai)/10;
    ge = data-1000*qian-100*bai- 10*shi;
    uart_putchar (UART1,qian+48);
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
  } 
  else 
  {
    data = -data;
    qian = data/1000;      
    bai = (data-1000*qian)/100;
    shi = (data-1000*qian-100*bai)/10;
    ge = data-1000*qian-100*bai- 10*shi;
    uart_putchar (UART1,'-');
    uart_putchar (UART1,qian+48);
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
  }
}
/*************************************************************************
*  �������ƣ�uart_send_5data(u16 data)
*  �޸�ʱ�䣺2012-1-20
*************************************************************************/
void uart_send_5data(u16 data)
{
	u8 dis[5];
	
	dis[0] = (char)(data/10000%10);
	dis[1] = (char)(data/1000%10);
	dis[2] = (char)(data/100%10);
	dis[3] = (char)(data/10%10);
	dis[4] = (char)(data%10);
        
        uart_putchar (UART1,dis[0]+48); 
        uart_putchar (UART1,dis[1]+48); 
        uart_putchar (UART1,dis[2]+48); 
        uart_putchar (UART1,dis[3]+48); 
        uart_putchar (UART1,dis[4]+48); 
}

/***********************************************************
------------------------------------------------------------
�������ƣ� void uart_send_2_1data(float data)
�������ܣ� ����һ��2.1��С��
��ڲ���:  data����Ҫ��ʾ��С�� 
           
���ڲ����� ��
------------------------------------------------------------
************************************************************/
void uart_send_2_1data(float data)
{
  u8  shi, ge, shifen;
  signed int temp;
  temp = (data*10);
  if(temp >= 0)
  {
    shi = temp /100;
    ge = (temp -100*shi)/10;
    shifen = temp -100*shi- 10*ge;
    uart_putchar (UART1,' '); 
    uart_putchar (UART1,48+shi); 
    uart_putchar (UART1,48+ge); 
    uart_putchar (UART1,'.'); 
    uart_putchar (UART1,48+shifen); 
  }
  else
  {
    temp = -temp;
    shi = temp /100;
    ge = (temp -100*shi)/10;
    shifen = temp -100*shi- 10*ge;
    uart_putchar (UART1,'-'); 
    uart_putchar (UART1,48+shi); 
    uart_putchar (UART1,48+ge); 
    uart_putchar (UART1,'.'); 
    uart_putchar (UART1,48+shifen);
  }
}

/***********************************************************
------------------------------------------------------------ 
�������ƣ� void uart_send_4_4data(float data)
�������ܣ� sci0ģ�鷢��һ��4.4С��
��ڲ����� data ����Ҫ���͵�С��
���ڲ����� �� 
------------------------------------------------------------
************************************************************/
void uart_send_4_4data(float data)
{
  u8  qian, bai, shi, ge, shifen, baifen, qianfen, wanfen;
  signed int temp1, temp2;
  if(data >= 0)
  { 
    temp1 = (signed int)data;
    temp2 = (data - temp1)*10000;
     
    qian = temp1/1000;      
    bai = (temp1-1000*qian)/100;
    shi = (temp1-1000*qian-100*bai)/10;
    ge = temp1-1000*qian-100*bai- 10*shi;
    
    shifen = temp2/1000;      
    baifen = (temp2-1000*shifen)/100;
    qianfen = (temp2-1000*shifen-100*baifen)/10;
    wanfen = temp2-1000*shifen-100*baifen- 10*qianfen;
    
    uart_putchar (UART1,qian+48); 
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
    uart_putchar (UART1,'.');
    uart_putchar (UART1,shifen+48);
    uart_putchar (UART1,baifen+48);
    uart_putchar (UART1,qianfen+48);
    uart_putchar (UART1,wanfen+48);   
    
  } 
  else
  {
    data = -data;
    temp1 = (signed int)data;
    temp2 = (data - temp1)*10000;
    
    qian = temp1/1000;      
    bai = (temp1-1000*qian)/100;
    shi = (temp1-1000*qian-100*bai)/10;
    ge = temp1-1000*qian-100*bai- 10*shi;
    
    shifen = temp2/1000;      
    baifen = (temp2-1000*shifen)/100;
    qianfen = (temp2-1000*shifen-100*baifen)/10;
    wanfen = temp2-1000*shifen-100*baifen- 10*qianfen;
    
    uart_putchar (UART1,'-');
    uart_putchar (UART1,qian+48); 
    uart_putchar (UART1,bai+48);
    uart_putchar (UART1,shi+48);
    uart_putchar (UART1,ge+48);
    uart_putchar (UART1,'.');
    uart_putchar (UART1,shifen+48);
    uart_putchar (UART1,baifen+48);
    uart_putchar (UART1,qianfen+48);
    uart_putchar (UART1,wanfen+48); 
  }  
}


/*******************������ٶ���������λ������****************************
-------------------------------------------------------------------------- 
�������ƣ� SendHex(unsigned char data)
           sci0_send_buf(unsigned char *data)
           buf_update(void)
�������ܣ� ��������ͼ����ʾ
��ڲ����� 
���ڲ�����  
ʹ��˵���� (ad�Ѿ���ʼ��)
           buf_update();      //���»�����������֡���ݣ�
           sci0_send_buf(buf);//�������ݣ�����֡���ݣ�
           delay_ms(5);       //��ʱ5ms
--------------------------------------------------------------------------
**************************************************************************/
void SendHex(unsigned char data)
 {
    unsigned char	 temp;
    temp = data >> 4;
    if (temp >= 10)
    {
    	uart_putchar (UART1,temp - 10 + 'A');
		}
    else
    {
    	uart_putchar (UART1,temp + '0');
		}
    temp = data & 0x0F;
    if (temp >= 10)
    {
        uart_putchar (UART1,temp - 10 + 'A');
    }
    else
    {
        uart_putchar (UART1,temp + '0');
    }
}


 
void uart_send_buf(unsigned char *data)
{
    int len;
    unsigned char	lrc=0;	    
    uart_putchar (UART1,'*');       // ����֡ͷ��һ���ֽ�                             
    len = (int)(data[0]<<8) | (int)(data[1]) ;
    data += 2;                 // ����ָ��
    uart_putchar (UART1,'L');       // ����֡���ͣ��������ֽ�
    uart_putchar (UART1,'D');
    while(len--)               // �������ݵ�ASCII�룬�������ֽں�CCD����
    {
        SendHex(*data);	
        lrc += *data++;
    } 
    lrc = 0-lrc;              // ����CRC������Ϊ����ֵ
    SendHex(lrc);             // ����CRCУ��ASCII
    uart_putchar (UART1,'#');      // ����֡β��һ���ֽ�
}
 

void buf_update(void)                   
{ 
  buf[6] = (((signed int)acc_th+125)&0x0f00)>>8;   //ͨ��1����Ϊacc_th   
  buf[7] = ((signed int)acc_th+125)&0x00ff;
  
  buf[8] = (((signed int)car.th+125)&0x0f00)>>8;   //ͨ��2����Ϊcar_th   
  buf[9] = ((signed int)car.th+125)&0x00ff;      
  
  buf[10] = (((signed int)gyr_xw+125)&0x0f00)>>8;  //ͨ��3����Ϊgyr_w   
  buf[11] = ((signed int)gyr_xw+125)&0x00ff;

  buf[12] = (((signed int)car.w+125)&0x0f00)>>8;   //ͨ��4����Ϊcar_w   
  buf[13] = ((signed int)car.w+125)&0x00ff;    
  /*
  buf[6] = (ATD0DR2&0x0f00) >> 8;      //����ͨ��1���ݣ���������ad2,(������X)
  buf[7] = ATD0DR2 & 0x00ff;
  
  buf[8] = (ATD0DR1&0x0f00) >> 8;      //����ͨ��2���ݣ���������ad1,(������Y)
  buf[9] = ATD0DR1 & 0x00ff; 
  
  buf[16] = (ATD0DR0&0x0f00) >> 8;     //����ͨ��6���ݣ���������ad0,(���ٶ�Z)
  buf[17] = ATD0DR0 & 0x00ff;
  */ 
}

  
/************************* ����CCD��λ������************************
------------------------------------------------------------------
1,���ڳ�ʼ��
 
3,��������: SendImageData(pixel);
------------------------------------------------------------------
******************************************************************/
void SendImageData(unsigned char * ImageData) 
{
    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    uart_putchar (UART1,'*');
    uart_putchar (UART1,'L');
    uart_putchar (UART1,'D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) 
    {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    uart_putchar (UART1,'#');
}