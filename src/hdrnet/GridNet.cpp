
#include "hdrnet/GridNet.h"
#include "utils/Utils.h"
#include "cnn/ConvolutionLayer.h"
#include "cnn/FCLayer.h"
#include "cnn/ReLULayer.h"
#include "cnn/TransposeLayer.h"
#include "cnn/FusionAddLayer.h"
#include "helper.h"

#include <string>


GridNet::GridNet(const char * model_dir):_model_dir(model_dir)
{
    _input_ptr_ptr = &_input_ptr;
    _output_ptr_ptr = &_output_ptr;

    /* Allocate tmp buffer */
    _buf1 = new float [1 * 8 * 128 * 128];
    _buf2 = new float [1 * 16 * 64 * 64];
    _buf3 = new float [1 * 64 * 16 * 16];

    // construct network
    /* kh, kw, ph, pw, sh, sw */
    int kh, kw, ph1, ph2, pw1, pw2, sh, sw;
    bool relu_flag;
    bool bias_flag;

    _layers.clear();

    /* Low level features */
    ConvolutionLayer * layer1 = new ConvolutionLayer(_input_ptr_ptr, &_buf1, {1,256,256,3}, {1,128,128,8}, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv1-weights.float32-3x3x3x8").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv1-biases.float32-8").c_str());
    _layers.push_back( layer1 );

    ConvolutionLayer * layer2 = new ConvolutionLayer(&_buf1, &_buf2, {1,128,128,8}, {1,64,64,16 }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv2-weights.float32-3x3x8x16").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv2-biases.float32-16").c_str());
    _layers.push_back( layer2 );

    ConvolutionLayer * layer3 = new ConvolutionLayer(&_buf2, &_buf1, {1,64,64,16 }, {1,32,32,32 },
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv3-weights.float32-3x3x16x32").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv3-biases.float32-32").c_str());
    _layers.push_back( layer3 );

    ConvolutionLayer * layer4 = new ConvolutionLayer(&_buf1, &_buf2, {1,32,32,32 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv4-weights.float32-3x3x32x64").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv4-biases.float32-64").c_str());
    _layers.push_back( layer4 );

    /* Local features */
    // use buf2 as input, buf3 as output
    ConvolutionLayer * layer5 = new ConvolutionLayer(&_buf2, &_buf1, {1,16,16,64 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=1, ph2=1, pw1=1, pw2=1, sh=1, sw=1, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-local-conv1-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-local-conv1-biases.float32-64").c_str());
    _layers.push_back( layer5 );

    ConvolutionLayer * layer6 = new ConvolutionLayer(&_buf1, &_buf3, {1,16,16,64 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=1, ph2=1, pw1=1, pw2=1, sh=1, sw=1, bias_flag=false, relu_flag=false,
        get_model_path_string("inference-coefficients-local-conv2-weights.float32-3x3x64x64").c_str(), 
        nullptr);
    _layers.push_back( layer6 );

    /* Global features */
    // also use buf2 as input, buf1 as output
    ConvolutionLayer * layer7 = new ConvolutionLayer(&_buf2, &_buf1, {1,16,16,64 }, {1,8,8,64   },
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-conv1-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-global-conv1-biases.float32-64").c_str());
    _layers.push_back( layer7 );

    ConvolutionLayer * layer8 = new ConvolutionLayer(&_buf1, &_buf2, {1,8,8,64  }, {1,4,4,64    }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-conv2-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-global-conv2-biases.float32-64").c_str());
    _layers.push_back( layer8 );

    FCLayer * layer10 = new FCLayer(&_buf2, &_buf1, {1,1,1,1024}, {1,1,1,256}, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-fc1-weights.float32-1024x256").c_str(), 
        get_model_path_string("inference-coefficients-global-fc1-biases.float32-256").c_str());
    _layers.push_back( layer10 );

    FCLayer * layer11 = new FCLayer(&_buf1, &_buf2, {1,1,1,256}, {1,1,1,128}, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-fc2-weights.float32-256x128").c_str(), 
        get_model_path_string("inference-coefficients-global-fc2-biases.float32-128").c_str());
    _layers.push_back( layer11 );

    FCLayer * layer12 = new FCLayer(&_buf2, &_buf1, {1,1,1,128}, {1,1,1,64}, bias_flag=true, relu_flag=false,
        get_model_path_string("inference-coefficients-global-fc3-weights.float32-128x64").c_str(), 
        get_model_path_string("inference-coefficients-global-fc3-biases.float32-64").c_str());
    _layers.push_back( layer12 );

    /* Fusion of local and global features */
    // merge buf3 and buf2 to buf2, and relu to buf1
    FusionAddLayer * layer13 = new FusionAddLayer(&_buf1, &_buf3, {1,1,1,64}, {1,16,16,64});
    _layers.push_back( layer13 );

    ReLULayer * layer14 = new ReLULayer(&_buf3, &_buf1, {1,16,16,64}, {1,16,16,64});
    _layers.push_back( layer14 );

    /* Prediction */
    ConvolutionLayer * layer15 = new ConvolutionLayer(&_buf1, &_buf2, {1,16,16,64}, {1,16,16,96}, 
        kh=1, kw=1, ph1=0, ph2=0, pw1=0, pw2=0, sh=1, sw=1, bias_flag=true, relu_flag=false,
        get_model_path_string("inference-coefficients-prediction-conv1-weights.float32-1x1x64x96").c_str(), 
        get_model_path_string("inference-coefficients-prediction-conv1-biases.float32-96").c_str());
    _layers.push_back( layer15 );

    /* Transpose */
    // 16x16x12x8 -> 16x16x8x12
    TransposeLayer * layer16 = new TransposeLayer(&_buf2, _output_ptr_ptr, {256, 4, 3, 8}, {256, 8, 3, 4},
        0, 3, 2, 1);
    _layers.push_back( layer16 );

}


GridNet::~GridNet()
{
    for (auto it = _layers.begin(); it != _layers.end(); it++)
    {
        delete (*it);
    }

    _layers.clear();

    delete [] _buf1;
    delete [] _buf2;
    delete [] _buf3;

}


int GridNet::run_network(float * input, float * output)
{
    _input_ptr = input;
    _output_ptr = output;

    // run network
    for (auto it = _layers.begin(); it != _layers.end(); it++)
    {
        (*it)->run();
    }

    return 0;
}

