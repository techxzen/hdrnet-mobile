
#include <string>
#include <iostream>
#include "hdrnet/Utils.h"
#include "hdrnet/hdrnet_api.h"

#define ROOT_DIR "/home/chen/myworkspace/projects/"


int main(int argc, char ** argv)
{
    /* input data param */
    std::string input_file = std::string(ROOT_DIR) + std::string("hdrnet/../sample_data/input_2048x2048.rgb");
    std::string output_file = std::string(ROOT_DIR) + std::string("hdrnet/../sample_data/input_2048x2048.rgb");
    
    int height = 2048;
    int width  = 2048;
    int stride = 2048;
    int size = height * stride;

    /* set buffer */
    char * input_data = new char [size];
    char * output_data = new char [size];

    /* 1. get data */
    Utils::get_data_from_rgb_file(input_data, input_file.c_str());

    /* 2. test api: run hdrnet */
    run_hdrnet(input_data, output_data, height, width, stride);

    /* 3. save data */
    Utils::save_data_to_rgb_file(output_data, output_file.c_str());

    /* free buffer */
    delete [] input_data;
    delete [] output_data;

    return 0;
}