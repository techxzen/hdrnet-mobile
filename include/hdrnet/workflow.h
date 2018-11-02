

#ifndef __WORKFLOW_H
#define __WORKFLOW_H

int get_grid(float * in, float * out);

//int generate_guide_map(float * rgb_input, float * guided_map);
int generate_guide_map(float * rgb_input);

int apply_slicing_layer_and_assemble();

#endif //__WORKFLOW_H