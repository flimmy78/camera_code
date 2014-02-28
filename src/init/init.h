

#ifndef __INIT_H__
#define __INIT_H__


/****************头文件包含*************/

#include "LED_CORE.H"
#include "nokia_5110.h"




/****************函数声明*************/

void board_init();

//电机初始化
void motor_init(void);

//定时器初始化

void board_pit_init(void);


#endif