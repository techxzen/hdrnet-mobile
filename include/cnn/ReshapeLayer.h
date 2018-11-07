

#ifndef __RESHAPELAYER_H
#define __RESHAPELAYER_H

#include "cnn/BaseLayer.h"


class ReshapeLayer final : public BaseLayer
{
public:
    ReshapeLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape,
        int idx0, int idx1, int idx2, int idx3) : BaseLayer(in, out, in_shape, out_shape)
    {
        // Initialization
        _idx[0] = idx0;
        _idx[1] = idx1;
        _idx[2] = idx2;
        _idx[3] = idx3;
    }

    int run() override;

public:
    int _idx[4];
};


int ReshapeLayer::run()
{
    LOGD("# ReshapeLayer run!\n");

    return 0;
}


#endif //__RESHAPELAYER_H