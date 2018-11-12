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
#include "hdrnet/hdrnet_api.h"
#include "utils/Utils.h"
#include "hdrnet/HdrnetTask.h"
#include "hdrnet/preprocess.h"
#include <cstdio>


struct HdrnetHandle
{
    HdrnetTask _hdrnet_task;
};


int setup_hdrnet(void ** handle)
{
    LOGD("# setup_hdrnet\n");

    (*handle) = new HdrnetHandle;

    HdrnetHandle * hdrnet_handle = static_cast<HdrnetHandle *>(* handle);

    hdrnet_handle->_hdrnet_task.build_task();

    return 0;
}

int run_hdrnet(void ** handle, UINT8 * src_img, UINT8 * dst_img, int height, int width)
{
    LOGD("# run_hdrnet!\n");

    int ret = 0;

    /* --- memory allocate --- */
    int size = height * width * 3;
    float * fullres_img_hwc = new float [size];

    /* normalize */
    normalize_data_to_float(src_img, fullres_img_hwc, size);

    HdrnetHandle * hdrnet_handle = static_cast<HdrnetHandle *>(* handle);

    hdrnet_handle->_hdrnet_task.run_task(fullres_img_hwc, fullres_img_hwc, height, width);

    convert_float_to_char(fullres_img_hwc, dst_img, size);

    return ret;
}


int clean_hdrnet(void ** handle)
{
    LOGD("# clean_hdrnet\n");

    HdrnetHandle * hdrnet_handle = static_cast<HdrnetHandle *>(* handle);

    hdrnet_handle->_hdrnet_task.clean_task();

    return 0;
}