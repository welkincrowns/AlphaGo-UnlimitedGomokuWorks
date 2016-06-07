#include "Playerp.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
	Py_Initialize();

	// test psigma
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
	Py_Finalize();
}