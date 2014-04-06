/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //������������������ȷ����ȫ

/*! 
 *  @brief      main����
 *  @since      v5.0
 *  @note       Ұ��Flahs��дʵ��
                K60�ĳ���Flash��ÿ������ 2K
                k60N512����512K�ĳ���Flash �� ����������Ϊ256 ��������ȡֵ��ΧΪ 0~255
                k60X256����256K�ĳ���Flash �� ����������Ϊ128 ��������ȡֵ��ΧΪ 0~127
                
                �ڴ�����������ݣ�
                һ�ζ�ȡ32λ������Ϊ��0x12345678
                һ�ζ�ȡ16λ������Ϊ��0x5678
                һ�ζ�ȡ8λ������Ϊ��0x78

                �Ӵ��ڵ�������ݿ��Կ��� ARM ��С��ģʽ��
                
                ע�⣺Flash���ܹ���Ƶ������д����������Ӱ������
 */
void  main(void)
{
    uint32  data32;
    uint16  data16;
    uint8   data8;

    flash_init();                                       //��ʼ��flash

    flash_erase_sector(SECTOR_NUM);                     //��������
                                                        //д��flash����ǰ����Ҫ�Ȳ�����Ӧ������(��Ȼ���ݻ���)

    if( 1 == flash_write(SECTOR_NUM, 0, 0x12345678) )   //д�����ݵ�������ƫ�Ƶ�ַΪ0������һ��д��4�ֽ�
        //if����������Ƿ�д��ɹ���д��ɹ��˾Ͷ�ȡ
    {
        data32 =    flash_read(SECTOR_NUM, 0, uint32);  //��ȡ4�ֽ�
        printf("һ�ζ�ȡ32λ������Ϊ��0x%08x\n", data32);

        data16 =    flash_read(SECTOR_NUM, 0, uint16);  //��ȡ2�ֽ�
        printf("һ�ζ�ȡ16λ������Ϊ��0x%04x\n", data16);

        data8  =    flash_read(SECTOR_NUM, 0, uint8);   //��ȡ1�ֽ�
        printf("һ�ζ�ȡ8λ������Ϊ��0x%02x\n", data8);
    }

    while(1);
}
