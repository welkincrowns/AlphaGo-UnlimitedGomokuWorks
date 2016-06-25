#include "Arena.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
using namespace std;

// useage 
// ./ArenaTest 100 pRho_black_0.ckpt pRho_white_0.ckpt
int main(int argc, char* argv[]){
	Py_Initialize();
	if (argc != 4){
		cout << "Useage:\n    ./ArenaTest [time of battles] [black_checkpoint_file] [white_checkpoint_file]" << endl;
		return 0;
	}
	double t = 0.0;
	int times = atoi(argv[1]);
	for (int i = 0; i < times; ++i){
		GomokuBoard board;
		//HumanGomokuPlayer player1(&board, 0);
		PrhoGomokuPlayer player1(argc, argv, &board, argv[2], 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
		PrhoGomokuPlayer player2(argc, argv, &board, argv[3], 1, "pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");
		//PrhoGomokuPlayer player2(argc, argv, &board, "pRho_white_00.ckpt");
		GomokuArena arena(&player1, &player2, &board);
		t = t + arena.Play();
	}
	cout << "The Mean Results: " << t / times << endl;
	//board.Print();
	Py_Finalize();
}

