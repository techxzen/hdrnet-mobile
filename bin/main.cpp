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
#include <cstdlib>
#include <iostream>
#include "utils/Utils.h"
#include "hdrnet/hdrnet_api.h"

#if 1
#define INPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/input_2048x2048x3.rgb"
#define OUTPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/input_2048x2048x3_output.rgb"
#define IMG_H 2048
#define IMG_W 2048
#else
#define INPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/IMG_20181001_082343_3968x2976x3.rgb"
#define OUTPUT_FILE_PATH "/home/chen/myworkspace/projects/sample_data/IMG_20181001_082343_3968x2976x3_output.rgb"
#define IMG_H 3968
#define IMG_W 2976
#endif

int main(int argc, char ** argv)
{
    int ret = 0;

    /* input data param */  
    const char * input_file = argv[1];
    const char * output_file = argv[2];
    int height = atoi(argv[3]);
    int width  = atoi(argv[4]);

    int size = height * width * 3;

    UINT8 * rgb_data = new UINT8 [size];

    /* 1. get data */
    ret = read_data_from_file(rgb_data, size, input_file);

    /* test hdrnet api */
    ret = setup_hdrnet();

    ret = run_hdrnet(rgb_data, rgb_data, height, width);

    ret = clean_hdrnet();

    /* 3. save data */
    ret = write_data_to_file(rgb_data, size, output_file);

    delete [] rgb_data;

    return ret;
}