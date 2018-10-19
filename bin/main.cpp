
#include <string>
#include <iostream>
#include "hdrnet/Utils.h"
#include "hdrnet/hdrnet_api.h"
#include "hdrnet/RGBData.h"

#define ROOT_DIR "/home/chen/myworkspace/projects/"


int main(int argc, char ** argv)
{
    /* input data param */
    std::string input_file = std::string(ROOT_DIR) + std::string("hdrnet-mobile/../sample_data/input_2048x2048.rgb");
    std::string output_file = std::string(ROOT_DIR) + std::string("hdrnet-mobile/../sample_data/input_2048x2048_output.rgb");
    
    int height = 2048;
    int width  = 2048;

    RGBData<char> rgb_data(height, width);

    /* 1. get data */
    rgb_data.get_data_from_rgb_file(input_file.c_str());

    /* 2. test api: run hdrnet */
    run_hdrnet(&rgb_data);

    /* 3. save data */
    rgb_data.save_data_to_rgb_file(output_file.c_str());

    return 0;
}