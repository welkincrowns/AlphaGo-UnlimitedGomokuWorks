#include "Playerp.h"
#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Oracle.h"
#include "Move.h"
#include "AI.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <vector>
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

void MCTSGomokuPlayer::Activate(TreeNode* node, const std::vector<Move>& steps) {
	if (node->turn == BLACK) {
		auto node_hash = std::make_pair(node->hash1, node->hash2);
		auto it = psigma_black_map.find(node_hash);
		if (it == psigma_black_map.end()) {
			psigma_black->Execute(steps, "psigma_black_feature_file.ar", "psigma_black_distribution_file.ar");
			node->Activate(psigma_black->distribution);
			psigma_black_map[node_hash] = psigma_black->distribution;
		}
		else
			node->Activate(it->second);
	}
	else {
		auto node_hash = std::make_pair(node->hash1, node->hash2);
		auto it = psigma_white_map.find(node_hash);
		if (it == psigma_white_map.end()) {
			psigma_white->Execute(steps, "psigma_white_feature_file.ar", "psigma_white_distribution_file.ar");
			node->Activate(psigma_white->distribution);
			psigma_white_map[node_hash] = psigma_white->distribution;
		}
		else
			node->Activate(it->second);
	}
}

Move MCTSGomokuPlayer::PlacePawn(const std::vector<Move> steps) {
	// simulation of opponent's move
	if (steps.size()) {
		for (auto it = root->edges.begin(); it != root->edges.end(); ++it)
			if ((*it)->move.x == steps.back().x && (*it)->move.y == steps.back().y) {
				TreeNode *tmp = (*it)->node;
				*it = NULL;
				delete root;
				root = tmp;
				break;
			}
	}
	if (!root->is_in_tree)
		Activate(root, steps);
	clock_t start_clock = clock();
	int cnt = 0;
	while (double(clock() - start_clock) / CLOCKS_PER_SEC < TIME_LIMIT) {
		//printf("NOW %d\n", ++cnt);
		// selection
		std::vector<Move> tsteps(steps);
		TreeNode *cur = root;
		std::vector<TreeEdge*> edges;
		while (cur->is_in_tree) {
			//cur->board.Print();
			//getchar();
			int sum_nr = 0;
			for (auto it = cur->edges.begin(); it != cur->edges.end(); ++it)
				sum_nr += (*it)->nr;
			double sqrt_sum_nr = sqrt(sum_nr);
			TreeEdge *choice = NULL;
			double max_a = -1000000000;
			for (auto it = cur->edges.begin(); it != cur->edges.end(); ++it) {
				//printf("move = (%d, %d, %d) nv = %d nr = %d p = %.1f q = %.1f wv = %.1f wr = %.1f\n", (*it)->move.turn, (*it)->move.x, (*it)->move.y, (*it)->nv, (*it)->nr, (*it)->p, (*it)->q, (*it)->wv, (*it)->wr);
				double q = (*it)->move.turn == BLACK ? (*it)->q : -(*it)->q;
				double u = 5 * (*it)->p * sqrt_sum_nr / (1 + (*it)->nr);
				double a = q + u;
				if (a > max_a)
					max_a = a, choice = *it;
			}
			assert(choice != NULL);
			//if (choice->move.turn > 1) {
				//cur->board.Print();
			//}
			TreeNode *tmp = cur;
			cur = choice->node;
			tsteps.push_back(choice->move);
			edges.push_back(choice);
		}
		//printf("Selection Succeed\n");
		// expansion
		if (edges.back()->nr == NTHR && edges.back()->node->result == -1)
			Activate(edges.back()->node, tsteps);
		//printf("Expansion Succeed\n");
		// evaluation
		double v;
		auto cur_hash = std::make_pair(cur->hash1, cur->hash2);
		auto it = vtheta_map.find(cur_hash);
		if (it == vtheta_map.end()) {
			vtheta->Execute(tsteps, "vtheta_feature_file.ar", "vtheta_distribution_file.ar");
			v = vtheta->distribution[0];
			vtheta_map[cur_hash] = vtheta->distribution;
		}
		else
			v = it->second[0];
		tsteps.clear();
		*board = cur->board;
		int z = 0;
		if (cur->turn == BLACK) {
			if (cur->result == BLACKFIVE)
				z = 1;
			else if (cur->result == WHITEFIVE)
				z = -1;
			while (!z) {
				Move a = player1->PlacePawn(tsteps);
				int result = board->AddStone(a.x, a.y, a.turn);
				if (result == BLACKFIVE) {
					z = 1;
					break;
				}
				Move b = player2->PlacePawn(tsteps);
				result = board->AddStone(b.x, b.y, b.turn);
				if (result == WHITEFIVE) {
					z = -1;
					break;
				}
			}
		}
		else {
			if (cur->result == BLACKFIVE)
				z = 1;
			else if (cur->result == WHITEFIVE)
				z = -1;
			while (!z) {
				Move a = player2->PlacePawn(tsteps);
				int result = board->AddStone(a.x, a.y, a.turn);
				if (result == WHITEFIVE) {
					z = -1;
					break;
				}
				Move b = player1->PlacePawn(tsteps);
				result = board->AddStone(b.x, b.y, b.turn);
				if (result == BLACKFIVE) {
					z = 1;
					break;
				}
			}
		}
		//printf("Evaluation Succeed\n");
		// backup
		for (auto it = edges.begin(); it != edges.end(); ++it) {
			++(*it)->nr, (*it)->wr += z;
			++(*it)->nv, (*it)->wv += v;
			(*it)->q = (1 - LAMBDA) * (*it)->wv / (*it)->nv + LAMBDA * (*it)->wr / (*it)->nr;
		}
		//printf("Backup Succeed\n");
	}
	// move
	int max_nr = 0;
	TreeEdge *choice = NULL;
	for (auto it = root->edges.begin(); it != root->edges.end(); ++it)
		if ((*it)->nr > max_nr) {
			max_nr = (*it)->nr;
			choice = *it;
		}
	Move move = choice->move;
	TreeNode *tmp = choice->node;
	choice->node = NULL;
	delete root;
	root = tmp;
	return move;
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