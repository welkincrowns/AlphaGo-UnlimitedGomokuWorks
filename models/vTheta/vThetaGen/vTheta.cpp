#include "Playerp.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
using namespace std;

GomokuBoard* board;
GomokuPlayer* player[2][2];

Move RandomSpot(const vector<Move> &steps, int turn, bool first_step){
	int x, y;
	while (true){
		x = rand() % 15; y = rand() % 15;
		if (!board->IsInBoard(x, y)){
			continue;
		}
		if (!board->IsEmpty(x, y)){
			continue;
		}
		if (turn == 0 && board->IsForbidden(x, y)) {
			continue;
		}
		bool ps = false;
		for (int i = max(0, x - 2); i <= min(14, x + 2); ++ i)
			for (int j = max(0, y - 2); j <= min(14, y + 2); ++ j)
				ps |= (!board->IsEmpty(i, j) );
		if (!first_step && !ps) continue;
		break;
	}
	return Move(turn, x, y);
}

int launch(int d){

	board->Clear();
	vector<Move> steps;
	steps.clear();

	int state = 0;
	int result = -1;
	int i = 0, p = rand() % 60;
	//cout << p << endl;
	while (true){
		//cout << i << endl;
		Move a;
		if (i == p ) {a = RandomSpot(steps, 0, i == 0); ++ state;}
		else a = player[state][0]->PlacePawn(steps);
		steps.push_back(a);
		result = board->AddStone(a.x, a.y, a.turn);
		//board->Print();
		if (result == BLACKFIVE) {
			result = 1;
			break;
		}
		++ i;
		//cout << i << endl;
		Move b;
		if (i == p) {b = RandomSpot(steps, 1, false); ++ state;}
		else b = player[state][1]->PlacePawn(steps);
		steps.push_back(b);
		result = board->AddStone(b.x, b.y, b.turn);
		//board->Print();
		if (result == WHITEFIVE) {
			result = 2;
			break;
		}
		++ i;
	}
	
	if (steps.size() <= p) return 1;

	stringstream ss;
	ss << "data_" << (d / 100) << ".txt";
	ofstream fs(ss.str(), std::ofstream::out | std::ofstream::app);
	for (int i = 0; i <= p; ++ i)
		fs << steps[i].x + 1 << endl << steps[i].y + 1 << endl;
	fs << "-1\n-1\n" << result << endl << result << endl;
	fs.close();
	return 0;
//	delete board;
}


int main(int argc, char* argv[]){

	Py_Initialize();
	int x;
	if (argc > 2 && string(argv[1]) == "--seed"){
		stringstream ss;
		ss << argv[2];
		ss >> x;
	}
	else x = time(0);
	srand(x);
	cout << "seed " << x << endl;

	board = new GomokuBoard;
	
	//player[0][0] = new PsigmaGomokuPlayer(argc, argv, board, 0, "pSigma_black", "pSigma_black_feature.al", "pSigma_black_distribution.al");
	//player[0][1] = new PsigmaGomokuPlayer(argc, argv, board, 1, "pSigma_white", "pSigma_white_feature.al", "pSigma_white_distribution.al");
	player[0][0] = new PrhoGomokuPlayer(argc, argv, board, "pSigma_black.ckpt", 0, "pSigma_black", "pSigma_black_feature.al", "pSigma_black_distribution.al");
	player[0][1] = new PrhoGomokuPlayer(argc, argv, board, "pSigma_white.ckpt", 1, "pSigma_white", "pSigma_white_feature.al", "pSigma_white_distribution.al");

	player[1][0] = new PrhoGomokuPlayer(argc, argv, board, "pRho_black.ckpt", 0, 
				"pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
	player[1][1] = new PrhoGomokuPlayer(argc, argv, board, "pRho_white.ckpt", 1, 
				"pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");

	for (int i = 0; i < 1000; ++ i)
		i -= launch(i);
	
	Py_Finalize();
}
