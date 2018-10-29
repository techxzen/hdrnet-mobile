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
    float * normalized_img = new float [size];
    float * f256x256_img = new float [256 * 256 * 3];


    /* normalize */
    normalize_data(src_img, normalized_img, size);

    /* resize */
    resize_image(normalized_img, f256x256_img, height, width, 256, 256);

    get_grid(f256x256_img, f256x256_img);

    write_data_to_file(f256x256_img, 256 * 256 * 3, "test.rgb");

    /* guide map */
    generate_guide_map(normalized_img);

    /* --- free memory --- */
    delete [] f256x256_img;
    delete [] normalized_img;

    LOGD("# run_hdrnet!\n");

    return ret;
}

int clean_hdrnet()
{
    LOGD("# clean_hdrnet\n");
    return 0;
}