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
#include <dlfcn.h>
#include "hdrnet/hdrnet_api.h"


int main(int argc, char ** argv)
{
    int ret = 0;
    const char * dl_name = argv[1];
    const char * model_dir = argv[2];
    
    /* input data param */
    const char * input_file = argv[3];
    const char * output_file = argv[4];
    int height = atoi(argv[5]);
    int width  = atoi(argv[6]);

    int size = height * width * 3;

    unsigned char * rgb_data = new unsigned char [size];


#if 1 //use dlopen
    /* dlopen library */
    void * dl_handle = dlopen(dl_name, RTLD_NOW);
    if (NULL == dl_handle) {
        printf("Error: dlopen %s failed!\n", dl_name);
        return -1;
    }

    /* dlopen so, get function pointer */
    Func_HDRNET_read_uchar_data_from_file f_HDRNET_read_uchar_data_from_file = NULL;
    *(void **)(&f_HDRNET_read_uchar_data_from_file) = dlsym(dl_handle, "HDRNET_read_uchar_data_from_file");

    Func_HDRNET_write_uchar_data_to_file f_HDRNET_write_uchar_data_to_file = NULL;
    *(void **)(&f_HDRNET_write_uchar_data_to_file) = dlsym(dl_handle, "HDRNET_write_uchar_data_to_file");

    Func_HDRNET_setup f_HDRNET_setup = NULL;
    f_HDRNET_setup = (Func_HDRNET_setup)dlsym(dl_handle, "HDRNET_setup");

    Func_HDRNET_run f_HDRNET_run = NULL;
    f_HDRNET_run = (Func_HDRNET_run)dlsym(dl_handle, "HDRNET_run");
    
    Func_HDRNET_clean f_HDRNET_clean = NULL;
    f_HDRNET_clean = (Func_HDRNET_clean)dlsym(dl_handle, "HDRNET_clean");
    

    /* 1. get data */
    ret = (*f_HDRNET_read_uchar_data_from_file)(rgb_data, size, input_file);

    /* 2. test hdrnet api */
    void * handle;

    ret = (*f_HDRNET_setup)(&handle, model_dir);

    ret = (*f_HDRNET_run)(&handle, rgb_data, rgb_data, height, width);

    ret = (*f_HDRNET_clean)(&handle);

    /* 3. save data */
    ret = (*f_HDRNET_write_uchar_data_to_file)(rgb_data, size, output_file);

    dlclose(dl_handle);

#else
    /* 1. get data */
    ret = HDRNET_read_uchar_data_from_file(rgb_data, size, input_file);

    /* 2. test hdrnet api */
    void * handle;

    ret = HDRNET_setup(&handle, model_dir);

    ret = HDRNET_run(&handle, rgb_data, rgb_data, height, width);

    ret = HDRNET_clean(&handle);

    /* 3. save data */
    ret = HDRNET_write_uchar_data_to_file(rgb_data, size, output_file);
#endif

    delete [] rgb_data;

    return ret;
}