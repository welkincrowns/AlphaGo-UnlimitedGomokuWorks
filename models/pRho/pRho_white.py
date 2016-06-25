import tensorflow as tfw
import numpy as np

def random_value(shape):
	return tfw.random_normal(shape = shape, mean = 0, stddev = 0.01, dtype = tfw.float32)
def weight_variable(shape):
	return tfw.Variable(random_value(shape), name = 'weight')
def biases_variable(shape):
	return tfw.Variable(tfw.constant(0.1, shape = shape), name = 'biases')
def conv2d(x, W):
	return tfw.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def nn_layer(batch_size, input_tensor, weight_shape, num_filter, layer_name, act = tfw.nn.relu, nd_reshape = False):
	with tfw.name_scope(layer_name):
		with tfw.name_scope('weights'):
			weights = weight_variable(weight_shape);
			prhow_variable_set[layer_name + '/' + 'weights/weight' + ':0'] = weights
		with tfw.name_scope('biases'):
			biases = biases_variable([num_filter])
			prhow_variable_set[layer_name + '/' + 'biases/biases' + ':0'] = biases
		with tfw.name_scope('conv_combination'):
			z = conv2d(input_tensor, weights) + biases
		with tfw.name_scope('activation'):
			if nd_reshape:
				reshape = tfw.reshape(z, [batch_size, 15 * 15])
				# a = tfw.nn.dropout(act(reshape), 0.8)
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
	prhow_saver.restore(prhow_sess, 'swordslot/' + filename)
	return 'w'

def player(filename_input, filename_output):
	data = read_data(filename_input)

	# play
	y_target, pos = prhow_sess.run([prhow_Y, prhow_POS], feed_dict={prhow_X: data})

	# write the distribution
	dis_file_object = open(filename_output, 'w+')
	for i in range(15 * 15):
		dis_file_object.write(str(y_target[0, i]) + '\n')
	dis_file_object.close()

	# return the answer (for demo)
	xx = pos[0] / 15
	yy = pos[0] % 15
	return xx, yy, y_target

with tfw.name_scope('input'):
	prhow_x = tfw.placeholder(tfw.float32, [1, 15, 15, 5], 'input_layer')

prhow_variable_set = {}
# neural network
prhow_hidden_conv_layer1 = nn_layer(1, prhow_x, [5, 5, 5, 81], 81, 'conv_layer_1')
prhow_hidden_conv_layer2 = nn_layer(1, prhow_hidden_conv_layer1, [3, 3, 81, 24], 24, 'conv_layer_2')
prhow_hidden_conv_layer3 = nn_layer(1, prhow_hidden_conv_layer2, [3, 3, 24, 24], 24, 'conv_layer_3')
prhow_hidden_conv_layer4 = nn_layer(1, prhow_hidden_conv_layer3, [3, 3, 24, 24], 24, 'conv_layer_4')
prhow_hidden_conv_layer5 = nn_layer(1, prhow_hidden_conv_layer4, [3, 3, 24, 24], 24, 'conv_layer_5')
prhow_hidden_conv_layer6 = nn_layer(1, prhow_hidden_conv_layer5, [3, 3, 24, 24], 24, 'conv_layer_6')
prhow_y = nn_layer(1, prhow_hidden_conv_layer6, [1, 1, 24, 1], 1, 'output_layer', tfw.nn.softmax, True)
prhow_v = tfw.argmax(prhow_y, 1);
prhow_saver = tfw.train.Saver(prhow_variable_set)
prhow_init = tfw.initialize_all_variables()

prhow_X = prhow_x
prhow_Y = prhow_y
prhow_POS = prhow_v

prhow_sess = tfw.InteractiveSession()
prhow_sess.run(prhow_init)
#load_player("pRho_white_00.ckpt");