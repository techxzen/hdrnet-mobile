
### hdrnet-mobile
#### Intro

A C++ implementation of inference part of the essay: *Gharbi, Michaël, et al. "Deep bilateral learning for real-time image enhancement."  ACM Transactions on Graphics (TOG) 36.4 (2017): 118*.

#### Usage

1. create binary model dir from tensorflow

`python script/get_tfmodel_value.py A_TENSORFLOW_DIR`

2. set binary model dir in makefile

`MODEL_DIR=YOUR_OWN_BINARY_MODEL_DIR`

3. build c++ executable 

`make -B`

4. test a picture

`python script/run.py A_PICTURE(like xxx.jpg)`


#### To be done
1. optimize the c++ code
2. build shared library，change the main to a version using shared library
3. create an Android NDK version
4. use OpenCL to accelerate
5. retrain the hdrnet+ model, change it to a post process tensorflow model


#### Reference model
laplacian strong 2014 tensorflow model

modelname = HDRNetCurves

