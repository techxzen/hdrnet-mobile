
#include <string>
#include <iostream>
#include "Utils.h"
#include "hdrnet_api.h"

#define ROOT_DIR "/home/chen/myworkspace/projects/"


int main(int argc, char ** argv)
{
    std::string input_file = std::string(ROOT_DIR) + std::string("hdrnet/../sample_data/");
    std::string output_file = std::string(ROOT_DIR) + std::string("hdrnet/../sample_data/");

    char * input_data;
    char * output_data;

    /* 1. get data */
    Utils::get_data_from_rgb_file(input_data, input_file.c_str());

    /* 2. test api: run hdrnet */
    hdrnet::run_hdrnet(input_data, output_data);

    /* 3. save data */
    Utils::save_data_to_rgb_file(output_data, output_file.c_str());

    free(input_data);
    free(output_data);

    return 0;
}