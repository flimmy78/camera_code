/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：UART串口发送实验
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	


#include "common.h"
#include "include.h"
#include "LED_CORE.h"
#include "init.h"


void main()
{
  int i= 0;
	board_init();
        while(1)
        {
         LCD_init();
         while(1)
          LCD_write_english_string(0,i++,"hello000000000000000000000000000000000000");
         
        }
}