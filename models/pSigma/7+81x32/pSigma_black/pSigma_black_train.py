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
			variable_set[weights.name] = weights
		with tf.name_scope('biases'):
			biases = biases_variable([num_filter])
			variable_set[biases.name] = biases
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

def training(train_size, test_size, cv_size, training_batch_size, training_rate):
	print train_size, test_size, cv_size, training_batch_size, training_rate
	import input_data
	gomoku = input_data.read_data(train_size, cv_size, test_size, 1);
	if tf.gfile.Exists('tmp/pSigma_white'):
		tf.gfile.DeleteRecursively('tmp/pSigma_white')
	tf.gfile.MakeDirs('tmp/pSigma_white')

	# placehold and input
	with tf.name_scope('input'):
		x = tf.placeholder(tf.float32, [None, 15, 15, 5], 'input_layer')
		y_ = tf.placeholder(tf.float32, [None, 15 * 15], 'y_label')

	batch_size = training_batch_size
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
		# loss = tf.reduce_mean(-tf.reduce_mean(y_ * tf.log(y), reduction_indices = 1));
		tf.scalar_summary('loss', loss)

	with tf.name_scope('evaluation'):
		with tf.name_scope('correct_prediction'):
			correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
		with tf.name_scope('accuracy'):
			accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
		tf.scalar_summary('accuracy', accuracy)

	# initialize
	order = 'o'
	'''while ((order != 'y') & (order != 'N')):
		order = raw_input('Continue the train last time? [y/N]')'''
	order = 'N'

	rec = []
	for k in range(10):
		training_rate = training_rate / 2.0;
		saver = tf.train.Saver(variable_set)
		train_step = tf.train.AdamOptimizer(training_rate).minimize(loss)
		init = tf.initialize_all_variables()
		merged = tf.merge_all_summaries()
		
		max_acc = 0

		# train
		with tf.Session() as sess:
			train_writer = tf.train.SummaryWriter('tmp/pSigma_white/train', sess.graph)
			test_writer = tf.train.SummaryWriter('tmp/pSigma_white/test', sess.graph)

			sess.run(init)
			# if (order == 'y'):
			if (k > 0):
				saver.restore(sess, 'tmp/pSigma_white.ckpt')

			for i in range(20001):
				if (i % 100 == 0):
					sacc = 0
					slos = 0

					for j in range(cv_size / training_batch_size):
						batch_xs, batch_ys = gomoku.crossvalidation.next_batch(training_batch_size)
						summary, acc, los = sess.run([merged, accuracy, loss], feed_dict={x: batch_xs, y_: batch_ys})
						sacc = sacc + acc
						slos = slos + los

					av_acc = sacc / (cv_size / training_batch_size)
					av_los = slos / (cv_size / training_batch_size)

					rec.append([av_acc, av_los])

					test_writer.add_summary(summary, i)
					#if ((i % 250 == 0) | ((i % 50 == 0) & (i < 250))):
					print 'Step %s: Accuracy(%s), Loss(%s)' % (i + k * 20000, av_acc, av_los)
					if (sacc > max_acc):
						max_acc = acc
					save_path = saver.save(sess, 'tmp/pSigma_white.ckpt')
					# print 'successfully saved in path', save_path

					file_ob = open('traing.txt', 'w+')
					for j in range(len(rec)):
						file_ob.write('Step %s: Accuracy(%s), Loss(%s)\n' % (j * 100, rec[j][0], rec[j][1]))
					file_ob.close()
				else:
					batch_xs, batch_ys = gomoku.train.next_batch(training_batch_size)
					summary, _, yt, lossv  = sess.run([merged, train_step, y, loss], feed_dict={x: batch_xs, y_: batch_ys})
					train_writer.add_summary(summary, i)
					# print 'Loss at step %s : %s' % (i, lossv)

			#print 'The final accuracy of test set'
			#print sess.run(accuracy, feed_dict={x: gomoku.test.state, y_: gomoku.test.action})

variable_set = {}
# training(492, 100, 100, 1, 0.003)
training(326701 - 120000, 60000, 60000, 100, 0.006)

