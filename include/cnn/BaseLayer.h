
#ifndef __BASELAYER_H
#define __BASELAYER_H

#include "helper.h"

struct TensorShape
{
    int n;
    int c;
    int h;
    int w;
};


class BaseLayer
{
public:
    BaseLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape)
    {
        _input = in;
        _output = out;
        _input_shape = in_shape;
        _output_shape = out_shape;
    }

    virtual int run() = 0;

public:
    float * _input;
    float * _output;
    TensorShape _input_shape;
    TensorShape _output_shape;
};


#endif //__BASELAYER_H