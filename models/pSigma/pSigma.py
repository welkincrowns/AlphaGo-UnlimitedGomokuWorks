import tensorflow as tf
import numpy as np

def random_value(shape):
	return tf.random_normal(shape = shape, mean = 0, stddev = 0.05, dtype = tf.float32)
def weight_variable(shape):
	return tf.Variable(random_value(shape), name = 'weight')
def biases_variable(shape):
	return tf.Variable(tf.constant(0.1, shape = shape), name = 'biases')
def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def nn_layer(batch_size, input_tensor, weight_shape, num_filter, layer_name, act = tf.nn.relu, nd_reshape = False):
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

def read_data(filename):
	fp = open(filename);
	go = []
	for linea in fp.readlines():
		go.append(int(linea.replace("\n","")))

	board = np.zeros((15, 15))
	feature = np.zeros((15, 15, 5))
	data_state = np.zeros((1, 15, 15, 5))
	turn = 1

	tot = 0

	for i in range(len(go) / 2):
		x = go[2 * i]
		y = go[2 * i + 1]
		board[x - 1][y - 1] = turn
		turn = 3 - turn

	for i in range(15):
		for j in range(15):
			feature[i, j, 0] = 0
			feature[i, j, 1] = 1
			if (abs(board[i, j] - turn) < 1e-9):
				feature[i, j, 2] = 1
			else:
				feature[i, j, 2] = 0

			if (abs(board[i, j] - (3 - turn)) < 1e-9):
				feature[i, j, 3] = 1
			else:
				feature[i, j, 3] = 0

			if (abs(board[i, j]) < 1e-9):
				feature[i, j, 4] = 1
			else:
				feature[i, j, 4] = 0
	data_state[0] = feature
	return data_state

def build_graph():
	with tf.name_scope('input'):
		x = tf.placeholder(tf.float32, [1, 15, 15, 5], 'input_layer')

	# neural network
	hidden_conv_layer1 = nn_layer(1, x, [5, 5, 5, 192], 192, 'conv_layer_1')
	hidden_conv_layer2 = nn_layer(1, hidden_conv_layer1, [3, 3, 192, 24], 24, 'conv_layer_2')
	hidden_conv_layer3 = nn_layer(1, hidden_conv_layer2, [3, 3, 24, 24], 24, 'conv_layer_3')
	hidden_conv_layer4 = nn_layer(1, hidden_conv_layer3, [3, 3, 24, 24], 24, 'conv_layer_4')
	hidden_conv_layer5 = nn_layer(1, hidden_conv_layer4, [3, 3, 24, 24], 24, 'conv_layer_5')
	hidden_conv_layer6 = nn_layer(1, hidden_conv_layer5, [3, 3, 24, 24], 24, 'conv_layer_6')
	y = nn_layer(1, hidden_conv_layer6, [1, 1, 24, 1], 1, 'output_layer', tf.nn.softmax, True)
	v = tf.argmax(y, 1);
	return x, y, v

def player(filename):
	data = read_data(filename)
	# print data.shape

	saver = tf.train.Saver()
	init = tf.initialize_all_variables()

	# train
	with tf.Session() as sess:
		sess.run(init)
		saver.restore(sess, 'tmp/pSigma.ckpt')
		y_target, pos = sess.run([Y, POS], feed_dict={X: data})

	print pos
	xx = pos[0] / 15
	yy = pos[0] % 15

	#print '(', x + 1, y + 1, ')'
	return xx, yy, y_target

# If train delete the following words
X, Y, POS = build_graph()