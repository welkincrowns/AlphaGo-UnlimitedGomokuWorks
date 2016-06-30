import tensorflow as tfrb
import numpy as np

def random_value(shape):
	return tfrb.random_normal(shape = shape, mean = 0, stddev = 0.01, dtype = tfrb.float32)
def weight_variable(shape):
	return tfrb.Variable(random_value(shape), name = 'weight')
def biases_variable(shape):
	return tfrb.Variable(tfrb.constant(0.1, shape = shape), name = 'biases')
def conv2d(x, W):
	return tfrb.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def nn_layer(batch_size, input_tensor, weight_shape, num_filter, layer_name, act = tfrb.nn.relu, nd_reshape = False):
	with tfrb.name_scope(layer_name):
		with tfrb.name_scope('weights'):
			weights = weight_variable(weight_shape);
			prhob_variable_set[layer_name + '/' + 'weights/weight' + ':0'] = weights
		with tfrb.name_scope('biases'):
			biases = biases_variable([num_filter])
			prhob_variable_set[layer_name + '/' + 'biases/biases' + ':0'] = biases
		with tfrb.name_scope('conv_combination'):
			z = conv2d(input_tensor, weights) + biases
		with tfrb.name_scope('activation'):
			if nd_reshape:
				reshape = tfrb.reshape(z, [batch_size, 15 * 15])
				# a = tfrb.nn.dropout(act(reshape), 0.8)
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
	prhob_saver.restore(prhob_sess, 'swordslot/' + filename)
	return 'w'

def player(filename_input, filename_output):
	data = read_data(filename_input)

	# play
	y_target, pos = prhob_sess.run([prhob_Y, prhob_POS], feed_dict={prhob_X: data})

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

with tfrb.name_scope('input'):
	prhob_x = tfrb.placeholder(tfrb.float32, [1, 15, 15, 5], 'input_layer')

prhob_variable_set = {}
# neural network
prhob_hidden_conv_layer1 = nn_layer(1, prhob_x, [5, 5, 5, 81], 81, 'conv_layer_1')
prhob_hidden_conv_layer2 = nn_layer(1, prhob_hidden_conv_layer1, [3, 3, 81, 24], 24, 'conv_layer_2')
prhob_hidden_conv_layer3 = nn_layer(1, prhob_hidden_conv_layer2, [3, 3, 24, 24], 24, 'conv_layer_3')
prhob_hidden_conv_layer4 = nn_layer(1, prhob_hidden_conv_layer3, [3, 3, 24, 24], 24, 'conv_layer_4')
prhob_hidden_conv_layer5 = nn_layer(1, prhob_hidden_conv_layer4, [3, 3, 24, 24], 24, 'conv_layer_5')
prhob_hidden_conv_layer6 = nn_layer(1, prhob_hidden_conv_layer5, [3, 3, 24, 24], 24, 'conv_layer_6')
prhob_y = nn_layer(1, prhob_hidden_conv_layer6, [1, 1, 24, 1], 1, 'output_layer', tfrb.nn.softmax, True)
prhob_v = tfrb.argmax(prhob_y, 1);
prhob_saver = tfrb.train.Saver(prhob_variable_set)
prhob_init = tfrb.initialize_all_variables()

prhob_X = prhob_x
prhob_Y = prhob_y
prhob_POS = prhob_v

prhob_sess = tfrb.InteractiveSession()
prhob_sess.run(prhob_init)
#load_player("pRho_black.ckpt");