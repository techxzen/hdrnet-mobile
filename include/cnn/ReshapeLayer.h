

#ifndef __RESHAPELAYER_H
#define __RESHAPELAYER_H

#include "cnn/BaseLayer.h"


class ReshapeLayer final : public BaseLayer
{
public:
    ReshapeLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape,
        int idx0, int idx1, int idx2, int idx3) : BaseLayer(in, out, in_shape, out_shape), 
            _idx0(idx0), _idx1(idx1), _idx2(idx2), _idx3(idx3)
    {
        // Initialization
    }

    int run() override;

public:
    int _idx0;
    int _idx1;
    int _idx2;
    int _idx3;
};


int ReshapeLayer::run()
{
    LOGD("# ReshapeLayer run!\n");

    return 0;
}


#endif //__RESHAPELAYER_H