#include    "include.h"
#include    "common.h"

/***********************************************
*   ��������str2float
*   ���ַ���ת��Ϊ������
*   strΪ�ַ����׵�ַ��nΪ�ַ�������
***********************************************/

float str2ufloat(char * str,u8 n)
{
    u8 point = 0;       //��ѯС���㣬����С�����λ�ã�������Ϊ0
    int len = 0;               //�ַ�������
    float inc = 1;
    u8 p,l;
    float num;
    
    for(len=0; len<n; len++)
    {
        if(str[point] != '.')
           point ++;
    }
    
    if(point > len)      //��С����
    {
        for(len=n-1; len>=0; len--)       //���һ���ַ��ı��
        {
            num = num + ((u8)str[len]-48)*inc;
            
            inc = inc*10;
        }
    }
    else
    {
        len = n-1;              //���һ���ַ��ı��
        p = point;
        l = len;
        while(point < len)
        {
            point++;
            inc = inc*0.1;
            
            num = num + ((u8)str[point]-48)*inc;
        }
        point = p;
        len = l;
        inc = 1;
        while(point > 0)
        {
            point--;
            
            num = num+((u8)str[point]-48)*inc;
            
            inc = inc*10;
        }
    }
    
    return num;
}

/***********************************************
*   ��������str2int
*   ���ַ���ת��Ϊ����
*   strΪ�ַ����׵�ַ��nΪ�ַ�������
***********************************************/
int str2int(char * str, int n)
{
    int len = n-1;               //���һ���ַ��ı��
    u16 inc = 1;
    int num = 0;
    
    if(str[0] == '-')
    {
        for(;len >0;len--)
        {
            num = num + str[len]*inc;
            inc = inc * 10;
        }
    }
    else
    {
        for(;len >= 0; len--)
        {
            num = num + str[len]*inc;
            inc = inc * 10;
        }
    }
    
    return num;
}