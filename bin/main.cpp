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
    void * handle;

    ret = setup_hdrnet(&handle);

    ret = run_hdrnet(&handle, rgb_data, rgb_data, height, width);

    ret = clean_hdrnet(&handle);

    /* 3. save data */
    ret = write_data_to_file(rgb_data, size, output_file);

    delete [] rgb_data;

    return ret;
}