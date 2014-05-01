
#include "include.h"
#include "assert.h"

volatile struct UART_MemMap *UARTx[6] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //定义五个指针数组保存 UARTx 的地址

unsigned char buf[18]=      //串口数据图形显示的一帧数据定义
{
  0,     // 长度高字节，设置为0
  16,    // 16
  0,     // 保留字节，设置为0
  0,     // 保留字节，设置为0
  0,     // 保留字节，设置为0
  0,     // 保留字节，设置为0
  0,0,   // 通道1的高、低字节数据， 由用户填入(字的大小不超过4096)
  0,0,   // 通道2的高、低字节数据， 由用户填入(字的大小不超过4096)
  0,0,   // 通道3的高、低字节数据， 由用户填入(字的大小不超过4096)
  0,0,   // 通道4的高、低字节数据， 由用户填入(字的大小不超过4096) 
  0,0,   // 通道5的高、低字节数据， 由用户填入(字的大小不超过4096) 
  0,0,   // 通道6的高、低字节数据， 由用户填入(字的大小不超过4096)
}; 


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_init
*  功能说明：初始化串口，设置波特率
*  参数说明：UARTn       模块号（UART0~UART5）
*            baud        波特率，如9600、19200、56000、115200等
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方例程上修改
*************************************************************************/
void uart_init (UARTn uratn, u32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    u32 sysclk;     //时钟

    /* 配置 UART功能的 GPIO 接口 开启时钟 */
    switch(uratn)
    {
    case UART0:
        if(UART0_RX == PTA1)
            PORTA_PCR1 = PORT_PCR_MUX(0x2);      //在PTA1上使能UART0_RXD
        else if(UART0_RX == PTA15)
            PORTA_PCR15 = PORT_PCR_MUX(0x3);     //在PTA15上使能UART0_RXD
        else if(UART0_RX == PTB16)
            PORTB_PCR16 = PORT_PCR_MUX(0x3);     //在PTB16上使能UART0_RXD
        else if(UART0_RX == PTD6)
            PORTD_PCR6 = PORT_PCR_MUX(0x3);      //在PTD6上使能UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);   //设置管脚有误？

        if(UART0_TX == PTA2)
            PORTA_PCR2 = PORT_PCR_MUX(0x2);     //在PTA2上使能UART0_RXD
        else if(UART0_TX == PTA14)
            PORTA_PCR14 = PORT_PCR_MUX(0x3);     //在PTA14上使能UART0_RXD
        else if(UART0_TX == PTB17)
            PORTB_PCR17 = PORT_PCR_MUX(0x3);     //在PTB17上使能UART0_RXD
        else if(UART0_TX == PTD7)
            PORTD_PCR7 = PORT_PCR_MUX(0x3);     //在PTD7上使能UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);  //设置管脚有误？


        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UARTn 时钟
        break;

    case UART1:
        if(UART1_RX == PTC3)
            PORTC_PCR3 = PORT_PCR_MUX(0x3);     //在PTC3上使能UART1_RXD
        else if(UART1_RX == PTE1)
            PORTE_PCR1 = PORT_PCR_MUX(0x3);     //在PTE1上使能UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //设置管脚有误？

        if(UART1_TX == PTC4)
            PORTC_PCR4 = PORT_PCR_MUX(0x3);     //在PTC4上使能UART1_RXD
        else if(UART1_TX == PTE0)
            PORTE_PCR0 = PORT_PCR_MUX(0x3);     //在PTE0上使能UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //设置管脚有误？

        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

    case UART2:
        PORTD_PCR3 = PORT_PCR_MUX(0x3);         //在PTD3上使能UART2_TXD功能
        PORTD_PCR2 = PORT_PCR_MUX(0x3);         //在PTD2上使能UART2_RXD
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

    case UART3:
        if(UART3_RX == PTB10)
            PORTB_PCR10 = PORT_PCR_MUX(0x3);     //在PTB10上使能UART3_RXD
        else if(UART3_RX == PTC16)
            PORTC_PCR16 = PORT_PCR_MUX(0x3);     //在PTC16上使能UART3_RXD
        else if(UART3_RX == PTE5)
            PORTE_PCR5 = PORT_PCR_MUX(0x3);      //在PTE5上使能UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        if(UART3_TX == PTB11)
            PORTB_PCR11 = PORT_PCR_MUX(0x3);     //在PTB11上使能UART3_RXD
        else if(UART3_TX == PTC17)
            PORTC_PCR17 = PORT_PCR_MUX(0x3);     //在PTC17上使能UART3_RXD
        else if(UART3_TX == PTE4)
            PORTE_PCR4 = PORT_PCR_MUX(0x3);     //在PTE4上使能UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

    case UART4:
        if(UART4_RX == PTC14)
            PORTC_PCR14 = PORT_PCR_MUX(0x3);     //在PTC14上使能UART4_RXD
        else if(UART4_RX == PTE25)
            PORTE_PCR25 = PORT_PCR_MUX(0x3);     //在PTE25上使能UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        if(UART4_TX == PTC15)
            PORTC_PCR15 = PORT_PCR_MUX(0x3);     //在PTC15上使能UART4_RXD
        else if(UART4_TX == PTE24)
            PORTE_PCR24 = PORT_PCR_MUX(0x3);     //在PTE24上使能UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

    case UART5:
        if(UART5_RX == PTD8)
            PORTD_PCR8 = PORT_PCR_MUX(0x3);     //在PTD8上使能UART5_RXD
        else if(UART5_RX == PTE9)
            PORTE_PCR9 = PORT_PCR_MUX(0x3);     //在PTE9上使能UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        if(UART5_TX == PTD9)
            PORTD_PCR9 = PORT_PCR_MUX(0x3);     //在PTD9上使能UART5_RXD
        else if(UART5_TX == PTE8)
            PORTE_PCR8 = PORT_PCR_MUX(0x3);     //在PTE8上使能UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
    default:
        break;
    }


    //设置的时候，应该禁止发送接受
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UART_C1_REG(UARTx[uratn]) = 0;	// 全部直接使用默认设置就行，所以直接清0

    //计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟
    if ((uratn == UART0) | (uratn == UART1))
        sysclk = core_clk_khz * 1000;            //内核时钟
    else
        sysclk = bus_clk_khz * 1000;  //外设时钟

    //设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器
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

    /* 允许发送和接收 */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );

    //设置是否允许接收和发送中断。通过设置 UART 模块的 C2 寄存器的
    //RIE 和 TIE 位实现。如果使能中断，必须首先实现中断服务程序；
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_getchar
*  功能说明：无限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
char uart_getchar (UARTn uratn)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn]);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_pendchar
*  功能说明：有限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
char uart_pendchar (UARTn uratn, char *ch)
{
    u32 i = 0;

    while(++i < 0xffffff)                                         //时间限制
    {
        if(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK)         //查询是否接受到数据
        {
            *ch  =   UART_D_REG(UARTx[uratn]);                    //接受到8位的数据
            return  1;                                            //返回 1 表示接收成功
        }
    }

    *ch = 0;                                                     //接收不到，应该清空了接收区
    return 0;                                                    //返回0表示接收失败
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_pendstr
*  功能说明：有限时间等待串口接受字符串
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
char uart_pendstr (UARTn uratn, char *str)
{
    u32 i = 0;
    while(uart_pendchar(uratn, str + i++));

    return (i <= 1 ? 0 : 1);
}



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_putchar
*  功能说明：串口发送一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程，printf会调用这函数
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //等待发送缓冲区空
    while(!(UART_S1_REG(UARTx[uratn]) & UART_S1_TDRE_MASK));
    //发送数据
    UART_D_REG(UARTx[uratn]) = (u8)ch;
}



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_query
*  功能说明：查询是否接受到一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：1           接收到一个字节了
*            0           没有接收到
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
int uart_query (UARTn uratn)
{
    return (UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendN
*  功能说明：串行 发送指定len个字节长度字符串 （包括 NULL 也会发送）
*  参数说明：UARTn       模块号（UART0~UART5）
*            buff        发送缓冲区
*            len         发送长度
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
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
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendStr
*  功能说明：串行发送字符串
*  参数说明：UARTn       模块号（UART0~UART5）
*            str         字符串
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_sendStr (UARTn uratn, const u8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_irq_EN
*  功能说明：开串口接收中断
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_irq_EN(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) |= UART_C2_RIE_MASK;    //开放UART接收中断
    enable_irq((uratn << 1) + 45);			        //开接收引脚的IRQ中断
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_irq_DIS
*  功能说明：关串口接收中断
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_irq_DIS(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) &= ~UART_C2_RIE_MASK;   //禁止UART接收中断
    disable_irq((uratn << 1) + 45);			        //关接收引脚的IRQ中断
}



/***********************************************************
------------------------------------------------------------ 
函数名称： void uart_send_3data(signed data)
函数功能： sci0模块发送一个3位整数
入口参数： data ：需要发送的整数
出口参数： 无 
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
函数名称： void uart_send_4data(signed data)
函数功能： sci0模块发送一个4位整数
入口参数： data ：需要发送的整数
出口参数： 无 
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
*  函数名称：uart_send_5data(u16 data)
*  修改时间：2012-1-20
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
函数名称： void uart_send_2_1data(float data)
函数功能： 发送一个2.1的小数
入口参数:  data：需要显示的小数 
           
出口参数： 无
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
函数名称： void uart_send_4_4data(float data)
函数功能： sci0模块发送一个4.4小数
入口参数： data ：需要发送的小数
出口参数： 无 
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


/*******************蓝宙加速度陀螺仪上位机程序****************************
-------------------------------------------------------------------------- 
函数名称： SendHex(unsigned char data)
           sci0_send_buf(unsigned char *data)
           buf_update(void)
函数功能： 串口数据图形显示
入口参数： 
出口参数：  
使用说明： (ad已经初始化)
           buf_update();      //更新缓冲区（更新帧数据）
           sci0_send_buf(buf);//发送数据（发送帧数据）
           delay_ms(5);       //延时5ms
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
    uart_putchar (UART1,'*');       // 发送帧头，一个字节                             
    len = (int)(data[0]<<8) | (int)(data[1]) ;
    data += 2;                 // 调整指针
    uart_putchar (UART1,'L');       // 发送帧类型，共两个字节
    uart_putchar (UART1,'D');
    while(len--)               // 发送数据的ASCII码，含保留字节和CCD数据
    {
        SendHex(*data);	
        lrc += *data++;
    } 
    lrc = 0-lrc;              // 计算CRC，可以为任意值
    SendHex(lrc);             // 发送CRC校验ASCII
    uart_putchar (UART1,'#');      // 发送帧尾，一个字节
}
 

void buf_update(void)                   
{ 
  buf[6] = (((signed int)acc_th+125)&0x0f00)>>8;   //通道1数据为acc_th   
  buf[7] = ((signed int)acc_th+125)&0x00ff;
  
  buf[8] = (((signed int)car.th+125)&0x0f00)>>8;   //通道2数据为car_th   
  buf[9] = ((signed int)car.th+125)&0x00ff;      
  
  buf[10] = (((signed int)gyr_xw+125)&0x0f00)>>8;  //通道3数据为gyr_w   
  buf[11] = ((signed int)gyr_xw+125)&0x00ff;

  buf[12] = (((signed int)car.w+125)&0x0f00)>>8;   //通道4数据为car_w   
  buf[13] = ((signed int)car.w+125)&0x00ff;    
  /*
  buf[6] = (ATD0DR2&0x0f00) >> 8;      //更新通道1数据，数据来自ad2,(陀螺仪X)
  buf[7] = ATD0DR2 & 0x00ff;
  
  buf[8] = (ATD0DR1&0x0f00) >> 8;      //更新通道2数据，数据来自ad1,(陀螺仪Y)
  buf[9] = ATD0DR1 & 0x00ff; 
  
  buf[16] = (ATD0DR0&0x0f00) >> 8;     //更新通道6数据，数据来自ad0,(加速度Z)
  buf[17] = ATD0DR0 & 0x00ff;
  */ 
}

  
/************************* 蓝宙CCD上位机程序************************
------------------------------------------------------------------
1,串口初始化
 
3,发送数据: SendImageData(pixel);
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