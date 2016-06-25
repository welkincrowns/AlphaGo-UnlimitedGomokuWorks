#include "Playerp.h"
#include <iostream>
using namespace std;

void psigma_black(int argc, char* argv[]){
	GomokuBoard board;
	PsigmaGomokuPlayer player1(argc, argv, &board, 0, "pSigma_black", "pSigma_black_feature.al", "pSigma_black_distribution.al");
	HumanGomokuPlayer player2(&board, 1);

	vector<Move> steps;
	steps.clear();

	while (true){
		Move a = player1.PlacePawn(steps);
		steps.push_back(a);
		int result = board.AddStone(a.x, a.y, a.turn);
		if (result == BLACKFIVE) {
			printf("Black wins\n");
			break;
		}

		Move b = player2.PlacePawn(steps);
		steps.push_back(b);
		result = board.AddStone(b.x, b.y, b.turn);
		if (result == WHITEFIVE) {
			printf("White wins\n");
			break;
		}
	}
}

void psigma_white(int argc, char* argv[]){
	GomokuBoard board;
	HumanGomokuPlayer player1(&board, 0);
	PsigmaGomokuPlayer player2(argc, argv, &board);

	vector<Move> steps;
	steps.clear();

	while (true){
		Move a = player1.PlacePawn(steps);
		steps.push_back(a);
		int result = board.AddStone(a.x, a.y, a.turn);
		if (result == BLACKFIVE) {
			printf("Black wins\n");
			break;
		}

		Move b = player2.PlacePawn(steps);
		steps.push_back(b);
		result = board.AddStone(b.x, b.y, b.turn);
		if (result == WHITEFIVE) {
			printf("White wins\n");
			break;
		}
	}	
}

void prho_black(int argc, char* argv[]){
	GomokuBoard board;
	PrhoGomokuPlayer player1(argc, argv, &board, "pRho_black_3.ckpt", 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
	HumanGomokuPlayer player2(&board, 1);

	vector<Move> steps;
	steps.clear();

	while (true){
		Move a = player1.PlacePawn(steps);
		steps.push_back(a);
		int result = board.AddStone(a.x, a.y, a.turn);
		if (result == BLACKFIVE) {
			printf("Black wins\n");
			break;
		}

		Move b = player2.PlacePawn(steps);
		steps.push_back(b);
		result = board.AddStone(b.x, b.y, b.turn);
		if (result == WHITEFIVE) {
			printf("White wins\n");
			break;
		}
	}
}

void prho_white(int argc, char* argv[]){
	GomokuBoard board;
	HumanGomokuPlayer player1(&board, 0);
	PrhoGomokuPlayer player2(argc, argv, &board, "pRho_white_1.ckpt");

	vector<Move> steps;
	steps.clear();

	while (true){
		Move a = player1.PlacePawn(steps);
		steps.push_back(a);
		int result = board.AddStone(a.x, a.y, a.turn);
		if (result == BLACKFIVE) {
			printf("Black wins\n");
			break;
		}

		Move b = player2.PlacePawn(steps);
		steps.push_back(b);
		result = board.AddStone(b.x, b.y, b.turn);
		if (result == WHITEFIVE) {
			printf("White wins\n");
			break;
		}
	}	
}


int main(int argc, char* argv[]){
	Py_Initialize();
	// test psigma_black
	// psigma_black(argc, argv);
	// test psigma_white
	//psigma_white(argc, argv);
	prho_black(argc, argv);
	Py_Finalize();
}
