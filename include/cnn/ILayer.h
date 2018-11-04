
#ifndef __ILAYER_H
#define __ILAYER_H

#include "helper.h"


struct TensorShape
{
    int n; /* #batch */
    int c; /* #channel */
    int h; /* #height */
    int w; /* #width */

    int size() const 
    {
        return n * c * h * w;
    }
};


class ILayer
{
public:
    virtual int run() = 0;
};

#endif //__ILAYER_H