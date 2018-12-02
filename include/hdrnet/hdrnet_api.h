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

#ifndef __HDRNET_API_H
#define __HDRNET_API_H

extern "C"
{
/* HDRNET_Setup */
int HDRNET_setup(void ** handle, const char * model_dir);

/* HDRNET_Run */
int HDRNET_run(void ** handle, unsigned char * src_data, unsigned char * dst_data, int height, int width);

/* HDRNET_Clean */
int HDRNET_clean(void ** handle);

/* HDRNET read & write */
int HDRNET_read_uchar_data_from_file(unsigned char * data, int size, const char * file_name);

int HDRNET_read_float_data_from_file(float * data, int size, const char * file_name);

int HDRNET_write_uchar_data_to_file(unsigned char * data, int size, const char * file_name);

int HDRNET_write_float_data_to_file(float * data, int size, const char * file_name);

/* function pointer */
typedef int (* Func_HDRNET_setup)(void ** handle, const char * model_dir);
typedef int (* Func_HDRNET_run)(void ** handle, unsigned char * src_data, unsigned char * dst_data, int height, int width);
typedef int (* Func_HDRNET_clean)(void ** handle);

typedef int (* Func_HDRNET_read_uchar_data_from_file)(unsigned char * data, int size, const char * file_name);
typedef int (* Func_HDRNET_read_float_data_from_file)(float * data, int size, const char * file_name);
typedef int (* Func_HDRNET_write_uchar_data_to_file)(unsigned char * data, int size, const char * file_name);
typedef int (* Func_HDRNET_write_float_data_to_file)(float * data, int size, const char * file_name);
}

#endif // __HDRNET_API_H