import tensorflow as tfsb
import numpy as np

def random_value(shape):
	return tfsb.random_normal(shape = shape, mean = 0, stddev = 0.01, dtype = tfsb.float32)
def weight_variable(shape):
	return tfsb.Variable(random_value(shape), name = 'weight')
def biases_variable(shape):
	return tfsb.Variable(tfsb.constant(0.1, shape = shape), name = 'biases')
def conv2d(x, W):
	return tfsb.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def nn_layer(batch_size, input_tensor, weight_shape, num_filter, layer_name, act = tfsb.nn.relu, nd_reshape = False):
	with tfsb.name_scope(layer_name):
		with tfsb.name_scope('weights'):
			weights = weight_variable(weight_shape);
			psigb_variable_set[layer_name + '/' + 'weights/weight' + ':0'] = weights
		with tfsb.name_scope('biases'):
			biases = biases_variable([num_filter])
			psigb_variable_set[layer_name + '/' + 'biases/biases' + ':0'] = biases
		with tfsb.name_scope('conv_combination'):
			z = conv2d(input_tensor, weights) + biases
		with tfsb.name_scope('activation'):
			if nd_reshape:
				reshape = tfsb.reshape(z, [batch_size, 15 * 15])
				# a = tfsb.nn.dropout(act(reshape), 0.8)
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

def load_player(filename):
	psigb_saver.restore(psigb_sess, filename)
	return 'w'

def player(filename_input, filename_output):
	data = read_data(filename_input)

	# play
	y_target, pos = psigb_sess.run([psigb_Y, psigb_POS], feed_dict={psigb_X: data})

	# write the distribution
	dis_file_object = open(filename_output, 'w+')
	for i in range(15 * 15):
		dis_file_object.write(str(y_target[0, i]) + '\n')
	dis_file_object.close()

	# print the distribution
	for i in range(15):
		for j in range(15):
			print i, j, y_target[0, i * 15 + j]
			
	# return the answer (for demo)
	xx = pos[0] / 15
	yy = pos[0] % 15
	return xx, yy, y_target

with tfsb.name_scope('input'):
	psigb_x = tfsb.placeholder(tfsb.float32, [1, 15, 15, 5], 'input_layer')

psigb_variable_set = {}
# neural network
psigb_hidden_conv_layer1 = nn_layer(1, psigb_x, [5, 5, 5, 81], 81, 'conv_layer_1')
psigb_hidden_conv_layer2 = nn_layer(1, psigb_hidden_conv_layer1, [3, 3, 81, 24], 24, 'conv_layer_2')
psigb_hidden_conv_layer3 = nn_layer(1, psigb_hidden_conv_layer2, [3, 3, 24, 24], 24, 'conv_layer_3')
psigb_hidden_conv_layer4 = nn_layer(1, psigb_hidden_conv_layer3, [3, 3, 24, 24], 24, 'conv_layer_4')
psigb_hidden_conv_layer5 = nn_layer(1, psigb_hidden_conv_layer4, [3, 3, 24, 24], 24, 'conv_layer_5')
psigb_hidden_conv_layer6 = nn_layer(1, psigb_hidden_conv_layer5, [3, 3, 24, 24], 24, 'conv_layer_6')
psigb_y = nn_layer(1, psigb_hidden_conv_layer6, [1, 1, 24, 1], 1, 'output_layer', tfsb.nn.softmax, True)
psigb_v = tfsb.argmax(psigb_y, 1);
psigb_saver = tfsb.train.Saver(psigb_variable_set)
psigb_init = tfsb.initialize_all_variables()

psigb_X = psigb_x
psigb_Y = psigb_y
psigb_POS = psigb_v

psigb_sess = tfsb.InteractiveSession()
psigb_sess.run(psigb_init)
load_player("pSigma_black.ckpt");