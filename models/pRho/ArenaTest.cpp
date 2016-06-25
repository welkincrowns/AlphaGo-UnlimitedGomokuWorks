#include "Arena.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
using namespace std;

// useage 
// ./ArenaTest 0 [number of black] [number of white] pRho_black_0.ckpt pRho_white_0.ckpt
// ./ArenaTest 1 [times] pRho_black_0.ckpt pRho_white_0.ckpt

char candb[1111][22], candw[1111][22];

int main(int argc, char* argv[]){
	Py_Initialize();

	if (atoi(argv[1]) == 0){
		double t = 0.0, cnt = 0;
		int nb = atoi(argv[2]),
			nw = atoi(argv[3]);
		/*
		for (int i = 0; i < nb; ++i)
			candb[i] = argv[3 + i];
		for (int i = 0; i < nw; ++i)
			candw[i] = argv[3 + nb + i];
		*/
		if (nb == 1){
			PrhoGomokuPlayer player1(argc, argv, NULL, argv[4 + 0], 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
			for (int i = 0; i < nw; ++i){
				GomokuBoard board;
				player1.setBoard(&board);
				PrhoGomokuPlayer player2(argc, argv, &board, argv[4 + nb + i], 1, "pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");
				GomokuArena arena(&player1, &player2, &board);
				double dt = arena.Play();
				if (dt > -1e-9)
					cnt += 1, t += dt;
			}
		cout << "The Mean Results: " << t / cnt << " " << cnt << endl;
		}
		else{
			PrhoGomokuPlayer player2(argc, argv, NULL, argv[4 + nb + 0], 1, "pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");
			for (int i = 0; i < nb; ++i){
				GomokuBoard board;
				PrhoGomokuPlayer player1(argc, argv, &board, argv[4 + i], 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
				player2.setBoard(&board);
				GomokuArena arena(&player1, &player2, &board);
				double dt = arena.Play();
				if (dt > -1e-9)
					cnt += 1, t += dt;
				}
			cout << "The Mean Results: " << t / cnt << " " << cnt << endl;
		}
	}
	else{
		double t = 0;
		int cnt = 0;
		PrhoGomokuPlayer player1(argc, argv, NULL, argv[3], 0, "pRho_black", "pRho_black_feature.al", "pRho_black_distribution.al");
		for (int i = 0; i < atoi(argv[2]); ++i){
			GomokuBoard board;
			player1.setBoard(&board);
			PrhoGomokuPlayer player2(argc, argv, &board, argv[4], 1, "pRho_white", "pRho_white_feature.al", "pRho_white_distribution.al");
			GomokuArena arena(&player1, &player2, &board);
			double dt = arena.Play();
			if (dt > -1e-9)
				cnt += 1, t += dt;
		}
		cout << "The Mean Results: " << t / cnt << " " << cnt << endl;	
		//board.Print();
	}

	Py_Finalize();
}

