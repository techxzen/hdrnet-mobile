/*
*******************************************************************************
*       Filename:  main.cpp
*    Description:  cpp file
*       
*        Version:  1.0
*        Created:  2018-10-10
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#include <string>
#include <iostream>
#include "utils/Utils.h"
#include "hdrnet/hdrnet_api.h"

#define INPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/input_2048x2048.rgb"
#define OUTPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/input_2048x2048_output.rgb"


int main(int argc, char ** argv)
{
    int ret = 0;

    /* input data param */  
    const char * input_file = INPUT_FILE_PATH;
    const char * output_file = OUTPUT_FILE_PATH;

    int height = 2048;
    int width  = 2048;
    int size = height * width * 3;

    UINT8 * rgb_data = new UINT8 [size];

    /* 1. get data */
    ret = read_data_from_rgb_file(rgb_data, size, input_file);

    /* test hdrnet api */
    ret = setup_hdrnet();

    ret = run_hdrnet(rgb_data, rgb_data, height, width);

    ret = clean_hdrnet();

    /* 3. save data */
    ret = write_data_to_rgb_file(rgb_data, size, output_file);

    delete [] rgb_data;

    return ret;
}