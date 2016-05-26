import tensorflow as tf

# Prepare the data
import input_data
batch_size = 500

gomoku = input_data.read_data(50000, 500, batch_size);

print gomoku.test.state.shape
if tf.gfile.Exists('/tmp/pSigma'):
	tf.gfile.DeleteRecursively('/tmp/pSigma')
tf.gfile.MakeDirs('/tmp/pSigma')
# Build the graph

# placehold and input
with tf.name_scope('input'):
	x = tf.placeholder(tf.float32, [None, 15, 15, 5], "input_layer")
	y_ = tf.placeholder(tf.float32, [None, 15 * 15], "y_label")
	dropoutp = tf.placeholder(tf.float32, [1], 'dropout_pro')

# neural network
def random_value(shape):
	return tf.random_normal(shape = shape, mean = 0, stddev = 0.05, dtype = tf.float32)
def weight_variable(shape):
	return tf.Variable(random_value(shape), name = 'weight')
def biases_variable(shape):
	return tf.Variable(tf.constant(0.1, shape = shape), name = 'biases')
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def nn_layer(input_tensor, weight_shape, num_filter, layer_name, act = tf.nn.relu, nd_reshape = False):
	with tf.name_scope(layer_name):
		with tf.name_scope('weights'):
			weights = weight_variable(weight_shape);
		with tf.name_scope('biases'):
			biases = biases_variable([num_filter])
		with tf.name_scope('conv_combination'):
			z = conv2d(input_tensor, weights) + biases
		with tf.name_scope('activation'):
			if nd_reshape:
				reshape = tf.reshape(z, [batch_size, 15 * 15])
				# a = tf.nn.dropout(act(reshape), 0.8)
				a = act(reshape)
			else:
				a = act(z)
		return a

hidden_conv_layer1 = nn_layer(x, [5, 5, 5, 192], 192, 'saber_arturia_conv_layer_1')
hidden_conv_layer2 = nn_layer(hidden_conv_layer1, [3, 3, 192, 24], 24, 'archer_emiya_conv_layer_2')
hidden_conv_layer3 = nn_layer(hidden_conv_layer2, [3, 3, 24, 24], 24, 'rider_medusa_conv_layer_3')
hidden_conv_layer4 = nn_layer(hidden_conv_layer3, [3, 3, 24, 24], 24, 'lancer_cuchulainn_onv_layer_4')
hidden_conv_layer5 = nn_layer(hidden_conv_layer4, [3, 3, 24, 24], 24, 'caster_medea_conv_layer_5')
hidden_conv_layer6 = nn_layer(hidden_conv_layer5, [3, 3, 24, 24], 24, 'berserker_hercules_conv_layer_6')
y = nn_layer(hidden_conv_layer6, [1, 1, 24, 1], 1, 'assassin_sasakikojiro_output_layer', tf.nn.softmax, True)

# results
with tf.name_scope('results'):
	loss = tf.reduce_mean(-tf.reduce_mean(y_ * tf.log(tf.clip_by_value(y, 1e-10, 1.0)), reduction_indices = 1))
	# loss = tf.reduce_mean(-tf.reduce_mean(y_ * tf.log(y), reduction_indices = 1));
	tf.scalar_summary('loss', loss)

with tf.name_scope('evaluation'):
	with tf.name_scope('correct_prediction'):
		correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
	with tf.name_scope('accuracy'):
		accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
	tf.scalar_summary('accuracy', accuracy)

# initialize

'''order = 'o'
while ((order != 'y') & (order != 'N')):
	order = raw_input('Continue the train last time? [y/N]')'''

train_step = tf.train.AdamOptimizer().minimize(loss)
saver = tf.train.Saver()
init = tf.initialize_all_variables()
merged = tf.merge_all_summaries()

max_acc = 0

# train
with tf.Session() as sess:
	sess.run(init)
	saver.restore(sess, '/tmp/pSigma.ckpt')
	y_pred = sess.run([y], feed_dict = {x : gomoku.test.state})
