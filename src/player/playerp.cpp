#include "Playerp.h"
#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Oracle.h"
#include "Move.h"
#include "AI.h"
#include <algorithm>
#include <cstdio>
#include <vector>

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

Move GreedyGomokuPlayer::PlacePawn(const std::vector<Move> steps) {
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && board->IsFive(i, j, TURN))
				return Move(TURN, i, j);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) &&
				board->IsFive(i, j, TURN ^ 1))
				return Move(TURN, i, j);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) &&
				(board->IsOpenFour(i, j, TURN) || board->IsDoubleFour(i, j, TURN) ||
				(board->IsFour(i, j, TURN) && board->IsOpenThree(i, j, TURN))))
				return Move(TURN, i, j);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && !board->IsForbidden(i, j) &&
				(board->IsOpenFour(i, j, TURN ^ 1) || board->IsDoubleFour(i, j, TURN ^ 1) ||
				(board->IsFour(i, j, TURN ^ 1) && board->IsOpenThree(i, j, TURN ^ 1))))
				return Move(TURN, i, j);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) &&
				(board->IsDoubleThree(i, j, TURN)))
				return Move(TURN, i, j);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && !board->IsForbidden(i, j) &&
				(board->IsDoubleThree(i, j, TURN ^ 1)))
				return Move(TURN, i, j);
	int maxt = -1, posi = 7, posj = 7;
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) &&
				(board->IsOpenThree(i, j, TURN) || board->IsFour(i, j, TURN))) {
				int t = board->CountOpenTwo(i, j, TURN);
				if (t > maxt)
					maxt = t, posi = i, posj = j;
			}
	if (maxt >= 0)
		return Move(TURN, posi, posj);
	maxt = -1, posi = 7, posj = 7;
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) &&
				(board->CountSimpleThree(i, j, TURN))) {
				int t = board->CountOpenTwo(i, j, TURN);
				if (t > maxt)
					maxt = t, posi = i, posj = j;
			}
	if (maxt >= 0)
		return Move(TURN, posi, posj);
	bool ok[BOARDSIZE][BOARDSIZE];
	memset(ok, false, sizeof ok);
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (!board->IsEmpty(i, j))
				for (int k = std::max(i - 2, 0); k < std::min(i + 3, BOARDSIZE); ++k)
					for (int l = std::max(j - 2, 0); l < std::min(j + 3, BOARDSIZE); ++l)
						ok[k][l] = true;
	maxt = -1, posi = 7, posj = 7;
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE) && ok[i][j]) {
				int t = board->CountOpenTwo(i, j, TURN);
				if (t > maxt)
					maxt = t, posi = i, posj = j;
			}
	return Move(TURN, posi, posj);
}

int KnowledgeableGomokuPlayer::built = 0;

KnowledgeableGomokuPlayer::KnowledgeableGomokuPlayer(GomokuBoard *_board, int turn /* = 1 */) : GomokuPlayer(_board), TURN(turn) {
	if (!built) {
		AI::Environmental_minister.size = 15;
		AI::Conference_minister.Start_Work();
		built = 1;
	}
}

Move KnowledgeableGomokuPlayer::PlacePawn(const std::vector<Move> steps) {
	bool ok[BOARDSIZE][BOARDSIZE];
	memset(ok, false, sizeof ok);
	ok[7][7] = true;
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j))
				AI::Environmental_minister.map[TURN].a[i][j] = 0;
			else {
				AI::Environmental_minister.map[TURN].a[i][j] = ((board->IsBlack(i, j) && TURN == BLACK) || (board->IsWhite(i, j) && TURN == WHITE)) ? 1 : 2;
				for (int k = std::max(i - 2, 0); k < std::min(i + 3, BOARDSIZE); ++k)
					for (int l = std::max(j - 2, 0); l < std::min(j + 3, BOARDSIZE); ++l)
						ok[k][l] = true;
			}
	double sum = 0;
	std::vector<std::pair<int, int> > choices;
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE)) {
				int t1 = AI::Knowledge_minister.Local_Assess(AI::Environmental_minister, AI::coor(i, j), TURN);
				AI::Environmental_minister.map[TURN].a[i][j] = 1;
				int t2 = AI::Knowledge_minister.Local_Assess(AI::Environmental_minister, AI::coor(i, j), TURN) + ok[i][j];
				int t = std::max(t2 - t1, 0);
				sum += t;
				choices.push_back(std::make_pair(i * 15 + j, t));
				AI::Environmental_minister.map[TURN].a[i][j] = 0;
			}
	double cur = 0, ran = double(rand()) / RAND_MAX;
//	double maxt = -1;
//	int ans;
	for (auto it = choices.begin(); it != choices.end(); ++it)
		if (ran <= (cur += it->second) / sum)
			return Move(TURN, it->first / 15, it->first % 15);
//		if (it->second > maxt) {
//			maxt = it->second;
//			ans = it->first;
//		}
//	return Move(TURN, ans / 15, ans % 15);
}

void HumanGomokuPlayer::DisplayBoard(const std::string& info, const std::vector<Move>& steps){
	system("clear");
	std::cout << "INFO: " << info << std::endl;
	std::cout << "=========================================" << std::endl;
	board->Print();
	printf("Place your pawn: ");
}

Move HumanGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	std::string info = "Sample: h8";
	int x = 0, y = 0;
	char order[10];
	while (true){
		DisplayBoard(info, steps);

		scanf("%s", order);
		bool valid = islower(order[0]);
		for (int i = 1; valid && order[i]; ++i)
			valid = valid && isdigit(order[i]);
		if (!valid) {
			info = "Invalid Order (Sample: h8)";
			continue;
		}
		x = order[0] - 'a';
		sscanf(order + 1, "%d", &y);
		--y;
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