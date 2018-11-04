
#ifndef __BASELAYER_H
#define __BASELAYER_H

#include "ILayer.h"

class BaseLayer : public ILayer
{
public:
    BaseLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape)
    {
        _input = in;
        _output = out;
        _input_shape = in_shape;
        _output_shape = out_shape;
    }

public:
    float * _input;
    float * _output;
    TensorShape _input_shape;
    TensorShape _output_shape;
};


#endif //__BASELAYER_H