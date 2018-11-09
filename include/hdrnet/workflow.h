

#ifndef __WORKFLOW_H
#define __WORKFLOW_H

int generate_bilateral_grid(float * in, float * out);

int generate_guide_map(
    float * full_res, //full res HWC
    float * guide_out, //full res 
    int height,
    int width
    );

int apply_slicing_layer_and_assemble(
    float * img_in, // full_res, HWC format
    float * grid, // coeeff, 16x16x8x3x4
    float * guide_map, // guide map
    float * img_out,
    int height,
    int width
    );

#endif //__WORKFLOW_H