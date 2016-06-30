#ifndef PLAYERP_H_
#define PLAYERP_H_

#include "Chessboard.h"
#include "Oracle.h"
#include "Move.h"
#include "Tree.h"
#include <iostream>
#include <map>
#include <random>
#include <chrono>

typedef pSigmaOracle vThetaOracle;

//class GomokuBoard : public Chessboard{
	// Just for a better name
//};
typedef Chessboard GomokuBoard;

class Player{
public:
	Player() {}
	virtual ~Player() {}
	virtual Move PlacePawn(const std::vector<Move> steps) = 0;
private:
	// Forbid it
	Player(const Player&);
	Player& operator = (const Player&);
};

class GomokuPlayer : public Player{
protected:
	GomokuBoard *board;
public:
	GomokuPlayer(GomokuBoard *_board) : Player(), board(_board) {}
	~GomokuPlayer() {}
private:
	// Forbid it
	GomokuPlayer(const GomokuPlayer&);
	GomokuPlayer& operator = (const GomokuPlayer&);
};

class PsigmaGomokuPlayer : public GomokuPlayer{
	pSigmaOracle psigma;
	const std::string feature_file, distribution_file;
	const int TURN;
public:
	// the defult setting is that PsigmaGomokuPlayer plays white
	// you can also create a PsigmaGomokuPlayer object which plays black
	// the recommended call is
	//      std::string mn = "pSigma_black", ff = "pSigma_black_feature.al", df = "pSigma_black_distribution.al";
	//      PsigmaGomokuPlayer(argc, argv, board, 0, mn, ff, df)
	PsigmaGomokuPlayer(int argc, char* argv[], GomokuBoard *_board, int turn = 1,
						const std::string& model_name = "pSigma_white",
						const std::string& _feature_file = "pSigma_white_feature.al",
						const std::string& _distribution_file = "pSigma_white_distribution.al")
						: GomokuPlayer(_board),
						  psigma(argc, argv, model_name),
						  TURN(turn),
						  feature_file(_feature_file),
						  distribution_file(_distribution_file) {
		std::cout << "Trace on" << std::endl;
	}
	// directly call the psigma deep neural network to get the answer
	Move PlacePawn(const std::vector<Move> steps);
	~PsigmaGomokuPlayer() {}
private:
	// Forbid it
	PsigmaGomokuPlayer(const PsigmaGomokuPlayer&);
	PsigmaGomokuPlayer& operator = (const PsigmaGomokuPlayer&);
};

class GreedyGomokuPlayer : public GomokuPlayer{
	const int TURN;
public:
	GreedyGomokuPlayer(GomokuBoard *_board, int turn = 1) : GomokuPlayer(_board), TURN(turn) {
		// empty
	}
	// greedily choose the answer
	Move PlacePawn(const std::vector<Move> steps);
	~GreedyGomokuPlayer() {}
private:
	// Forbid it
	GreedyGomokuPlayer(const GreedyGomokuPlayer&);
	GreedyGomokuPlayer& operator = (const GreedyGomokuPlayer&);
};

class KnowledgeableGomokuPlayer : public GomokuPlayer{
	static int built;
	const int TURN;
public:
	KnowledgeableGomokuPlayer(GomokuBoard *_board, int turn = 1);
	// choose the answer according to knowledge
	Move PlacePawn(const std::vector<Move> steps);
	Move MustMove();
	~KnowledgeableGomokuPlayer() {}
private:
	// Forbid it
	KnowledgeableGomokuPlayer(const KnowledgeableGomokuPlayer&);
	KnowledgeableGomokuPlayer& operator = (const KnowledgeableGomokuPlayer&);
};

class MCTSGomokuPlayer : public GomokuPlayer{
	const int TURN;
	const int TIME_LIMIT;
	const int NTHR;
	const double LAMBDA;
	const double CPUCT;
	pSigmaOracle *psigma_black, *psigma_white;
	vThetaOracle *vtheta;
	TreeNode *root;
	KnowledgeableGomokuPlayer *player1, *player2;
	std::map<std::pair<int, int>, std::vector<double> > psigma_black_map, psigma_white_map, vtheta_map;

public:
	MCTSGomokuPlayer(pSigmaOracle *_psigma_black, pSigmaOracle *_psigma_white, vThetaOracle *_vtheta, int turn = 0, int time_limit = 5, int nthr = 5, double lambda = 1, double cpuct = 5)
			: GomokuPlayer(NULL), psigma_black(_psigma_black), psigma_white(_psigma_white), vtheta(_vtheta), root(new TreeNode), TURN(turn), TIME_LIMIT(time_limit), NTHR(nthr), LAMBDA(lambda), CPUCT(cpuct) {
		board = new GomokuBoard;
		player1 = new KnowledgeableGomokuPlayer(board, 0);
		player2 = new KnowledgeableGomokuPlayer(board, 1);
		Activate(root, std::vector<Move>());
	}
	// choose the answer using MCTS algorithm
	Move PlacePawn(const std::vector<Move> steps);
	~MCTSGomokuPlayer() { delete root; }
private:
	void Activate(TreeNode *node, const std::vector<Move>& steps);
	// Forbid it
	MCTSGomokuPlayer(const MCTSGomokuPlayer&);
	MCTSGomokuPlayer& operator = (const MCTSGomokuPlayer&);
};

class PrhoGomokuPlayer : public GomokuPlayer{
	pRhoOracle prho;
	const std::string feature_file, distribution_file;
	const int TURN;
	// for generate random number
	std::mt19937_64 rng;
public:
	// the defult setting is that PrhoGomokuPlayer plays white
	// you can also create a PrhoGomokuPlayer object which plays black
	// the recommended call is
	//      std::string sn = "pRho_black_00.ckpt", mn = "pRho_black", ff = "pRho_black_feature.al", df = "pRho_black_distribution.al";
	//      PrhoGomokuPlayer(argc, argv, board, sn, 0, mn, ff, df)
	PrhoGomokuPlayer(int argc, char* argv[], GomokuBoard *_board,
						const std::string& swordname,
						int turn = 1,
						const std::string& model_name = "pRho_white",
						const std::string& _feature_file = "pRho_white_feature.al",
						const std::string& _distribution_file = "pRho_white_distribution.al")
						: GomokuPlayer(_board),
						  prho(argc, argv, model_name, swordname),
						  TURN(turn),
						  feature_file(_feature_file),
						  distribution_file(_distribution_file) {
		    //std::cout << "Trace On (player): " << model_name << std::endl;
    		// initialize the random number generator with time-dependent seed
    		//uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    		//std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    		//rng.seed(ss);
			rng.seed(6);
	}
	// directly call the psigma deep neural network to get the answer
	Move PlacePawn(const std::vector<Move> steps);
	void setBoard(GomokuBoard *_board){
		board = _board;
	}
	~PrhoGomokuPlayer() {}
private:
	// Forbid it
	PrhoGomokuPlayer(const PrhoGomokuPlayer&);
	PrhoGomokuPlayer& operator = (const PrhoGomokuPlayer&);
};

class HumanGomokuPlayer : public GomokuPlayer{
	const int TURN;
public:
	HumanGomokuPlayer(GomokuBoard *_board, int turn = 0) : GomokuPlayer(_board), TURN(turn) {
		// empty
	}
	// print the board to the screen and let the human player to place pawn
	Move PlacePawn(const std::vector<Move> steps);
	~HumanGomokuPlayer() {}
private:
	// print the board to the screen
	void DisplayBoard(const std::string& info, const std::vector<Move>& steps);
	// Forbid it
	HumanGomokuPlayer(const HumanGomokuPlayer&);
	HumanGomokuPlayer& operator = (const HumanGomokuPlayer&);
};

class QinAIGomokuPlayer : public GomokuPlayer{
	const int TURN;
public:
	QinAIGomokuPlayer(GomokuBoard *_board, int turn = 0) : GomokuPlayer(_board), TURN(turn) {
		// empty
	}
	// choose the answer using QinAI
	Move PlacePawn(const std::vector<Move> steps);
	~QinAIGomokuPlayer() {}
private:
	// print the board to the screen
	void DisplayBoard(const std::string& info, const std::vector<Move>& steps);
	// Forbid it
	QinAIGomokuPlayer(const QinAIGomokuPlayer&);
	QinAIGomokuPlayer& operator = (const QinAIGomokuPlayer&);
};

#endif // PLAYERP_H_