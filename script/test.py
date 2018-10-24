import tensorflow as tf
  
w = tf.Variable(2, dtype=tf.float32, name='w')  
b = tf.Variable(1, dtype=tf.float32, name='b')  

x = tf.placeholder(tf.float32, shape=[1], name='x')  
  
logit = w * x + b
  
init = tf.initialize_all_variables()  
  
saver = tf.train.Saver()  
  
with tf.Session() as sess:  
    sess.run(init)  
    saver.save(sess, "./model/model.ckpt") 