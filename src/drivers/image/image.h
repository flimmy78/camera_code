/********************************************************************
 *
 *	文件名：	image.h
 *	功能  ：	计算图像中线，返回图像中线与车夹角。
 *	作者  ：	赵志明
 *	日期  ：	2013/11/20
 *	版本  ：	V1.0
 *
 *
 *  说明：本接口调用函数image_average_offset获取每幅图像偏差值。使用方式为：image_average_offset(image , threshold)
 *	其中image为图像的二维数组，threshold为图像二值化设定的阈值。
 *******************************************************************/
#ifndef _IMAGE_H_
#define _IMAGE_H_
/************摄像头配置************/
#include "common.h"
#include "Kalman.h"
#define    ROW_START    120
#define    ROW_END      159
#define    ROW          40      //ROW_END-ROW_START
#define    COL          200					//图像列数
extern u8 threshold;
extern u8 image[ROW][COL];   
extern u8 edge_l[ROW];                //存取图像左偏差。
extern u8 edge_r[ROW];               //存取图像右偏差。
   
//#include "stdint.h"

/***********************************************************************
 *
 函数名		:	image_left_offset
 参数		:	image:图像二维数组行指针 n:行号
 输出		:	无
 操作结果	:	提取一行左边偏移值。
 *
 ************************************************************************/

u8 image_left_offset(u8 (*image)[COL] , u8 n);

/***********************************************************************
 *
 函数名		:	image_right_offset
 参数		:	image:图像二维数组行指针 n:行号.
 输出		:	无
 操作结果	:	提取一行右边偏移值。
 *
 ************************************************************************/

u8 image_right_offset(u8 (*image)[COL] , u8 n);

/***********************************************************************
 *
 函数名		:	image_err
 参数		:	image:图像二维数组行指针 
 输出		:	无
 操作结果	:	提取图像平均偏差。
 *
 ************************************************************************/

void image_err(cars_status car,u8 m , u8 n);

#endif
