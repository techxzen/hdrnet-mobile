

#ifndef __FUSIONADDLAYER_H
#define __FUSIONADDLAYER_H

#include "cnn/BaseLayer.h"


class FusionAddLayer final : public BaseLayer
{
public:
    FusionAddLayer(float ** in, float ** out, TensorShape in_shape, TensorShape out_shape
        ) : BaseLayer(in, out, in_shape, out_shape)
    {
        // Initialization
    }

    int run() override;
};


int FusionAddLayer::run()
{
    LOGD("# FusionAddLayer run!\n");

    for (int h = 0; h < _output_shape.h; h++)
    {
        for (int w = 0; w < _output_shape.w; w++)
        {
            for (int c = 0; c < _output_shape.c; c++)
            {
                int out_idx = c + w * _output_shape.c + h * _output_shape.c * _output_shape.w;

                (*_output)[out_idx] = (*_output)[out_idx] + (*_input)[c];
            }
        }
    }

    return 0;
}


#endif //__FUSIONLAYER_H