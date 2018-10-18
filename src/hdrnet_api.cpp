

#include "hdrnet/hdrnet_api.h"
#include "hdrnet/preprocess.h"
#include <cstdio>


int run_hdrnet(void * input_data, void * output_data, int height, int width, int stride)
{
    char * input_rgb = (char *)input_data;//-------- static_cast ? dynamic_cast ?

    float * float_data = nullptr;

    normalize_data(input_rgb, float_data);

    

    printf("# run_hdrnet!\n");

    return 0;
}