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
/***********************************************************************
 *
 函数名		:	image_binary
 参数		:	**image:图像二维数组.threshold。:图像二值化阈值。
 输出		:	无
 操作结果	:	图像二值化
 *
 ************************************************************************/
 
void image_binary(int8_t (*image)[image_col] , int8_t threshold)
{	
	int8_t i,j;
	 for(i = 0 ; i < image_row ; i++)
    {
        for (j = 0 ; j < image_col ; j++)
          {
            if( *( *(image+i) + j) > threshold)
                *( *(image+i) + j) = 1;
            else
                *(*(image+i)+j) = 0;

          }
    }

}	

/***********************************************************************
 *
 函数名		:	image_left_offset
 参数		:	image:图像二维数组行指针.
 输出		:	无
 操作结果	:	提取一行左边偏移值。
 *
 ************************************************************************/
 
int8_t image_left_offset(int8_t (*image)[image_col] , int8_t n)
{
	int8_t left_edge = image_col / 2;
	while(left_edge > 0)
	{
		if(1 == *(*(image + n) + left_edge))
			break;
		left_edge--;
	}
	return(left_edge - (image_col / 2));
}

/***********************************************************************
 *
 函数名		:	image_right_offset
 参数		:	image:图像二维数组行指针.
 输出		:	无
 操作结果	:	提取一行右边偏移值。
 *
 ************************************************************************/
 
int8_t image_right_offset(int8_t (*image)[image_col] , int8_t n)
{
	int8_t right_edge = image_col / 2;
	while(right_edge < image_col)
          {
		if(1 == *(*(image + n) + right_edge))
			break;
		right_edge++;
          }
	return(right_edge - image_col / 2);
}


 /***********************************************************************
 *
 函数名		:	image_average_offset
 参数		:	图像二维数组。
 输出		:	每行平均偏差。
 操作结果	:	提取一行右边偏移值。
 *
 ************************************************************************/
 
 int8_t image_average_offset(int8_t (*image)[image_col] , int8_t threshold)
 {
	int8_t i;
	unsigned int offset;
	image_binary(image , threshold);
	for(i=0 ; i<image_row ; i++)
	{	
		offset = offset + image_right_offset(image , i) + image_left_offset(image , i);
	}
	return((int8_t )(offset / image_row));
 }
 