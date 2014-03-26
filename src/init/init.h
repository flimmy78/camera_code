

#ifndef __INIT_H__
#define __INIT_H__


/****************ͷ�ļ�����*************/

#include "LED_CORE.H"
#include "nokia_5110.h"

//��������
#define KEY1_PORT    PORTE
#define KEY2_PORT    PORTE
#define KEY3_PORT    PORTE

#define KEY1_PIN    1
#define KEY2_PIN    3
#define KEY3_PIN    5


//���뿪����
#define SW8_PORT     PORTC

#define SW8_PIN1     8
#define SW8_PIN2     9
#define SW8_PIN3     10
#define SW8_PIN4     11
#define SW8_PIN5     12
#define SW8_PIN6     13
#define SW8_PIN7     14
#define SW8_PIN8     15


//AD��������
#define AD_PORT      PORTB

#define       AD_PIN1 0
#define       AD_PIN2 1
#define       AD_PIN3 2
#define       AD_PIN4 3
#define       AD_PIN5 4
#define       AD_PIN6 5
#define       AD_PIN7 6
#define       AD_PIN8 7


/****************��������*************/

void board_init();

//�����ʼ��


//��ʱ����ʼ��

void board_pit_init(void);


#endif