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



int HDRNET_setup(void ** handle, const char * model_dir)
{
    LOGD("# setup_hdrnet\n");

    (*handle) = new HdrnetTask(model_dir);

    return 0;
}

int HDRNET_run(void ** handle, UINT8 * src_img, UINT8 * dst_img, int height, int width)
{
    LOGD("# run_hdrnet!\n");

    int ret = 0;

    /* memory allocate */
    int size = height * width * 3;
    float * fullres_img_hwc = new float [size];

    /* normalize */
    normalize_data_to_float(src_img, fullres_img_hwc, size);

    HdrnetTask * hdrnet_task = static_cast<HdrnetTask *>(* handle);

    hdrnet_task->run_task(fullres_img_hwc, fullres_img_hwc, height, width);

    convert_float_to_char(fullres_img_hwc, dst_img, size);

    return ret;
}


int HDRNET_clean(void ** handle)
{
    LOGD("# clean_hdrnet\n");

    HdrnetTask * hdrnet_task = static_cast<HdrnetTask *>(* handle);

    delete hdrnet_task;

    return 0;
}


int HDRNET_read_data_from_file(UINT8 * data, int size, const char * file_name)
{
    return read_data_from_file(data, size, file_name);
}

int HDRNET_read_data_from_file(float * data, int size, const char * file_name)
{
    return read_data_from_file(data, size, file_name);
}

int HDRNET_write_data_to_file(UINT8 * data, int size, const char * file_name)
{
    return write_data_to_file(data, size, file_name);
}

int HDRNET_write_data_to_file(float * data, int size, const char * file_name)
{
    return write_data_to_file(data, size, file_name);
}
