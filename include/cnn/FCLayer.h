
#ifndef __FCLAYER_H
#define __FCLAYER_H

#include "cnn/BaseLayer.h"


class FCLayer final : public BaseLayer
{
public:
    FCLayer(float ** in, float ** out, TensorShape in_shape, TensorShape out_shape, bool bias_flag, bool relu_flag,
            const char * weight_file, const char * bias_file 
        ) : BaseLayer(in, out, in_shape, out_shape), _bias_flag(bias_flag), _relu_flag(relu_flag)
    {
        // Initialization
        int weight_size = _output_shape.c * _input_shape.c;
        int bias_size = _output_shape.c;

        _weight = new float [weight_size];
        read_data_from_file(_weight, weight_size, weight_file);
        
        if (_bias_flag)
        {
            _bias = new float [bias_size];
            read_data_from_file(_bias, bias_size, bias_file);
        }
    }

    ~FCLayer()
    {
        delete [] _weight;

        if (_bias_flag)
        {
            delete [] _bias;
        }
    }

    int run() override;

public:
    bool _bias_flag;
    bool _relu_flag;
    float * _weight;
    float * _bias;

};


int FCLayer::run()
{
    LOGD("# FCLayer run!\n");

    int in_nodes_num  = _input_shape.c;
    int out_nodes_num = _output_shape.c;

    for (int o = 0; o < out_nodes_num; o++)
    {
        float result = 0;
        
        for (int i = 0; i < in_nodes_num; i++)
        {
            result += (*_input)[i] * _weight[i * out_nodes_num + o];
        }

        if (_bias_flag)
        {
            result += _bias[o];
        }

        if (_relu_flag)
        {
            result = (result > 0) ? result : 0;
        }

        (*_output)[o] = result;
    }

    return 0;
}


#endif //__FCLAYER_H