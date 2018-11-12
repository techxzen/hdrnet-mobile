
#ifndef __GRID_NET_H
#define __GRID_NET_H

#include "cnn/ILayer.h"
#include <vector>

class GridNet final
{
public:
    GridNet();

    ~GridNet() = default;

    int build_network();

    int run_network(float * input, float * output);
    
    int clean_network();

private:
    std::vector<ILayer *> _layers;
    
    float ** _input_ptr_ptr;
    float ** _output_ptr_ptr;
    float * _input_ptr;
    float * _output_ptr;

    float * _buf1;
    float * _buf2;
    float * _buf3;
};

#endif