
#ifndef __FCLAYER_H
#define __FCLAYER_H

#include "cnn/BaseLayer.h"


class FCLayer final : public BaseLayer
{
public:
    FCLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape, 
        ) : BaseLayer(in, out, in_shape, out_shape)
    {
        // Initialization
    }

    int run() override;

public:
    float * _weight;
    float * _bias;
};


int FCLayer::run()
{
    LOGD("# FCLayer run!\n");
    
    int in_nodes_num  = _input_shape.w;
    int out_nodes_num = _output_shape.w;

    for (int o = 0; o < out_nodes_num; o++)
    {
        float result = 0;
        
        for (int i = 0; i < in_nodes_num; i++)
        {
            result += _input[i] * _weight[o * in_nodes_num + i];
        }

        result += _bias[o];

        _output[i] = result;
    }
    return 0;
}


#endif //__FCLAYER_H