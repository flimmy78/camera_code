#include    "include.h"
#include    "common.h"

/***********************************************
*   函数名：str2float
*   将字符串转换为浮点数
*   str为字符串首地址，n为字符串长度
***********************************************/

float str2ufloat(char * str,u8 n)
{
    u8 point = 0;       //查询小数点，返回小数点的位置，若无则为0
    int len = 0;               //字符串长度
    float inc = 1;
    u8 p,l;
    float num;
    
    for(len=0; len<n; len++)
    {
        if(str[point] != '.')
           point ++;
    }
    
    if(point > len)      //无小数点
    {
        for(len=n-1; len>=0; len--)       //最后一个字符的标号
        {
            num = num + ((u8)str[len]-48)*inc;
            
            inc = inc*10;
        }
    }
    else
    {
        len = n-1;              //最后一个字符的标号
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
*   函数名：str2int
*   将字符串转换为整型
*   str为字符串首地址，n为字符串长度
***********************************************/
int str2int(char * str, int n)
{
    int len = n-1;               //最后一个字符的标号
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