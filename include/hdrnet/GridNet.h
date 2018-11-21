
#ifndef __GRID_NET_H
#define __GRID_NET_H

#include "cnn/ILayer.h"
#include <vector>
#include <string>

class GridNet
{
public:
    GridNet(const char * model_dir);

    ~GridNet();

    int run_network(float * input, float * output);


protected:
    inline std::string get_model_path_string(const char * relative_path)
    {
        return (std::string(_model_dir) + std::string("/") + std::string(relative_path)).c_str();
    }

    const char * _model_dir;

private:
    std::vector<ILayer *> _layers;

    float ** _input_ptr_ptr;
    float ** _output_ptr_ptr;
    float * _input_ptr;
    float * _output_ptr;

    float * _buf1;
    float * _buf2;
    float * _buf3;
};

#endif