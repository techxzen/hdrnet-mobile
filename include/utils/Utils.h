/*
*******************************************************************************
*       Filename:  Utils.h
*    Description:  header file
*       
*        Version:  1.0
*        Created:  2018-10-10
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#ifndef __UTILS_H
#define __UTILS_H

#define UINT8 unsigned char

template<typename Type>
int read_data_from_file(Type * data, int size, const char * file_name);

int write_data_to_file(UINT8 * data, int size, const char * file_name);

int write_data_to_file(float * data, int size, const char * file_name);

#endif // __UTILS_H