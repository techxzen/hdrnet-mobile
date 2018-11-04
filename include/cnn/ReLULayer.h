
#ifndef __RELULAYER_H
#define __RELULAYER_H

#include "cnn/BaseLayer.h"


class ReLULayer final : public BaseLayer
{
public:
    ReLULayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape
        ) : BaseLayer(in, out, in_shape, out_shape)
    {
        // Initialization
    }

    int run() override;

};


int ReLULayer::run()
{
    LOGD("# ReLULayer run!\n");

#if 0
    for (int i = 0; i < _output_shape.size(); i++)
    {
        _output[i] = (_input[i] > 0) ? _input[i] : 0;
    }
#endif

    return 0;
}


#endif //__RELULAYER_H