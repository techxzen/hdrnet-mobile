/*
*******************************************************************************
*       Filename:  preoprocess.h
*    Description:  header file
*       
*        Version:  1.0
*        Created:  2018-10-10
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#ifndef __PREPROCESS_H
#define __PREPROCESS_H

#include "type.h"

int normalize_data(UINT8 * char_data, float * float_data, int size);

int convert_float_to_char(float * float_data, UINT8 * char_data, int size);

int resize_image(float * src_img, float * dst_img, int src_height, int src_width, int dst_height, int dst_width);


#endif // __PREPROCESS_H