/*
*******************************************************************************
*       Filename:  template.cpp
*    Description:  cpp file
*       
*        Version:  1.0
*        Created:  2018-07-20
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/
#include "utils/Utils.h"
#include <cstdio>
#include <cmath>

#define LOGE(x...) printf(x)

#ifdef DEBUG
#define LOGD(x...) printf(x)
#else
#define LOGD(x...) 
#endif

template<typename Type>
int read_data_from_file(Type * data, int size, const char * file_name)
{
    FILE * fi = fopen(file_name, "r");
    if (fi == NULL)
    {
        LOGE("Error: file open error! %s\n", file_name);
        return -1;
    }

    fread(data, sizeof(Type), size,  fi);

    fclose(fi);

    LOGD("# get_data_from_rgb_file!\n");
    return 0;
}

template int read_data_from_file<UINT8>(UINT8 * data, int size, const char * file_name);
template int read_data_from_file<float>(float * data, int size, const char * file_name);



int write_data_to_file(UINT8 * data, int size, const char * file_name)
{
    FILE * fo = fopen(file_name, "w");
    if (fo == NULL)
    {
        LOGE("Error: file open error! %s\n", file_name);
        return -1;
    }

    fwrite(data, sizeof(UINT8), size, fo);

    fclose(fo);

    LOGD("%s saved!\n", file_name);

    return 0;
}


int write_data_to_file(float * float_data, int size, const char * file_name)
{
    UINT8 * uint8_data = new UINT8 [size];

    for (int i = 0; i < size; i++)
    {
        int tmp = round(float_data[i] * float(255));
        uint8_data[i] = (tmp < 0) ? 0 : ((tmp > 255) ? 255 : tmp);
    }

    write_data_to_file(uint8_data, size, file_name);
    
    delete [] uint8_data;

    return 0;
}
