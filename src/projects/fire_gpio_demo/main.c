/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��main.c
 * ����         ��UART���ڷ���ʵ��
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
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