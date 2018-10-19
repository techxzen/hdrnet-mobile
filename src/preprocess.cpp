

#include "hdrnet/preprocess.h"

int normalize_data(RGBData<char> * char_data, RGBData<float> * float_data)
{
    
    for (int i = 0; i < float_data->get_size(); i++)
    {
        float_data->get_data_ptr()[i] = char_data->get_data_ptr()[i] / float(255.0);
    }

    return 0;
}