

#include "hdrnet/flow.h"

int get_grid(float * in, float * out)
{
    std::vector<BaseLayer *> layers;

    // construct network
    ConvolutionLayer layer1 = ConvolutionLayer(in, out, {1,2,3,4}, {1,2,3,4}, 1,1,1,1,1,1);
    layers.push_back( &layer1 );

    // run network
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        (*it)->run();
    }

    return 0;
}