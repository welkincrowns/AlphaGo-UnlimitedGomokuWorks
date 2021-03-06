import tensorflow as tf
import numpy as np
import random
import os

def random_value(shape):
	return tf.random_normal(shape = shape, mean = 0, stddev = 0.01, dtype = tf.float32)
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
			variable_set[layer_name + '/' + 'weights/weight' + ':0'] = weights
		with tf.name_scope('biases'):
			biases = biases_variable([num_filter])
			variable_set[layer_name + '/' + 'biases/biases' + ':0'] = biases
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

# use turn 1(black) 2(white)
def read_data(filename, nd_turn):
	fp = open(filename);
	go = []
	for linea in fp.readlines():
		go.append(int(linea.replace("\n","")))

	result = go[0]
	if (result < 0):
		return (0, np.zeros((1, 15, 15, 5)), np.zeros(1, 15 * 15), 0)

	turn = 1
	M = 0
	for i in range((len(go) - 1) / 2):
		if (nd_turn == turn):
			M = M + 1
		turn = 3 - turn

	board = np.zeros((15, 15))
	feature = np.zeros((15, 15, 5))
	data_state = np.zeros((M, 15, 15, 5))
	data_action = np.zeros((M, 15 * 15))

	tot = 0
	turn = 1
	for i in range((len(go) - 1) / 2):
		x = go[2 * i + 1]
		y = go[2 * i + 2]
		# print x, y
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

		label = np.zeros((15 * 15));
		label[(x - 1) * 15 + y - 1] = 1
		board[x - 1][y - 1] = turn
		turn = 3 - turn

		if (nd_turn == turn):
			continue
		# print tot
		data_state[tot] = feature
		data_action[tot] = label
		tot = tot + 1
	return result, data_state, data_action, M


def training(training_rate):
	with tf.name_scope('input'):
		x = tf.placeholder(tf.float32, [None, 15, 15, 5], 'input_layer')
		y_ = tf.placeholder(tf.float32, [None, 15 * 15], 'y_label')
	batch_size = 1
	# print type(t)
	# print batch_size
	# neural network
	hidden_conv_layer1 = nn_layer(batch_size, x, [5, 5, 5, 81], 81, 'conv_layer_1')
	hidden_conv_layer2 = nn_layer(batch_size, hidden_conv_layer1, [3, 3, 81, 24], 24, 'conv_layer_2')
	hidden_conv_layer3 = nn_layer(batch_size, hidden_conv_layer2, [3, 3, 24, 24], 24, 'conv_layer_3')
	hidden_conv_layer4 = nn_layer(batch_size, hidden_conv_layer3, [3, 3, 24, 24], 24, 'conv_layer_4')
	hidden_conv_layer5 = nn_layer(batch_size, hidden_conv_layer4, [3, 3, 24, 24], 24, 'conv_layer_5')
	hidden_conv_layer6 = nn_layer(batch_size, hidden_conv_layer5, [3, 3, 24, 24], 24, 'conv_layer_6')
	y = nn_layer(batch_size, hidden_conv_layer6, [1, 1, 24, 1], 1, 'output_layer', tf.nn.softmax, True)

	# results
	with tf.name_scope('results'):
		loss = tf.reduce_mean(-tf.reduce_mean(y_ * tf.log(tf.clip_by_value(y, 1e-10, 1.0)), reduction_indices = 1))

	saver = tf.train.Saver(variable_set)
	init = tf.initialize_all_variables()

	sess = tf.InteractiveSession()
	sess.run(init)
	saver.restore(sess, 'tmp/pRho_black_0.ckpt')
	saver.save(sess, ('tmp/pRho_black_%d.ckpt' % 1))
	os.system('cp ' + ('tmp/pRho_black_%d.ckpt' % 1) + ' ' + ('swordslot/pRho_black_%d.ckpt' % 1))
	saver.restore(sess, 'tmp/pRho_white_0.ckpt')
	saver.save(sess, ('tmp/pRho_white_%d.ckpt' % 1))
	os.system('cp ' + ('tmp/pRho_white_%d.ckpt' % 1) + ' ' + ('swordslot/pRho_white_%d.ckpt' % 1))
	#return
	num_sword = 1;
	for i in range(1000):
		
		#saver.save(sess, ('swordslot/pRho_black_%d.ckpt' % num_sword))
		swordnum = 16
		
		# Black
		swordlist = ''
		win_tot = 0
		lose_tot = 0
		for j in range(swordnum):
			idx = random.randint(0, num_sword - 1)
			swordlist = swordlist + (' pRho_white_%d.ckpt' % (idx))
		#print ('./ArenaTest 0 1 %d pRho_black_%d.ckpt' % (swordnum, num_sword)) + swordlist
		os.system(('./ArenaTest 0 1 %d pRho_black_%d.ckpt' % (swordnum, num_sword)) + swordlist)
		
		train_step = tf.train.AdamOptimizer(training_rate / swordnum).minimize(loss)
		init = tf.initialize_all_variables()
		sess.run(init)
		saver.restore(sess, 'tmp/pRho_black_%d.ckpt' % num_sword)
		for j in range(swordnum):
			result, state, action, M = read_data('Arena/round%d.log' % (j + 1), 1)
			if (result == 1):
				#print j
				win_tot = win_tot + 1
				for k in range(M):
					sess.run(train_step, feed_dict={x: state[k : k + 1], y_: action[k : k + 1]})
		saver.save(sess, ('tmp/pRho_black_%d.ckpt' % num_sword))
		os.system('cp ' + ('tmp/pRho_black_%d.ckpt' % num_sword) + ' ' + ('swordslot/pRho_black_%d.ckpt' % num_sword))

		train_step = tf.train.AdamOptimizer(training_rate / swordnum).minimize(-loss)
		init = tf.initialize_all_variables()
		sess.run(init)
		saver.restore(sess, 'tmp/pRho_black_%d.ckpt' % num_sword)
		for j in range(swordnum):
			result, state, action, M = read_data('Arena/round%d.log' % (j + 1), 1)
			if (result == 2):
				lose_tot = lose_tot + 1
				#print j
				for k in range(M):
					sess.run(train_step, feed_dict={x: state[k : k + 1], y_: action[k : k + 1]})
		saver.save(sess, ('tmp/pRho_black_%d.ckpt' % num_sword))
		os.system('cp ' + ('tmp/pRho_black_%d.ckpt' % num_sword) + ' ' + ('swordslot/pRho_black_%d.ckpt' % num_sword))
		print ('black: totally %d blades, win %d blades, loss %d blades' % (swordnum, win_tot, lose_tot))
		
		
		# White
		swordlist = ''
		win_tot = 0
		lose_tot = 0
		for j in range(swordnum):
			idx = random.randint(0, num_sword - 1)
			swordlist = swordlist + (' pRho_black_%d.ckpt' % (idx))
		#print ('./ArenaTest 0 %d 1%s pRho_white_%d.ckpt' % (swordnum, swordlist, num_sword))
		os.system(('./ArenaTest 0 %d 1%s pRho_white_%d.ckpt' % (swordnum, swordlist, num_sword)))
		
		train_step = tf.train.AdamOptimizer(training_rate / swordnum).minimize(loss)
		init = tf.initialize_all_variables()
		sess.run(init)
		saver.restore(sess, 'tmp/pRho_white_%d.ckpt' % num_sword)
		for j in range(swordnum):
			result, state, action, M = read_data('Arena/round%d.log' % (j + 1), 2)
			if (result == 2):
				win_tot = win_tot + 1
				for k in range(M):
					sess.run(train_step, feed_dict={x: state[k : k + 1], y_: action[k : k + 1]})
		saver.save(sess, ('tmp/pRho_white_%d.ckpt' % num_sword))
		os.system('cp ' + ('tmp/pRho_white_%d.ckpt' % num_sword) + ' ' + ('swordslot/pRho_white_%d.ckpt' % num_sword))

		train_step = tf.train.AdamOptimizer(training_rate / swordnum).minimize(-loss)
		init = tf.initialize_all_variables()
		sess.run(init)
		saver.restore(sess, 'tmp/pRho_white_%d.ckpt' % num_sword)
		for j in range(swordnum):
			result, state, action, M = read_data('Arena/round%d.log' % (j + 1), 2)
			if (result == 1):
				lose_tot = lose_tot + 1
				for k in range(M):
					sess.run(train_step, feed_dict={x: state[k : k + 1], y_: action[k : k + 1]})
		saver.save(sess, ('tmp/pRho_white_%d.ckpt' % num_sword))
		os.system('cp ' + ('tmp/pRho_white_%d.ckpt' % num_sword) + ' ' + ('swordslot/pRho_white_%d.ckpt' % num_sword))
		print ('white: totally %d blades, win %d blades, loss %d blades' % (swordnum, win_tot, lose_tot))

		if ((i + 1) % 2 == 0):
			
			if ((i + 1) % 20 == 0):
				print 'I have created over %d blades, the gain is as follows: ' % (i * 16 + 16)
				os.system('./ArenaTest 1 100 pRho_black_%d.ckpt pRho_white_%d.ckpt' % (num_sword, 0))
				os.system('./ArenaTest 1 100 pRho_black_%d.ckpt pRho_white_%d.ckpt' % (0, num_sword))

			saver.restore(sess, ('tmp/pRho_black_%d.ckpt' % (num_sword)))
			saver.save(sess, ('tmp/pRho_black_%d.ckpt' % (num_sword + 1)))
			os.system('cp ' + ('tmp/pRho_black_%d.ckpt' % (num_sword + 1)) + ' ' + ('swordslot/pRho_black_%d.ckpt' % (num_sword + 1)))
			#saver.save(sess, ('swordslot/pRho_black_%d.ckpt' % (num_sword)))
	

			saver.restore(sess, ('tmp/pRho_white_%d.ckpt' % (num_sword)))
			saver.save(sess, ('tmp/pRho_white_%d.ckpt' % (num_sword + 1)))
			os.system('cp ' + ('tmp/pRho_white_%d.ckpt' % (num_sword + 1)) + ' ' + ('swordslot/pRho_white_%d.ckpt' % (num_sword + 1)))
			
			num_sword = num_sword + 1



variable_set = {}
# training(492, 100, 100, 1, 0.003)
training(0.0001)

