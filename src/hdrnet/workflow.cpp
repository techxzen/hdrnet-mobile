/*
*******************************************************************************
*       Filename:  workflow.cpp
*    Description:  cpp file
*       
*        Version:  1.0
*        Created:  2018-10-29
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/

#include "hdrnet/workflow.h"
#include "cnn/ConvolutionLayer.h"
#include <vector>

#include "utils/Utils.h"


int get_grid(float * in, float * out)
{
    std::vector<BaseLayer *> layers;

    // Create Buffer
    float * buf1 = in;
    float * buf2 = out;

    // construct network
    ConvolutionLayer layer1 = ConvolutionLayer(buf1, buf2, {1,2,3,4}, {1,2,3,4}, 1, 1, 1, 1, 1, 1);
    layers.push_back( &layer1 );

    ConvolutionLayer layer2 = ConvolutionLayer(buf2, buf1, {2,2,3,4}, {1,2,3,4}, 1, 1, 1, 1, 1, 1);
    layers.push_back( &layer2 );


    // run network
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        (*it)->run();
    }

    return 0;
}


int generate_guide_map(float * full_res)
{
    // allocate 
    float * coeff = new float [16 * 16 * 8 * 3 * 4];
    float * guide_ref = new float [2048 * 2048];

    // read data
    read_data_from_file(coeff, 16 * 16 * 8 * 3 * 4, "/home/chen/myworkspace/projects/sample_data/temp/coeffs_16x16x8x3x4.float32");
    read_data_from_file(guide_ref, 16 * 16 * 8 * 3 * 4, "/home/chen/myworkspace/projects/sample_data/temp/guide_2048x2048.float32");

    // run


    // free
    delete [] guide_ref;
    delete [] coeff;

    return 0;
}