#include "Playerp.h"
#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Oracle.h"
#include "Move.h"
#include <cstdio>
#include <ctime>
#include <random>
#include <chrono>

Move PsigmaGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	psigma.Execute(steps, feature_file, distribution_file);

	double maxd = 0.0;
	int posi = 0;
	int posj = 0;

	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j){
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE)){
				if (psigma.distribution[i * 15 + j] > maxd){
					maxd = psigma.distribution[i * 15 + j];
					posi = i;
					posj = j;
				}
			}
		}
	printf("%d %d %d\n", TURN, posi, posj);
	return Move(TURN, posi, posj);
}

void HumanGomokuPlayer::DisplayBoard(const std::string& info, const std::vector<Move>& steps){
	system("clear");
	std::cout << "INFO: " << info << std::endl;
	std::cout << "=========================================" << std::endl;

	int board[15][15];
	memset(board, -1, sizeof board);
	for (int i = 0; i < steps.size(); ++i)
		board[steps[i].x][steps[i].y] = steps[i].turn;

	printf(" ");
	for (int i = 0; i < 15; ++i)
		printf(" %c", i + 'A');
	printf("\n");

	for (int i = 0; i < 15; ++i){
		printf("%c", i + 'a');

		for (int j = 0; j < 15; ++j){
			printf(" ");
			switch(board[i][j]){
				case -1: {printf("."); break;}
				case 0:  {printf("X"); break;}
				case 1:  {printf("O"); break;}
			}
		}

		printf("\n");
	}

	printf("Place your pawn: ");
}

Move HumanGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	std::string info = "Sample: 'hH' for (8, 8)";
	int x = 0, y = 0;
	char order[10];
	while (true){
		DisplayBoard(info, steps);

		scanf("%s", order);
		if (strlen(order) != 2){
			info = "Invalid Order (Such as hH for (8, 8))";
			continue;
		}
		x = order[0] - 'a', y = order[1] - 'A';
		if (!board->IsInBoard(x, y)){
			info = "Out of Range";
			continue;
		}
		if (!board->IsEmpty(x, y)){
			info = "Stone Exists";
			continue;
		}
		if (TURN == BLACK && board->IsForbidden(x, y)) {
			info = "Forbidden Move";
			continue;
		}
		break;
	}

	return Move(TURN, x, y);
}

Move PrhoGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	prho.Execute(steps, feature_file, distribution_file);

	int posi = 0;
	int posj = 0;
	/*for (int i = 0; i < 15; ++i){
		for (int j = 0; j < 15; ++j)
			printf("%.3lf ", prho.distribution[i * 15 + j]);
		puts("");
	}*/
	double s = 0.0;
	// normalize the distribution
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j){
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE))
				s += prho.distribution[i * 15 + j];
			else prho.distribution[i * 15 + j] = 0.0;
		}
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j){
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE))
				prho.distribution[i * 15 + j] /= s;
		}

    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);
    double t = unif(rng);
	double nws = 0.0;
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j){
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE)){
				double nwsr = nws + prho.distribution[i * 15 + j];
				if (nws <= t && t < nwsr)
					posi = i, posj = j;
				nws = nwsr;
			}
		}
	//printf("%d %d %d\n", TURN, posi, posj);
	return Move(TURN, posi, posj);
}
