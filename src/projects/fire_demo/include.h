#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "gpio.h"       //IO口操作
#include  "uart.h"      //串口
#include  "lptmr.h"     //低功耗定时器(延时)
#include  "adc.h"			//模数转换
#include  "dac.h"			//数模转换
#include  "delay.h"		//延时函数
#include  "exti.h"		//exti外部中断
#include  "LED.h"			//LED模块
#include  "mcg.h"
#include  "wdog.h"		//看门狗
#include  "PIT.h"			//PIT定时器中断模块



#endif  //__INCLUDE_H__
