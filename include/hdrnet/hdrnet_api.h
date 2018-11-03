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

/* HDRNET_Setup */
int setup_hdrnet();

/* HDRNET_Run */
int run_hdrnet(unsigned char * src_data, unsigned char * dst_data, int height, int width);

/* HDRNET_Clean */
int clean_hdrnet();


#endif // __HDRNET_API_H