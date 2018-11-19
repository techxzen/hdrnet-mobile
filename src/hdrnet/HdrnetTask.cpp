
#include "hdrnet/HdrnetTask.h"
#include "utils/Utils.h"

#include <algorithm>
#include "helper.h"

int HdrnetTask::build_task()
{
    // build network
    _grid_net.build_network();

    // param
    _ccm = new float [3 * 3];
    _ccm_bias = new float [3];
    _shifts = new float [1 * 1 * 3 * 16];
    _slopes = new float [1 * 1 * 1 * 3 * 16];
    _channel_mix_weight = new float [3];
    _channel_mix_bias = new float [1];

    read_data_from_file(_ccm, 3 * 3, get_model_path_string("inference-guide-ccm.float32-3x3").c_str());

    read_data_from_file(_ccm_bias, 3, get_model_path_string("inference-guide-ccm_bias.float32-3").c_str());
    
    read_data_from_file(_shifts, 1 * 1 * 3 * 16, get_model_path_string("inference-guide-shifts.float32-1x1x3x16").c_str());
    
    read_data_from_file(_slopes, 1 * 1 * 1 * 3 * 16, get_model_path_string("inference-guide-slopes.float32-1x1x1x3x16").c_str());

    read_data_from_file(_channel_mix_weight, 1 * 1 * 3 * 1, get_model_path_string("inference-guide-channel_mixing-weights.float32-1x1x3x1").c_str());

    read_data_from_file(_channel_mix_bias, 1, get_model_path_string("inference-guide-channel_mixing-biases.float32-1").c_str());

    //buffer
    _lowres_img_hwc = new float [256 * 256 * 3];
    _bilateral_grid = new float [16 * 16 * 96];

    return 0;
}


int HdrnetTask::clean_task()
{
    // clean network
    _grid_net.clean_network();

    // release buffer
    delete [] _ccm;
    delete [] _ccm_bias;
    delete [] _shifts;
    delete [] _slopes;
    delete [] _channel_mix_bias;
    delete [] _channel_mix_weight;

    return 0;
}


int HdrnetTask::run_task(float * in, float * out, int height, int width)
{
    int size = height * width;
    float * guide_map = new float [size];

    resize(in, _lowres_img_hwc, height, width, 256, 256);

    generate_bilateral_grid(_lowres_img_hwc, _bilateral_grid);

    generate_guide_map(in, guide_map, height, width);

    apply_slicing_layer_and_assemble(in, _bilateral_grid, guide_map, out, height, width);

    delete [] guide_map;

    return 0;
}


int HdrnetTask::resize(float * src, float * dst, int src_height, int src_width, int dst_height, int dst_width)
{
    for (int h = 0; h < dst_height; h++)
    {
        for (int w = 0; w < dst_width; w++)
        {
            float crespond_h = (h + 0.5f) * src_height / (1.0f * dst_height) - 0.5f;
            float crespond_w = (w + 0.5f) * src_width / (1.0f * dst_width) - 0.5f;

            int floor_h = static_cast<int>(floor(crespond_h));
            int floor_w = static_cast<int>(floor(crespond_w));

            float value_r = 0.0f;
            float value_g = 0.0f;
            float value_b = 0.0f;

            for (int hh = floor_h; hh < floor_h + 2; hh++)
            {
                int h_idx     = std::max(std::min(hh, src_height - 1), 0);
                float h_ratio = std::max(1.0f - std::abs(crespond_h - hh), 0.0f);
                
                for (int ww = floor_w; ww < floor_w + 2; ww++)
                {
                    int w_idx     = std::max(std::min(ww, src_width - 1), 0);
                    float w_ratio = std::max(1.0f - std::abs(crespond_w - ww), 0.0f);
                
                    int in_idx = h_idx * src_width + w_idx;
                    value_r += src[in_idx * 3 + 0] * h_ratio * w_ratio;
                    value_g += src[in_idx * 3 + 1] * h_ratio * w_ratio;
                    value_b += src[in_idx * 3 + 2] * h_ratio * w_ratio;
                }
            }
                
            int dst_pos = h * dst_width + w;
                                
            dst[dst_pos * 3 + 0] = value_r;            
            dst[dst_pos * 3 + 1] = value_g;            
            dst[dst_pos * 3 + 2] = value_b;
        }
    }

    return 0;
}



int HdrnetTask::generate_bilateral_grid(
    float * in, 
    float * out
    )
{
    LOGD("# Run AI ...\n");

    _grid_net.run_network(in, out);

    return 0;
}


int HdrnetTask::generate_guide_map(
    float * full_res, //full res HWC
    float * guide_out, //full res
    int height,
    int width 
    )
{
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

            new_r = _ccm[0] * r + _ccm[3] * g + _ccm[6] * b + _ccm_bias[0];
            new_g = _ccm[1] * r + _ccm[4] * g + _ccm[7] * b + _ccm_bias[1];
            new_b = _ccm[2] * r + _ccm[5] * g + _ccm[8] * b + _ccm_bias[2];

            /* use slope and shifts per channel */
            float guide_r = 0;
            float guide_g = 0;
            float guide_b = 0;
            for (int i = 0; i < 16; i++)
            {
                guide_r += _slopes[i] * std::max(new_r - _shifts[i], float(0));
                guide_g += _slopes[i + 16] * std::max(new_g - _shifts[i + 16], float(0)); 
                guide_b += _slopes[i + 32] * std::max(new_b - _shifts[i + 32], float(0));
            }

            /* channel mix */
            float guide_value = 0;
            guide_value = _channel_mix_weight[0] * guide_r + 
                _channel_mix_weight[1] * guide_g + 
                _channel_mix_weight[2] * guide_b + _channel_mix_bias[0];
            guide_out[h * width + w] = guide_value;
        }
    }

    return 0;
}


int HdrnetTask::apply_slicing_layer_and_assemble(
    float * img_in, // full_res, HWC format
    float * grid, // coeeff, 16x16x8x3x4
    float * guide_map, // guide map
    float * img_out,
    int height,
    int width
    )
{
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

    return 0;
}
