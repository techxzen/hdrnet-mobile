
#ifndef __HDRNET_TASK_H
#define __HDRNET_TASK_H

#include "hdrnet/GridNet.h"

class HdrnetTask
{
public:
    HdrnetTask() = default;
    
    ~HdrnetTask() = default;

    int build_task();

    int run_task(float * in, float * out, int height, int width);
    
    int clean_task();

private:
    int resize(float * src, float * dst, int src_height, int src_width, int dst_height, int dst_width);

    int generate_bilateral_grid(
        float * in, 
        float * out
        );

    int generate_guide_map(
        float * full_res, //full res HWC
        float * guide_out, //full res
        int height,
        int width 
        );

    int apply_slicing_layer_and_assemble(
        float * img_in, // full_res, HWC format
        float * grid, // coeeff, 16x16x8x3x4
        float * guide_map, // guide map
        float * img_out,
        int height,
        int width
        );

private:
    GridNet _grid_net;

    float * _ccm;
    float * _ccm_bias;
    float * _shifts;
    float * _slopes;
    float * _channel_mix_weight;
    float * _channel_mix_bias;

    // buffer
    float * _lowres_img_hwc;
    float * _bilateral_grid;
};


#endif
