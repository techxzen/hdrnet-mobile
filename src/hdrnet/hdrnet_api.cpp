/*
*******************************************************************************
*       Filename:  hdrnet_api.cpp
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
#include "hdrnet/hdrnet_api.h"
#include "hdrnet/preprocess.h"
#include "hdrnet/workflow.h"
#include "utils/Utils.h"
#include <cstdio>


int setup_hdrnet()
{
    LOGD("# setup_hdrnet\n");
    return 0;
}

int run_hdrnet(UINT8 * src_img, UINT8 * dst_img, int height, int width)
{
    int ret = 0;

    /* --- memory allocate --- */
    int size = height * width * 3;
    float * fullres_img_hwc = new float [size];
    float * lowres_img_hwc = new float [256 * 256 * 3];

#if 1
    /* normalize */
    normalize_data(src_img, fullres_img_hwc, size);
#else
    read_data_from_file(fullres_img_hwc, size, "/home/chen/myworkspace/projects/sample_data/temp/fullres_2048x2048x3.float32");
#endif

#if 1
    /* resize */
    resize_image(fullres_img_hwc, lowres_img_hwc, height, width, 256, 256);
#else
    read_data_from_file(lowres_img_hwc, 256 * 256 * 3, "/home/chen/myworkspace/projects/sample_data/temp/lowres.float32-256x256x3");
#endif


    /* AI inference */
    float * AI_out = new float [8 * 128 * 128];

    float * AI_ref = new float [8 * 128 * 128];

    //read_data_from_file(AI_ref, 8 * 128 * 128, "/home/chen/myworkspace/projects/sample_data/temp/conv1.float32-1x128x128x8");
    //read_data_from_file(AI_ref, 8 * 128 * 128, "/home/chen/myworkspace/projects/sample_data/temp/local.float32-1x16x16x64");
    //read_data_from_file(AI_ref, 1 * 1024, "/home/chen/myworkspace/projects/sample_data/temp/reshape.float32-1x1024");
    //read_data_from_file(AI_ref, 1 * 256, "/home/chen/myworkspace/projects/sample_data/temp/fc.float32-1x256");
    //read_data_from_file(AI_ref, 1 * 256, "/home/chen/myworkspace/projects/sample_data/temp/fc2.float32-1x128");
    //read_data_from_file(AI_ref, 1 * 64, "/home/chen/myworkspace/projects/sample_data/temp/fc3.float32-1x64");
    read_data_from_file(AI_ref, 16 * 16 * 96, "/home/chen/myworkspace/projects/sample_data/temp/coeffs_16x16x8x3x4.float32");
    //read_data_from_file(AI_ref, 16 * 16 * 64, "/home/chen/myworkspace/projects/sample_data/temp/fusion.float32-16x16x64");
    //read_data_from_file(AI_ref, 16 * 16 * 96, "/home/chen/myworkspace/projects/sample_data/temp/prediction.float32-16x16x96");
    printf("data ready\n");


    get_grid(lowres_img_hwc, AI_out);

    printf("conv1_out:\n");
    for (int i = 0; i < 65; i++)
    {
        printf("%f ", AI_out[i]);
    }
    printf("\n");

    printf("conv1_ref:\n");
    for (int i = 0; i < 65; i++)
    {
        printf("%f ", AI_ref[i]);
    }
    printf("\n");

    delete [] AI_ref;


    /* guide map */
    float * guide_map = new float [height * width];
    generate_guide_map(fullres_img_hwc, guide_map, height, width);

    /* apply slicing layer */
    apply_slicing_layer_and_assemble(fullres_img_hwc, AI_out, guide_map, fullres_img_hwc, height, width);

    
    convert_float_to_char(fullres_img_hwc, dst_img, size);


    delete [] guide_map;
    delete [] AI_out;

    /* --- free memory --- */
    delete [] fullres_img_hwc;
    delete [] lowres_img_hwc;

    LOGD("# run_hdrnet!\n");

    return ret;
}

int clean_hdrnet()
{
    LOGD("# clean_hdrnet\n");
    return 0;
}