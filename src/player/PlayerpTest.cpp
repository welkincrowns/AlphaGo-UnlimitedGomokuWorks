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

void greedy_black() {
	GomokuBoard board;
	GreedyGomokuPlayer player1(&board, 0);
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

void knowledgeable_black() {
	GomokuBoard board;
	KnowledgeableGomokuPlayer player1(&board, 0);
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

int knowledgeable_battle() {
	GomokuBoard board;
	KnowledgeableGomokuPlayer player1(&board, 0);
	KnowledgeableGomokuPlayer player2(&board, 1);

	vector<Move> steps;
	steps.clear();

	while (true){
		Move a = player1.PlacePawn(steps);
		//printf("%d %d\n", a.x, a.y);
		steps.push_back(a);
		int result = board.AddStone(a.x, a.y, a.turn);
		if (result == BLACKFIVE) {
			printf("Black wins\n");
			return 0;
		}
		Move b = player2.PlacePawn(steps);
		steps.push_back(b);
		//printf("%d %d\n", b.x, b.y);
		result = board.AddStone(b.x, b.y, b.turn);
		if (result == WHITEFIVE) {
			printf("White wins\n");
			return 1;
		}
		//getchar();
	}
}

int main(int argc, char* argv[]){
	Py_Initialize();
	// test psigma_black
	psigma_black(argc, argv);
	// test psigma_white
	// psigma_white(argc, argv);
	// test greedy_black
	// greedy_black();
	// test knowledgeable_black
	// knowledgeable_black();
	int n = 5000, ans = 0;
	while (n--)
		ans += knowledgeable_battle();
	printf("%d\n", ans);
	Py_Finalize();
}