
import tensorflow as tf


ckpt_name = "local_laplacian_strong_1024"
ckpt_file = "/home/chen/myworkspace/projects/sample_data/pretrained_models/local_laplacian/strong_1024/upgraded_from_sig2017.ckpt-0"
dest_path = '/home/chen/myworkspace/projects/sample_data/pretrained_models/local_laplacian/strong_1024/binaries/'


reader = tf.train.NewCheckpointReader(ckpt_file) 
var_dict = reader.get_variable_to_shape_map()

var_tuple = sorted(var_dict.items(), key = lambda v:v[0], reverse=False)

for key, shape in var_tuple:
    include = "inference"
    #include = "splat"
    #include = "guide"
    exclude = "Adam"
    if ((include in key) and (exclude == '' or exclude not in key)):
        print('')
        ''' get file_name string '''
        file_name = key.replace('/', '-')
        ''' get shape string '''
        shape_str = ''
        for i in range(len(shape) - 1):
            shape_str += '%dx' %(shape[i])
        shape_str += '%d' %(shape[len(shape) - 1])

        print((key, shape))
        print((file_name, shape_str))
        weights = reader.get_tensor(key)
        print(type(weights.dtype))
        print(weights.dtype)
        ''' save to file '''
        #weights.tofile(dest_path + file_name + '.float32' + '-' + shape_str)

print(include)