#include "Arena.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
using namespace std;

int main(int argc, char* argv[]){
	Py_Initialize();

	double t = 0.0;
	//for (int i = 0; i < 1000; ++i){
		GomokuBoard board;
		//HumanGomokuPlayer player1(&board, 0);
		PrhoGomokuPlayer player1(argc, argv, &board, "pRho_black_00.ckpt", 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
		PrhoGomokuPlayer player2(argc, argv, &board, "pRho_white_00.ckpt", 1, "pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");
		//PrhoGomokuPlayer player2(argc, argv, &board, "pRho_white_00.ckpt");
		GomokuArena arena(&player1, &player2, &board);
		t = t + arena.Play();
	//}
	//cout << t / 1000 << endl;
	//board.Print();
	Py_Finalize();
}

