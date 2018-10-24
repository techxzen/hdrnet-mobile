
import tensorflow as tf


ckpt_name = "local_laplacian_strong_1024"
ckpt_file = "/home/chen/myworkspace/projects/sample_data/pretrained_models/local_laplacian/strong_1024/upgraded_from_sig2017.ckpt-0"


reader = tf.train.NewCheckpointReader(ckpt_file) 
var_dict = reader.get_variable_to_shape_map()

var_tuple = sorted(var_dict.items(), key = lambda v:v[0], reverse=False)

for key, value in var_tuple:
    include = "splat"
    exclude = ""
    if ((include in key) and (exclude == '' or exclude not in key)):
        print('')
        print((key, value))
        weights = reader.get_tensor(key)
        print(type(weights))
        #print(value)
print(include)