import pSigma_white as pSigma
import numpy as np
import os

def display(boardX):
	os.system('clear')
	print ' ',
	for i in range(15):
		print chr(ord('A') + i),
	print 
	for i in range(15):
		print chr(ord('a') + i),
		for j in range(15):
			if (abs(boardX[i][j] - 1.0) < 1e-9):
				print 'X',
			if (abs(boardX[i][j] - 2.0) < 1e-9):
				print 'O',
			if (abs(boardX[i][j]) < 1e-9):
				print '.',
		print 
		
def save(play_log):
	file_object = open('player.log', 'w+')
	for i in range(len(play_log)):
		a = play_log[i]
		all_the_text = str(a[0] + 1) + '\n' + str(a[1] + 1) + '\n'
		file_object.write(all_the_text)
	file_object.close()

def checkwin(boardX, turn):
	for i in range(11):
		for j in range(11):
			if (boardX[i, j] == turn):
				ok = 1
				for k in range(5):
					if (boardX[i + k, j + k] != turn):
						ok = 0
				if (ok):
					return 1
	for i in range(15):
		for j in range(11):
			if (boardX[i, j] == turn):
				ok = 1
				for k in range(5):
					if (boardX[i, j + k] != turn):
						ok = 0
				if (ok):
					return 1
	for i in range(11):
		for j in range(15):
			if (boardX[i, j] == turn):
				ok = 1
				for k in range(5):
					if (boardX[i + k, j] != turn):
						ok = 0
				if (ok):
					return 1
	return 0

def computer_turn(play_log, boardX, turn):
	save(play_log)
	[x, y, distribution] = pSigma.player('player.log', 'distr.log')
	print distribution
	boardX[x, y] = turn;
	play_log.append([x, y])
	return play_log, boardX

def player_turn(play_log, boardX, turn):
	player = raw_input('Enter your place(such as hH in (8, 8)): ')
	x = ord(player[0]) - ord('a')
	y = ord(player[1]) - ord('A')
	while (abs(boardX[x, y]) > 1e-9):
		player = raw_input('Enter your place(such as hH in (8, 8)): ')
		x = ord(player[0]) - ord('a')
		y = ord(player[1]) - ord('A')
	boardX[x][y] = turn
	play_log.append([x, y])
	return play_log, boardX
'''
play_log = [];
boardX = np.zeros((15, 15))
while (1):
	[play_log, boardX] = computer_turn(play_log, boardX, 1)
	print play_log
	if (checkwin(boardX, 1)):
		print 'Computer Win!'
		break
	display(boardX)
	[play_log, boardX] = player_turn(play_log, boardX, 2)
	if (checkwin(boardX, 2)):
		print 'You Win!'
		break
'''
play_log = [];
boardX = np.zeros((15, 15))
while (1):
	display(boardX)
	[play_log, boardX] = player_turn(play_log, boardX, 1)
	if (checkwin(boardX, 1)):
		print 'You Win!'
		break
	[play_log, boardX] = computer_turn(play_log, boardX, 2)
	print play_log
	if (checkwin(boardX, 2)):
		print 'Computer Win!'
		break
