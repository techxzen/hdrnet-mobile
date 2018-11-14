#### 典型tensorflow模型文件夹

![tf_model_dir](/home/chen/myworkspace/projects/hdrnet-mobile/doc/img/tensorflow_model_dir.png)

包括4个文件：

1. checkpoint  是检查点文件，文件保存了一个目录下所有的模型文件列表；
2. model.ckpt.meta文件保存了TensorFlow计算图的结构，可以理解为神经网络的网络结构，该文件可以被 tf.train.import_meta_graph 加载到当前默认的图来使用
3. ckpt.data保存模型中每个变量的取值


#### Reference model

laplacian strong 2014 tensorflow model

modelname = HDRNetCurves