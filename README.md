
## hdrnet-mobile
#### Introduction

A C++ implementation of inference part of the paper: *Gharbi, Michaël, et al. "Deep bilateral learning for real-time image enhancement."  ACM Transactions on Graphics (TOG) 36.4 (2017): 118*.

#### Usage

1. create binary model dir from tensorflow ( [pretrained models](https://data.csail.mit.edu/graphics/hdrnet/pretrained_models.zip) )
```shell
python script/get_tfmodel_value.py
```

2. build shared library and c++ executable 
```shell
make -B
```

3. add libhdrnet_api.so to LD_LIBRARY_PATH
```shell
export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
```

4. test a picture
```shell
python script/run.py A_PICTURE(like xxx.jpg) your_binary_model_dir
```

#### To be done
1. optimize the c++ code
2. create an Android NDK version
3. use OpenCL to accelerate
4. retrain the hdrnet+ model, change it to a post process tensorflow model


#### Reference
1. [MIT hdrnet's webpage](https://groups.csail.mit.edu/graphics/hdrnet/)
2. [Gharbi's hdrnet_legacy(forked)](https://github.com/itchencheng/hdrnet_legacy)
3. [Pretrained models](https://data.csail.mit.edu/graphics/hdrnet/pretrained_models.zip)



