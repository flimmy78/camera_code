/********************************************************************
 *
	文件名：	image.c
	功能  ：	计算图像中线。
	作者  ：	赵志明
	日期  ：	2013/11/20
	版本  ：	V1.0
 *
 *******************************************************************/
 #include "image.h"
 #include "common.h"
 
/***********************************************************************
 *
 函数名		:	image_left_offset
 参数		:	image:图像二维数组行指针.
 输出		:	无
 操作结果	:	提取一行左边偏移值。
 *
 ************************************************************************/
 
int16_t  image_left_offset(u8 (*image)[COL] , u8 n)
{
        unsigned int temp = 0;
	int16_t left_edge = COL / 2;
	while(left_edge > 0)
	{
		if(*(*(image + n) + left_edge) <= threshold) temp++;
                else temp = 0;
                if(temp == 3) break;
		left_edge--;
	}
	return(left_edge -COL/2 );
}

/***********************************************************************
 *
 函数名		:	image_right_offset
 参数		:	image:图像二维数组行指针.
 输出		:	无
 操作结果	:	提取一行右边偏移值。
 *
 ************************************************************************/
 
int16_t image_right_offset(u8 (*image)[COL] , u8 n)
{
       unsigned int temp = 0;
	int16_t right_edge = COL / 2;
	while(right_edge < COL)
          {
		if( *(*(image + n) + right_edge) <= threshold) temp++;
                else temp = 0;
                if( temp == 0) break;
		right_edge++;
          }
	return(right_edge - COL/2);
}

/***********************************************************************
 *
 函数名		:	image_err
 参数		:	image:图像二维数组行指针 
 输出		:	无
 操作结果	:	提取图像平均偏差。
 *
 ************************************************************************/

void image_err(cars_status car,u8 m , u8 n)
{
  float temp;
  int i;
  car->direction_err_old = car->direction_err_new;
  for(i = m;i<= n ;i++)
  {
    temp  += (edge_l[i] + edge_r[i]) / 2.0;
  }
  car->direction_err_new = temp/(n-m +1 ) ;
}

 