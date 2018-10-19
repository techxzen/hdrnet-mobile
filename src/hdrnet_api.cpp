
#include "hdrnet/Utils.h"
#include "hdrnet/hdrnet_api.h"
#include "hdrnet/preprocess.h"
#include <cstdio>


int run_hdrnet(RGBData<char> * rgb_data)
{
    int ret = 0;

    RGBData<float> float_rgb_data(rgb_data->get_height(), rgb_data->get_width());

    ret = normalize_data(rgb_data, &float_rgb_data);
    if (0 != ret)
    {
        LOGE("Error: normalize_data failed! \n");
    }

    LOGD("# run_hdrnet!\n");

    return 0;
}