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
#include <vector>

#include <string>
#include <algorithm>
#include "utils/Utils.h"


int get_grid(float * in, float * out)
{
    std::vector<BaseLayer *> layers;

    // Create Buffer
    float * buf1 = in;
    float * buf2 = out;

    // construct network
    ConvolutionLayer layer1 = ConvolutionLayer(buf1, buf2, {1,2,3,4}, {1,2,3,4}, 1, 1, 1, 1, 1, 1);
    layers.push_back( &layer1 );

    ConvolutionLayer layer2 = ConvolutionLayer(buf2, buf1, {2,2,3,4}, {1,2,3,4}, 1, 1, 1, 1, 1, 1);
    layers.push_back( &layer2 );


    // run network
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        (*it)->run();
    }

    return 0;
}


int generate_guide_map(float * full_res)
{
    int height = 2048;
    int width  = 2048;

    // allocate 
    float * coeff = new float [16 * 16 * 8 * 3 * 4];
    float * guide_out = new float [2048 * 2048];
    float * guide_ref = new float [2048 * 2048];

    // param
    float * ccm = new float [3 * 3];
    float * ccm_bias = new float [3];
    float * shifts = new float [1 * 1 * 3 * 16];
    float * slopes = new float [1 * 1 * 1 * 3 * 16];
    float * channel_mix_weight = new float [3];
    float * channel_mix_bias = new float [1];

    // read data
    read_data_from_file(coeff, 16 * 16 * 8 * 3 * 4, "/home/chen/myworkspace/projects/sample_data/temp/coeffs_16x16x8x3x4.float32");
    read_data_from_file(guide_ref, 16 * 16 * 8 * 3 * 4, "/home/chen/myworkspace/projects/sample_data/temp/guide_2048x2048.float32");

    std::string binary_model_dir = "/home/chen/myworkspace/projects/sample_data/pretrained_models/local_laplacian/strong_1024/binaries/";
    std::string file_path = "";

    file_path = binary_model_dir + "inference-guide-ccm.float32-3x3";
    read_data_from_file(ccm, 3 * 3, file_path.c_str());

    file_path = binary_model_dir + "inference-guide-ccm_bias.float32-3";
    read_data_from_file(ccm_bias, 3, file_path.c_str());
    
    file_path = binary_model_dir + "inference-guide-shifts.float32-1x1x3x16";
    read_data_from_file(shifts, 1 * 1 * 3 * 16, file_path.c_str());
    
    file_path = binary_model_dir + "inference-guide-slopes.float32-1x1x1x3x16";
    read_data_from_file(slopes, 1 * 1 * 1 * 3 * 16, file_path.c_str());

    file_path = binary_model_dir + "inference-guide-channel_mixing-weights.float32-1x1x3x1";
    read_data_from_file(channel_mix_weight, 1 * 1 * 3 * 1, file_path.c_str());

    file_path = binary_model_dir + "inference-guide-channel_mixing-biases.float32-1";
    read_data_from_file(channel_mix_bias, 1, file_path.c_str());

    // run
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float r, g, b;
            r = full_res[h * width + w];
            g = full_res[h * width + w + height * width];
            b = full_res[h * width + w + height * width * 2];

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
    delete [] guide_out;
    delete [] coeff;

    delete [] ccm;
    delete [] ccm_bias;
    delete [] shifts;
    delete [] slopes;
    delete [] channel_mix_bias;
    delete [] channel_mix_weight;

    return 0;
}