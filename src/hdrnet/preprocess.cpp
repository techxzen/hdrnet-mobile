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

int resize(float * src, float * dst, int src_height, int src_width, int dst_height, int dst_width)
{
    for (int h = 0; h < dst_height; h++)
    {
        for (int w = 0; w < dst_width; w++)
        {
            float crespond_h = (h + 0.5f) * src_height / (1.0f * dst_height) - 0.5f;
            float crespond_w = (w + 0.5f) * src_width / (1.0f * dst_width) - 0.5f;

            int floor_h = static_cast<int>(floor(crespond_h));
            int floor_w = static_cast<int>(floor(crespond_w));

            float value_r = 0.0f;
            float value_g = 0.0f;
            float value_b = 0.0f;

            for (int hh = floor_h; hh < floor_h + 2; hh++)
            {
                int h_idx     = std::max(std::min(hh, src_height - 1), 0);
                float h_ratio = std::max(1.0f - std::abs(crespond_h - hh), 0.0f);
                
                for (int ww = floor_w; ww < floor_w + 2; ww++)
                {
                    int w_idx     = std::max(std::min(ww, src_width - 1), 0);
                    float w_ratio = std::max(1.0f - std::abs(crespond_w - ww), 0.0f);
                
                    int in_idx = h_idx * src_width + w_idx;
                    value_r += src[in_idx * 3 + 0] * h_ratio * w_ratio;
                    value_g += src[in_idx * 3 + 1] * h_ratio * w_ratio;
                    value_b += src[in_idx * 3 + 2] * h_ratio * w_ratio;
                }
            }
                
            int dst_pos = h * dst_width + w;
                                
            dst[dst_pos * 3 + 0] = value_r;            
            dst[dst_pos * 3 + 1] = value_g;            
            dst[dst_pos * 3 + 2] = value_b;
        }
    }

    return 0;
}
