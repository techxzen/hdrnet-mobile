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

#define CLIP(x, min, max) ((x < min) ? min : (x > max) ? max : x) 

int normalize_data(UINT8 * uint8_data, float * float_data, int size)
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

int resize_image(float * src_img, float * dst_img, int src_height, int src_width, int dst_height, int dst_width)
{
    float h_scale = float(src_height) / float(dst_height);
    float w_scale = float(src_width) / float(dst_width);

    for (int c = 0; c < 3; c++)
    {
        for (int h = 0; h < dst_height; h++)
        {
            for (int w = 0; w < dst_width; w++)
            {
                int nearest_h = floor(h * h_scale);
                int nearest_w = floor(w * w_scale);

                #ifdef DEBUG
                if(nearest_h < 0 || nearest_h > src_height - 1 ||
                    nearest_w < 0 || nearest_w > src_width -1)
                {
                    LOGE("Error: index out of range.\n");
                }
                #endif
                
                int dst_pos = h * dst_width * 3 + w * 3+ c;
                int src_pos = nearest_h * src_width * 3 + nearest_w * 3 +c;
                                
                dst_img[dst_pos]= src_img[src_pos];            
            }
        }
    }

    return 0;
}
