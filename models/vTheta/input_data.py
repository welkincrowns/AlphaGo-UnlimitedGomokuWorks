import numpy as np

class DataSet(object):
  def __init__(self, state, action):
    self._state = state
    self._action = action
    self._epochs_completed = 0
    self._index_in_epoch = 0
    self._num_examples = state.shape[0]

  @property
  def state(self):
    return self._state

  @property
  def action(self):
    return self._action

  def next_batch(self, batch_size):
    start = self._index_in_epoch
    self._index_in_epoch += batch_size
    if self._index_in_epoch > self._num_examples:
      # Finished epoch
      self._epochs_completed += 1
      # Shuffle the data
      perm = np.arange(self._num_examples)
      np.random.shuffle(perm)
      self._state = self._state[perm]
      self._action = self._action[perm]
      # Start next epoch
      start = 0
      self._index_in_epoch = batch_size
      assert batch_size <= self._num_examples
    end = self._index_in_epoch
    return self._state[start:end], self._action[start:end]

def read_data(train_size, cv_size, test_size, batches):
	
	go = []
	for i in range(batches):
		fp = open("data/data_%d.txt" % i)
		for linea in fp.readlines():
			go.append(int(linea.replace("\n","")))

	M = 0
	board = np.zeros((15, 15))
	feature = np.zeros((15, 15, 5))

	for i in range(len(go) / 2):
		if (go[2 * i] == -1):
			M = M + 1

	print M
	if (M < train_size + cv_size + test_size):
		print "no enough data!!!"

	M = min(train_size + cv_size + test_size, M);

	data_state = np.zeros((M, 15, 15, 5))
	data_action = np.zeros((M, 1))

	tot = 0
	turn = 1
	pe = False
	for ca in range(len(go) / 2):
		if (pe):
			pe = False
			if (tot % 1000 == 0 and ca > 0):
				print 'read in %d actions' % tot;
			continue

		x = go[2 * ca]
		y = go[2 * ca + 1]

		# print '(%d, %d)' % (x, y), 
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

		if ((x == -1) and (y == -1)):
			board = np.zeros((15, 15))
			label = np.zeros((1))
			if (go[2 * ca + 2] == turn):
				label[0] = 1
			else:
				label[0] = -1
			data_state[tot] = feature
			data_action[tot] = label

			tot = tot + 1
			if (tot >= M):
				break
			turn = 1
			pe = True
			# print
			continue
		
		board[x - 1][y - 1] = turn
		turn = 3 - turn
	
	class DataSets(object):
		pass
	data_sets = DataSets()

	data_sets.train = DataSet(data_state[0: train_size], data_action[0: train_size])
	data_sets.crossvalidation = DataSet(data_state[train_size: cv_size + train_size], data_action[train_size: cv_size + train_size])
	data_sets.test = DataSet(data_state[train_size + cv_size: test_size + cv_size + train_size], 
							data_action[train_size + cv_size: test_size + cv_size + train_size])

	return data_sets

#read_data(1000, 0, 0, 10)
