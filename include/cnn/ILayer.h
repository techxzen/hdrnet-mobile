
#ifndef __ILAYER_H
#define __ILAYER_H

#include "helper.h"


struct TensorShape
{
    int n; /* #batch */
    int h; /* #height */
    int w; /* #width */
    int c; /* #channel */

    int size() const 
    {
        return n * h * w * c;
    }
};


class ILayer
{
public:
    ILayer() = default;
    virtual ~ILayer() = default;

    virtual int run() = 0;
};

#endif //__ILAYER_H