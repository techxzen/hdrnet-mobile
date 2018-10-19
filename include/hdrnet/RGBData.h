

#ifndef __RGBDATA_H
#define __RGBDATA_H

#include "hdrnet/Utils.h"
#include <cstdio>

template <typename DataType>
class RGBData
{
public:
    RGBData()
    {
        _data_ptr = nullptr;
        _width = 0;
        _height = 0;
    }

    RGBData(int h, int w) : _width(w), _height(h)
    {
        _size = _height * _width * 3;
        _data_ptr = new DataType [_size];
    }

    ~RGBData()
    {
        if (nullptr != _data_ptr)
        {
            delete [] _data_ptr;
        }
    }

    int get_size()
    {
        return _size;
    }

    int get_height()
    {
        return _height;
    }

    int get_width()
    {
        return _width;
    }

    DataType * get_data_ptr()
    {
        return _data_ptr;
    }


    int get_data_from_rgb_file(const char * file_name);

    int save_data_to_rgb_file(const char * file_name);

private:
    DataType *  _data_ptr;
    int         _width;
    int         _height;
    int         _size;
};


template <typename DataType>
int RGBData<DataType>::get_data_from_rgb_file(const char * file_name)
{
    FILE * fi = fopen(file_name, "r");
    if (fi == NULL)
    {
        LOGE("Error: file open error! %s\n", file_name);
        return -1;
    }

    fread(_data_ptr, sizeof(char), _size,  fi);

    fclose(fi);

    LOGD("# get_data_from_rgb_file!\n");
    return 0;
}    

template <typename DataType>
int RGBData<DataType>::save_data_to_rgb_file(const char * file_name)
{
    FILE * fo = fopen(file_name, "w");
    if (fo == NULL)
    {
        LOGE("Error: file open error! %s\n", file_name);
        return -1;
    }

    fwrite(_data_ptr, sizeof(char), _size, fo);

    fclose(fo);

    LOGD("# save_data_to_rgb_file!\n");
    return 0;
}




#endif // __RGBDATA_H