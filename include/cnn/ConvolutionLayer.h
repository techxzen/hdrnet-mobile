
#ifndef __CONVOLUTIONLAYER_H
#define __CONVOLUTIONLAYER_H

#include "cnn/BaseLayer.h"
#include "utils/Utils.h"

class ConvolutionLayer final : public BaseLayer
{
public:
    ConvolutionLayer(float * in, float * out, TensorShape in_shape, TensorShape out_shape, 
        int k_h, int k_w, int p_h1, int p_h2, int p_w1, int p_w2, int s_h, int s_w, bool bias_flag, bool relu_flag,
        const char * weight_file, const char * bias_file
        ) : BaseLayer(in, out, in_shape, out_shape), _kernel_h(k_h), _kernel_w(k_w), 
                        _pad_h_up(p_h1), _pad_h_down(p_h2), _pad_w_left(p_w1), _pad_w_right(p_w2), _stride_h(s_h), _stride_w(s_w),
                        _bias_flag(bias_flag), _relu_flag(relu_flag)
    {
        // Initialization
        int weight_size = _output_shape.c * _input_shape.c * k_h * k_w;
        int bias_size = _output_shape.c;

        _weight = new float [weight_size];
        _bias = new float [bias_size];

        read_data_from_file(_weight, weight_size, weight_file);

        read_data_from_file(_bias, bias_size, bias_file);
    }

    ~ConvolutionLayer()
    {
        delete [] _weight;
        delete [] _bias;
    }

    int run() override;

public:
    int _kernel_h;
    int _kernel_w;
    int _pad_h_up;
    int _pad_h_down;
    int _pad_w_left;
    int _pad_w_right;
    int _stride_h;
    int _stride_w;

    bool _bias_flag;
    bool _relu_flag;

    float * _weight;
    float * _bias;
};


int ConvolutionLayer::run()
{
    LOGD("# ConvolutionLayer Run!\n");

    int k_h_r1 = _kernel_h / 2;
    int k_h_r2 = _kernel_h / 2;
    int k_w_r1 = _kernel_w / 2;
    int k_w_r2 = _kernel_w / 2;

    for (int c = 0; c < _output_shape.c; c++)
    {
        for (int h = 0; h < _output_shape.h; h++)
        {
            for (int w = 0; w < _output_shape.w; w++)
            {
                int output_idx = c * _output_shape.h * _output_shape.w + h * _output_shape.w + w;

                float result = 0;
                
                /* Core computation begins. */
                for (int in_c = 0; in_c < _input_shape.c; in_c++)
                {
                    /* Per channel */
                    for (int y = -k_h_r1; y <= k_h_r2; y++)
                    {
                        for (int x = -k_w_r1; x <= k_w_r2; x++)
                        {
                            int in_h = (h * _stride_h) + (y + k_h_r1) - _pad_h_up;
                            int in_w = (w * _stride_w) + (x + k_w_r1) - _pad_w_left;

                            float in_value = 0;
                            float wt_value = 0;

                            /* get in_value */
                            if (in_w < 0 || in_w > (_input_shape.w - 1) || in_h < 0 || in_h > (_input_shape.h - 1))
                            {
                                in_value = 0;
                            }
                            else
                            {
                                int in_value_idx = in_c * _input_shape.h * _input_shape.w + in_h * _input_shape.w + in_w;
                                in_value = _input[in_value_idx];
                            }

                            /* get wt_value */
                            /*
                                if weight shape: (out_c x in_c x kh x kw)
                            int wt_value_idx = (c * _input_shape.c * _kernel_h * _kernel_w) + 
                                                (in_c * _kernel_w * _kernel_h) + 
                                                 * _kernel_w + 
                                                (x + k_w_r1);
                            */

                            /* 
                              if weight shape: (kh x kw x in_c x out_c)
                            */
                            int wt_value_idx = ((y + k_h_r1) * _kernel_w * _input_shape.c * _output_shape.c) + 
                                                (x + k_w_r1) * _input_shape.c * _output_shape.c + 
                                                in_c * _output_shape.c + 
                                                c;

                            wt_value = _weight[wt_value_idx];

                            result += in_value * wt_value;
                        }
                    }
                }

                if (_bias_flag)
                {
                    result += _bias[c];
                }

                /* If relu followed */
                if (_relu_flag)
                {
                    result = (result > 0) ? result : 0;
                }

                _output[output_idx] = result;
            }
        }
    }

    return 0;
}


#endif //__CONVOLUTIONLAYER_H