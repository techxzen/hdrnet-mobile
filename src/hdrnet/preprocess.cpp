/*
*******************************************************************************
*       Filename:  preprocess.cpp
*    Description:  cpp file
*       
*        Version:  1.0
*        Created:  2018-10-10
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#include "helper.h"
#include "hdrnet/preprocess.h"
#include <cmath>
#include <algorithm>

#define CLIP(x, min, max) ((x < min) ? min : (x > max) ? max : x) 

int normalize_data_to_float(UINT8 * uint8_data, float * float_data, int size)
{
    for (int i = 0; i < size; i++)
    {
        float_data[i] = float(uint8_data[i]) / float(255.0);
    }

    return 0;
}

int convert_float_to_char(float * float_data, UINT8 * uint8_data, int size)
{
    for (int i = 0; i < size; i++)
    {
        uint8_data[i] = CLIP(round(float_data[i] * 255), 0, 255);
    }

    return 0;
}

