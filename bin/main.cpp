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
#include "hdrnet/hdrnet_api.h"


int main(int argc, char ** argv)
{
    int ret = 0;
    const char * model_dir = argv[1];
    
    /* input data param */
    const char * input_file = argv[2];
    const char * output_file = argv[3];
    int height = atoi(argv[4]);
    int width  = atoi(argv[5]);

    int size = height * width * 3;

    unsigned char * rgb_data = new unsigned char [size];

    /* 1. get data */
    ret = HDRNET_read_data_from_file(rgb_data, size, input_file);

    /* 2. test hdrnet api */
    void * handle;

    ret = HDRNET_setup(&handle, model_dir);

    ret = HDRNET_run(&handle, rgb_data, rgb_data, height, width);

    ret = HDRNET_clean(&handle);

    /* 3. save data */
    ret = HDRNET_write_data_to_file(rgb_data, size, output_file);

    delete [] rgb_data;

    return ret;
}