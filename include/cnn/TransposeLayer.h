

#ifndef __TRANSPOSELAYER_H
#define __TRANSPOSELAYER_H

#include "cnn/BaseLayer.h"


class TransposeLayer final : public BaseLayer
{
public:
    TransposeLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape,
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


int TransposeLayer::run()
{
    LOGD("# TransposeLayer run!\n");

    int dims[4] = {_input_shape.n, _input_shape.h, _input_shape.w, _input_shape.c};
    int dim_res[4] = {0};
    dim_res[0] = _input_shape.h * _input_shape.w * _input_shape.c;
    dim_res[1] = _input_shape.w * _input_shape.c;
    dim_res[2] = _input_shape.c;
    dim_res[3] = 1;

    int out_idx = 0;
    for (int d0 = 0; d0 < dims[_idx[0]]; d0++)
    {
        for (int d1 = 0; d1 < dims[_idx[1]]; d1++)
        {
            for (int d2 = 0; d2 < dims[_idx[2]]; d2++)
            {
                for (int d3 = 0; d3 < dims[_idx[3]]; d3++)
                {
                    int in_idx = d0 * dim_res[_idx[0]] + d1 * dim_res[_idx[1]] + 
                                    d2 * dim_res[_idx[2]] + d3 * dim_res[_idx[3]];
                    _output[out_idx++] = _input[in_idx];
                }
            }
        }
    }

    return 0;
}


#endif //__TRANSPOSELAYER_H