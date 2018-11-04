

#ifndef __FUSIONADDLAYER_H
#define __FUSIONADDLAYER_H

#include "cnn/BaseLayer.h"


class FusionAddLayer final : public BaseLayer
{
public:
    FusionAddLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape
        ) : BaseLayer(in, out, in_shape, out_shape)
    {
        // Initialization
    }

    int run() override;
};


int FusionAddLayer::run()
{
    LOGD("# FusionAddLayer run!\n");

    return 0;
}


#endif //__FUSIONLAYER_H