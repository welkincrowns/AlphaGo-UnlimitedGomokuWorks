#include "Arena.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
using namespace std;

int main(int argc, char* argv[]){
	GomokuBoard board;
	HumanGomokuPlayer player1(&board, 0);
	PrhoGomokuPlayer player2(argc, argv, &board, "pRho_white_00.ckpt");
	GomokuArena arena(&player1, &player2, &board);
	cout << arena.Play() << endl;
}