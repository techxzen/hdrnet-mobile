/*
*******************************************************************************
*       Filename:  helper.h
*    Description:  header file
*       
*        Version:  1.0
*        Created:  2018-10-10
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#ifndef __HELPER_H
#define __HELPER_H

#include <cstdio>
#include <string>

#define LOGE(msg...) printf(msg);
#define LOGD(msg...) printf(msg);


inline std::string get_model_path_string(const char * relative_path)
{
    return (std::string(MODEL_DIR) + std::string(relative_path)).c_str();
}

#endif // __HELPER_H