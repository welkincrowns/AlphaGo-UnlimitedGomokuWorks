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
	/*for (int i = 0; i < BOARDSIZE; ++i)
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
				return Move(TURN, i, j);*/
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
	return Move(TURN, 0, 0);
}

Move KnowledgeableGomokuPlayer::MustMove() {
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
	/*for (int i = 0; i < BOARDSIZE; ++i)
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
				return Move(TURN, i, j);*/
	return Move(-1, 0, 0);
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

//const double curve[] = {1, 0.9820137900379085, 0.9781187290638695, 0.9734030064231342, 0.9677045353015494, 0.9608342772032357, 0.9525741268224333, 0.9426758241011313, 0.9308615796566532, 0.9168273035060777, 0.9002495108803148, 0.8807970779778824, 0.8581489350995122, 0.8320183851339245, 0.8021838885585817, 0.7685247834990176, 0.7310585786300049, 0.6899744811276125, 0.6456563062257954, 0.598687660112452, 0.549833997312478, 0.5, 0.401312339887548, 0.31002551887238755, 0.23147521650098235, 0.1679816148660755, 0.11920292202211757, 0.0831726964939224, 0.057324175898868734, 0.039165722796764335, 0.026596993576865846};
const double curve[] = {1, 0.9820137900379085, 0.9781187290638695, 0.9734030064231342, 0.9677045353015494, 0.9608342772032357, 0.9525741268224333, 0.9426758241011313, 0.9308615796566532, 0.9168273035060777, 0.9002495108803148, 0.8807970779778824, 0.8581489350995122, 0.8320183851339245, 0.8021838885585817, 0.7685247834990176, 0.7310585786300049, 0.6899744811276125, 0.6456563062257954, 0.598687660112452, 0.549833997312478, 0.5, 0.5, 0.4, 0.4, 0.3, 0.3, 0.2, 0.2, 0.1, 0.1};

Move MCTSGomokuPlayer::PlacePawn(const std::vector<Move> steps) {
	// simulation of opponent's move
	if (steps.size()) {
		for (auto it = root->edges.begin(); it != root->edges.end(); ++it)
			if ((*it)->move.x == steps.back().x && (*it)->move.y == steps.back().y) {
				TreeNode *tmp = (*it)->node;
				*it = NULL;
				delete root;
				root = tmp;
				if (!root->is_in_tree)
					Activate(root, steps);
				break;
			}
	}
	if (root->edges.size() == 0)
		return Move(TURN, 0, 0);
	//for (auto it = steps.begin(); it != steps.end(); ++it)
	//	printf("%d %d %d\n", it->turn, it->x, it->y);
	clock_t start_clock = clock();
	int cnt = 0;
	//while (double(clock() - start_clock) / CLOCKS_PER_SEC < TIME_LIMIT) {
	//int lim = steps.size() < 36 ? 50 : 200;
	int lim = 400;
	while (cnt < lim) {
		//printf("NOW %d\n", ++cnt);
		// selection
		std::vector<Move> tsteps(steps);
		TreeNode *cur = root;
		std::vector<TreeEdge*> edges;
		while (cur->is_in_tree) {
			//cur->board.Print();
			//getchar();
			TreeEdge *choice = NULL;
			int sum_nr = 0;
			for (auto it = cur->edges.begin(); it != cur->edges.end(); ++it)
				sum_nr += (*it)->nr;
			if (sum_nr == 0) {
				double max_p = -1;
				for (auto it = cur->edges.begin(); it != cur->edges.end(); ++it) {
					if ((*it)->p > max_p)
						max_p = (*it)->p, choice = *it;
				}
			}
			else {
				double sqrt_sum_nr = sqrt(sum_nr);
				double max_a = -1000000000;
				for (auto it = cur->edges.begin(); it != cur->edges.end(); ++it) {
					//printf("move = (%d, %d, %d) nv = %d nr = %d p = %.1f q = %.1f wv = %.1f wr = %.1f\n", (*it)->move.turn, (*it)->move.x, (*it)->move.y, (*it)->nv, (*it)->nr, (*it)->p, (*it)->q, (*it)->wv, (*it)->wr);
					double q = (*it)->move.turn == BLACK ? (*it)->q : -(*it)->q;
					double u = CPUCT * (*it)->p * sqrt_sum_nr / (1 + (*it)->nr);
					double a = q + u;
					if (a > max_a) {
						//printf("a = %.2f q = %.2f u = %.2f\n", a, q, u);
						max_a = a, choice = *it;
					}
				}
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
//		if (edges.back()->nr == NTHR)
//			Activate(edges.back()->node, tsteps);
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
		int roll_cnt = 0;
		double z = 0;
		if (cur->turn == BLACK) {
			if (cur->result == BLACKFIVE)
				z = 1;
			else if (cur->result == WHITEFIVE)
				z = -1;
			while (!z) {
				Move a = player1->MustMove();
				if (a.turn == -1) {
					if (++roll_cnt > 30)
						break;
					a = player1->PlacePawn(tsteps);
				}
				if (!board->IsEmpty(a.x, a.y) || board->IsForbidden(a.x, a.y))
					break;
				int result = board->AddStone(a.x, a.y, a.turn);
				if (result == BLACKFIVE) {
					z = curve[roll_cnt];
					break;
				}
				Move b = player2->MustMove();
				if (b.turn == -1) {
					if (++roll_cnt > 30)
						break;
					b = player2->PlacePawn(tsteps);
				}
				if (!board->IsEmpty(b.x, b.y) || board->IsForbidden(b.x, b.y))
					break;
				result = board->AddStone(b.x, b.y, b.turn);
				if (result == WHITEFIVE) {
					z = -curve[roll_cnt];
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
				Move b = player2->MustMove();
				if (b.turn == -1) {
					if (++roll_cnt > 30)
						break;
					b = player2->PlacePawn(tsteps);
				}
				if (!board->IsEmpty(b.x, b.y) || board->IsForbidden(b.x, b.y))
					break;
				int result = board->AddStone(b.x, b.y, b.turn);
				if (result == WHITEFIVE) {
					z = -curve[roll_cnt];
					break;
				}
				Move a = player1->MustMove();
				if (a.turn == -1) {
					if (++roll_cnt > 30)
						break;
					a = player1->PlacePawn(tsteps);
				}
				if (!board->IsEmpty(a.x, a.y) || board->IsForbidden(a.x, a.y))
					break;
				result = board->AddStone(a.x, a.y, a.turn);
				if (result == BLACKFIVE) {
					z = curve[roll_cnt];
					break;
				}
			}
		}
		//printf("Evaluation Succeed\n");
		// backup
		if (roll_cnt <= 30) {
			++cnt;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				++(*it)->nr, (*it)->wr += z;
				++(*it)->nv, (*it)->wv += v;
				(*it)->q = (1 - LAMBDA) * (*it)->wv / (*it)->nv + LAMBDA * (*it)->wr / (*it)->nr;
			}
		}
		//printf("Backup Succeed\n");
	}
	for (auto it = root->edges.begin(); it != root->edges.end(); ++it)
		if ((*it)->nr > 0) {
			printf("Move(%d, %d, %d) p = %.2f q = %.2f nr = %d wr = %.2f\n", (*it)->move.turn, (*it)->move.x, (*it)->move.y, (*it)->p, (*it)->q, (*it)->nr, (*it)->wr);
			for (auto i1 = (*it)->node->edges.begin(); i1 != (*it)->node->edges.end(); ++i1)
				if ((*i1)->nr > 0)
					printf("---- Move(%d, %d, %d) p = %.2f q = %.2f nr = %d wr = %.2f\n", (*i1)->move.turn, (*i1)->move.x, (*i1)->move.y, (*i1)->p, (*i1)->q, (*i1)->nr, (*i1)->wr);
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
	std::vector<Move> tsteps(steps);
	tsteps.push_back(move);
	if (!root->is_in_tree)
		Activate(root, tsteps);
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

Move QinAIGomokuPlayer::PlacePawn(const std::vector<Move> steps) {
	char sys_str[1000], board_str[1000];
	for (int i = 0; i < BOARDSIZE; ++i)
		for (int j = 0; j < BOARDSIZE; ++j)
			board_str[i * BOARDSIZE + j] = board->IsEmpty(i, j) ? '0' : board->IsBlack(i, j) ? '1' : '2';
	board_str[BOARDSIZE * BOARDSIZE] = 0;
	sprintf(sys_str, "./QinAI %d %d %s", TURN == BLACK ? 1 : 2, BOARDSIZE, board_str);
	int sys_ret = system(sys_str);
	int ret = WEXITSTATUS(sys_ret);
	if (!(ret >= 0 && ret < BOARDSIZE * BOARDSIZE))
		ret = 0;
	return Move(TURN, ret / BOARDSIZE, ret % BOARDSIZE);
}