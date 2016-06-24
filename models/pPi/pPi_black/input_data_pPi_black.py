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

def read_data(train_size, cv_size, test_size):
	fp = open("data_pPi_black.txt");
	go = []

	rtot = 0
	for linea in fp.readlines():
		go.append(int(linea.replace("\n","")))
		rtot = rtot + 1
		if (rtot % (1000 * (15 * 15 * 15 + 2)) == 0 and rtot > 0):
			print 'read in %d actions' % (rtot / (15 * 15 * 15 + 2))

	M = 0
	feature = np.zeros((15 * 15 * 15))
	turn = 1

	M = len(go) / (15 * 15 * 15 + 2);
	if (M * (15 * 15 * 15 + 2) != len(go)):
		print 'Some errors might happen'
		return

	print M
	if (M < train_size + cv_size + test_size):
		print "no enough data!!!"

	M = min(train_size + cv_size + test_size, M);

	data_state = np.zeros((M, 15 * 15 * 15))
	data_action = np.zeros((M, 15 * 15))

	tot = 0
	for i in range(len(go) / (15 * 15 * 15 + 2)):
		if (i % 1000 == 0 and i > 0):
			print 'read in %d actions' % i;

		for j in range(15 * 15 * 15):
			feature[j] = go[i * (15 * 15 * 15 + 2) + j]

		label = np.zeros(15 * 15)
		x = go[(15 * 15 * 15 + 2) * i + 15 * 15 * 15]
		y = go[(15 * 15 * 15 + 2) * i + 15 * 15 * 15 + 1]
		label[x * 15 + y] = 1

		data_state[tot] = feature
		data_action[tot] = label
		tot = tot + 1
		if (tot >= M):
			break

	# print data_state.shape
	# print tot
	
	class DataSets(object):
		pass
	data_sets = DataSets()

	data_sets.train = DataSet(data_state[0: train_size], data_action[0: train_size])
	data_sets.crossvalidation = DataSet(data_state[train_size: cv_size + train_size], data_action[train_size: cv_size + train_size])
	data_sets.test = DataSet(data_state[train_size + cv_size: test_size + cv_size + train_size], 
							data_action[train_size + cv_size: test_size + cv_size + train_size])

	return data_sets

#read_data(1000, 400, 400)
