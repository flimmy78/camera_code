

#ifndef __INIT_H__
#define __INIT_H__


/****************头文件包含*************/

#include "LED_CORE.H"
#include "nokia_5110.h"

//按键配置
#define KEY1_PORT    PORTE
#define KEY2_PORT    PORTE
#define KEY3_PORT    PORTE

#define KEY1_PIN    1
#define KEY2_PIN    3
#define KEY3_PIN    5


//拨码开配置
#define SW8_PORT     PORTC

#define SW8_PIN1     8
#define SW8_PIN2     9
#define SW8_PIN3     10
#define SW8_PIN4     11
#define SW8_PIN5     12
#define SW8_PIN6     13
#define SW8_PIN7     14
#define SW8_PIN8     15


//AD引脚配置
#define AD_PORT      PORTB

#define       AD_PIN1 0
#define       AD_PIN2 1
#define       AD_PIN3 2
#define       AD_PIN4 3
#define       AD_PIN5 4
#define       AD_PIN6 5
#define       AD_PIN7 6
#define       AD_PIN8 7


/****************函数声明*************/

void board_init();

//电机初始化


//定时器初始化

void board_pit_init(void);


#endif