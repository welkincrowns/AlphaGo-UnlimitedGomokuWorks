# AlphaGo-UnlimitedGomokuWorks

## Introduction

This is a team project of our Object-oriented Programming class, Tsinghua University.

We use the method of AlphaGo to conduct a AI that can play Gomoku. We keep the 2 policy neural network (SL policy network and RL policy network), use greedy oracle to replace the fast rollout policy network and delete the value network since it don't work very well.

You can play with our AI by call the following commands in MAC OS after converting your path to `release`

	./board -gomoku -black ./PlayGTP black
	
We use tensorflow to train and use our neural network

## Researches

The details can be seen in the white paper in `docs/whitepaper.pdf`

## Structure

The implementation of our tree search algorithm and all c++ source file are in `src`.

We preprocess the training data, the code can be seen in `preprocessing`

The neural network can be seen in `models`

The GUI code can been seen in `GUI`

The executable file can been seen in `release`
