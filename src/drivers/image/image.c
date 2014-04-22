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
 
u8  image_left_offset(u8 (*image)[COL] , u8 n)
{
	u8 left_edge = COL / 2;
	while(left_edge > 0)
	{
		if(*(*(image + n) + left_edge) <= threshold)
			break;
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
 
u8 image_right_offset(u8 (*image)[COL] , u8 n)
{
	u8 right_edge = COL / 2;
	while(right_edge < COL)
          {
		if( *(*(image + n) + right_edge) <= threshold)
			break;
		right_edge++;
          }
	return(right_edge - COL/2);
}


 /***********************************************************************
 *
 函数名		:	image_average_offset
 参数		:	图像二维数组。
 输出		:	每行平均偏差。
 操作结果	:	提取一行右边偏移值。
 *
 ************************************************************************/
 
 u8 image_average_offset(u8 (*image)[COL] , u8 m,u8 n)
 {
	u8 i;
	unsigned int offset;
	for(i=m ; i<n ; i++)
	{	
		offset = offset + image_right_offset(image , i) + image_left_offset(image , i);
	}
	return((u8 )(offset / (n-m)));
 }
 