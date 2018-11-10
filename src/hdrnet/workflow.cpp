/*
*******************************************************************************
*       Filename:  workflow.cpp
*    Description:  cpp file
*       
*        Version:  1.0
*        Created:  2018-10-29
*         Author:  chencheng
*
*        History:  initial draft
*******************************************************************************
*/
#include "hdrnet/workflow.h"
#include "cnn/ConvolutionLayer.h"
#include "cnn/FCLayer.h"
#include "cnn/ReLULayer.h"
#include "cnn/TransposeLayer.h"
#include "cnn/FusionAddLayer.h"

#include <vector>
#include <string>
#include <algorithm>
#include "utils/Utils.h"


inline std::string get_model_path_string(const char * relative_path)
{
    return (std::string(MODEL_DIR) + std::string(relative_path)).c_str();
}

int generate_bilateral_grid(float * in, float * out)
{
    LOGD("# Run AI ...\n");

    std::vector<ILayer *> layers;

    float * in_buf = in;
    float * out_buf = out;

    /* Allocate tmp buffer */
    float * buf1 = new float [1 * 8 * 128 * 128];
    float * buf2 = new float [1 * 16 * 64 * 64];
    float * buf3 = new float [1 * 64 * 16 * 16];

    // construct network
    /* kh, kw, ph, pw, sh, sw */
    int kh, kw, ph1, ph2, pw1, pw2, sh, sw;
    bool relu_flag;
    bool bias_flag;

    /* Low level features */
    ConvolutionLayer layer1 = ConvolutionLayer(in_buf, buf1, {1,256,256,3}, {1,128,128,8}, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv1-weights.float32-3x3x3x8").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv1-biases.float32-8").c_str());
    layers.push_back( &layer1 );

    ConvolutionLayer layer2 = ConvolutionLayer(buf1, buf2, {1,128,128,8}, {1,64,64,16 }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv2-weights.float32-3x3x8x16").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv2-biases.float32-16").c_str());
    layers.push_back( &layer2 );

    ConvolutionLayer layer3 = ConvolutionLayer(buf2, buf1, {1,64,64,16 }, {1,32,32,32 },
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv3-weights.float32-3x3x16x32").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv3-biases.float32-32").c_str());
    layers.push_back( &layer3 );

    ConvolutionLayer layer4 = ConvolutionLayer(buf1, buf2, {1,32,32,32 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-splat-conv4-weights.float32-3x3x32x64").c_str(), 
        get_model_path_string("inference-coefficients-splat-conv4-biases.float32-64").c_str());
    layers.push_back( &layer4 );

    /* Local features */
    // use buf2 as input, buf3 as output
    ConvolutionLayer layer5 = ConvolutionLayer(buf2, buf1, {1,16,16,64 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=1, ph2=1, pw1=1, pw2=1, sh=1, sw=1, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-local-conv1-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-local-conv1-biases.float32-64").c_str());
    layers.push_back( &layer5 );

    ConvolutionLayer layer6 = ConvolutionLayer(buf1, buf3, {1,16,16,64 }, {1,16,16,64 }, 
        kh=3, kw=3, ph1=1, ph2=1, pw1=1, pw2=1, sh=1, sw=1, bias_flag=false, relu_flag=false,
        get_model_path_string("inference-coefficients-local-conv2-weights.float32-3x3x64x64").c_str(), 
        nullptr);
    layers.push_back( &layer6 );

    /* Global features */
    // also use buf2 as input, buf1 as output
    ConvolutionLayer layer7 = ConvolutionLayer(buf2, buf1, {1,16,16,64 }, {1,8,8,64   },
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-conv1-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-global-conv1-biases.float32-64").c_str());
    layers.push_back( &layer7 );

    ConvolutionLayer layer8 = ConvolutionLayer(buf1, buf2, {1,8,8,64  }, {1,4,4,64    }, 
        kh=3, kw=3, ph1=0, ph2=1, pw1=0, pw2=1, sh=2, sw=2, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-conv2-weights.float32-3x3x64x64").c_str(), 
        get_model_path_string("inference-coefficients-global-conv2-biases.float32-64").c_str());
    layers.push_back( &layer8 );

    FCLayer layer10 = FCLayer(buf2, buf1, {1,1,1,1024}, {1,1,1,256}, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-fc1-weights.float32-1024x256").c_str(), 
        get_model_path_string("inference-coefficients-global-fc1-biases.float32-256").c_str());
    layers.push_back( &layer10 );

    FCLayer layer11 = FCLayer(buf1, buf2, {1,1,1,256}, {1,1,1,128}, bias_flag=true, relu_flag=true,
        get_model_path_string("inference-coefficients-global-fc2-weights.float32-256x128").c_str(), 
        get_model_path_string("inference-coefficients-global-fc2-biases.float32-128").c_str());
    layers.push_back( &layer11 );

    FCLayer layer12 = FCLayer(buf2, buf1, {1,1,1,128}, {1,1,1,64}, bias_flag=true, relu_flag=false,
        get_model_path_string("inference-coefficients-global-fc3-weights.float32-128x64").c_str(), 
        get_model_path_string("inference-coefficients-global-fc3-biases.float32-64").c_str());
    layers.push_back( &layer12 );

    /* Fusion of local and global features */
    // merge buf3 and buf2 to buf2, and relu to buf1
    FusionAddLayer layer13 = FusionAddLayer(buf1, buf3, {1,1,1,64}, {1,16,16,64});
    layers.push_back( &layer13 );

    ReLULayer layer14 = ReLULayer(buf3, buf1, {1,16,16,64}, {1,16,16,64});
    layers.push_back( &layer14 );

    /* Prediction */
    ConvolutionLayer layer15 = ConvolutionLayer(buf1, buf2, {1,16,16,64}, {1,16,16,96}, 
        kh=1, kw=1, ph1=0, ph2=0, pw1=0, pw2=0, sh=1, sw=1, bias_flag=true, relu_flag=false,
        get_model_path_string("inference-coefficients-prediction-conv1-weights.float32-1x1x64x96").c_str(), 
        get_model_path_string("inference-coefficients-prediction-conv1-biases.float32-96").c_str());
    layers.push_back( &layer15 );

    /* Transpose */
    // 16x16x12x8 -> 16x16x8x12
    TransposeLayer layer16 = TransposeLayer(buf2, out_buf, {256, 4, 3, 8}, {256, 8, 3, 4},
        0, 3, 2, 1);
    layers.push_back( &layer16 );

    // run network
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        (*it)->run();
    }

    /* Free buffer */
    delete [] buf1;
    delete [] buf2;
    delete [] buf3;

    return 0;
}


int generate_guide_map(
    float * full_res, //full res HWC
    float * guide_out, //full res
    int height,
    int width 
    )
{
#ifdef DEBUG_COMPARE
    float * guide_ref = new float [2048 * 2048];
#endif

    // param
    float * ccm = new float [3 * 3];
    float * ccm_bias = new float [3];
    float * shifts = new float [1 * 1 * 3 * 16];
    float * slopes = new float [1 * 1 * 1 * 3 * 16];
    float * channel_mix_weight = new float [3];
    float * channel_mix_bias = new float [1];

#ifdef DEBUG_COMPARE
    // read data
    read_data_from_file(guide_ref, 16 * 16 * 8 * 3 * 4, "/home/chen/myworkspace/projects/sample_data/temp/guide_2048x2048.float32");
#endif

    read_data_from_file(ccm, 3 * 3, get_model_path_string("inference-guide-ccm.float32-3x3").c_str());

    read_data_from_file(ccm_bias, 3, get_model_path_string("inference-guide-ccm_bias.float32-3").c_str());
    
    read_data_from_file(shifts, 1 * 1 * 3 * 16, get_model_path_string("inference-guide-shifts.float32-1x1x3x16").c_str());
    
    read_data_from_file(slopes, 1 * 1 * 1 * 3 * 16, get_model_path_string("inference-guide-slopes.float32-1x1x1x3x16").c_str());

    read_data_from_file(channel_mix_weight, 1 * 1 * 3 * 1, get_model_path_string("inference-guide-channel_mixing-weights.float32-1x1x3x1").c_str());

    read_data_from_file(channel_mix_bias, 1, get_model_path_string("inference-guide-channel_mixing-biases.float32-1").c_str());

    // run
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float r, g, b;
            r = full_res[h * width * 3 + w * 3 + 0];
            g = full_res[h * width * 3 + w * 3 + 1];
            b = full_res[h * width * 3 + w * 3 + 2];

            /* use ccm, create new r, g, b value 
               transpose([new_r, new_g, new_b]) = [r,g,b] * ccm + bias
            */
            float new_r, new_g, new_b;

            new_r = ccm[0] * r + ccm[3] * g + ccm[6] * b + ccm_bias[0];
            new_g = ccm[1] * r + ccm[4] * g + ccm[7] * b + ccm_bias[1];
            new_b = ccm[2] * r + ccm[5] * g + ccm[8] * b + ccm_bias[2];

            /* use slope and shifts per channel */
            float guide_r = 0;
            float guide_g = 0;
            float guide_b = 0;
            for (int i = 0; i < 16; i++)
            {
                guide_r += slopes[i] * std::max(new_r - shifts[i], float(0));
                guide_g += slopes[i + 16] * std::max(new_g - shifts[i + 16], float(0)); 
                guide_b += slopes[i + 32] * std::max(new_b - shifts[i + 32], float(0));
            }

            /* channel mix */
            float guide_value = 0;
            guide_value = channel_mix_weight[0] * guide_r + 
                channel_mix_weight[1] * guide_g + 
                channel_mix_weight[2] * guide_b + channel_mix_bias[0];
            guide_out[h * width + w] = guide_value;
        }
    }

#ifdef DEBUG_COMPARE
    {
        printf("guide_out:\n");
        for (int i = 0; i < 10; i++)
        {
            printf("%f ", guide_out[i]);
        }
        printf("\n");

        printf("guide_ref:\n");
        for (int i = 0; i < 10; i++)
        {
            printf("%f ", guide_ref[i]);
        }
        printf("\n");
    }
    // free
    delete [] guide_ref;
#endif

    delete [] ccm;
    delete [] ccm_bias;
    delete [] shifts;
    delete [] slopes;
    delete [] channel_mix_bias;
    delete [] channel_mix_weight;

    return 0;
}


int apply_slicing_layer_and_assemble(
    float * img_in, // full_res, HWC format
    float * grid, // coeeff, 16x16x8x3x4
    float * guide_map, // guide map
    float * img_out,
    int height,
    int width
    )
{
#ifdef DEBUG_COMPARE
    float * out_ref = new float [2048 * 2048 * 3];
    read_data_from_file(out_ref, 3 * 2048 * 2048, "/home/chen/myworkspace/projects/sample_data/temp/out_2048x2048x3.float32");
#endif

    /* Begin to process. */
    int grid_height = 16;
    int grid_width  = 16;
    int grid_depth  = 8;
    int chans = 12;

    int h_scale = grid_width * grid_depth * chans;
    int w_scale = grid_depth * chans;
    int d_scale = chans;

    float * chans_values = new float [chans];


    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            for (int i = 0; i < chans; i++)
            {
                chans_values[i] = 0;
            }

            float gh = (h + 0.5f) * grid_height / (1.0f * height) - 0.5f;
            float gw = (w + 0.5f) * grid_width  / (1.0f * width)  - 0.5f;
            //gd = (guide_map[h * width + w] + 0.5f) * grid_depth / 1.0f - 0.5f; 
            float gd = guide_map[h * width + w] * grid_depth - 0.5f;//according to the cuda code

            /* The neighboring position */
            int fh = static_cast<int>(floor(gh));
            int fw = static_cast<int>(floor(gw));
            int fd = static_cast<int>(floor(gd));

            /* The neighboring 8 values, tri-linear interpolation */
            for (int hh = fh; hh < fh + 2; hh++)
            {
                int h_idx     = std::max(std::min(hh, grid_height - 1), 0);
                float h_ratio = std::max(1.0f - std::abs(gh - hh), 0.0f);

                for (int ww = fw; ww < fw + 2; ww++)
                {
                    int w_idx     = std::max(std::min(ww, grid_width - 1), 0);
                    float w_ratio = std::max(1.0f - std::abs(gw - ww), 0.0f);

                    for (int dd = fd; dd < fd + 2; dd++)
                    {
                        int d_idx = std::max(std::min(dd, grid_depth - 1), 0);
                        float d_ratio = std::max(1.0f - std::abs(gd - dd), 0.0f);
                        for (int c = 0; c < chans; c++)
                        {
                            int grid_idx = h_idx * h_scale + w_idx * w_scale + d_idx * d_scale + c;
                            chans_values[c] += grid[grid_idx] * h_ratio * w_ratio * d_ratio;
                        }
                    }
                }
            }

            /* RGB format is HWC */
            float r = img_in[h * width * 3 + w * 3 + 0];
            float g = img_in[h * width * 3 + w * 3 + 1];
            float b = img_in[h * width * 3 + w * 3 + 2];

            img_out[h * width * 3 + w * 3 + 0] = chans_values[0] * r + chans_values[1] * g + chans_values[2] * b + chans_values[3];
            img_out[h * width * 3 + w * 3 + 1] = chans_values[4] * r + chans_values[5] * g + chans_values[6] * b + chans_values[7];
            img_out[h * width * 3 + w * 3 + 2] = chans_values[8] * r + chans_values[9] * g + chans_values[10] * b + chans_values[11];

            //return 0;
        }
    }

    delete [] chans_values;

#ifdef DEBUG_COMPARE
    /* Test */
    printf("img_out:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%f ", img_out[i]);
    }
    printf("\n");

    printf("out_ref:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%f ", out_ref[i]);
    }
    printf("\n");

    delete [] out_ref;
#endif

    return 0;
}