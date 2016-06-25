#ifndef PLAYERP_H_
#define PLAYERP_H_

#include "Chessboard.h"
#include "Oracle.h"
#include "Move.h"
#include <iostream>

class GomokuBoard : public Chessboard{
	// Just for a better name
};

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
	~KnowledgeableGomokuPlayer() {}
private:
	// Forbid it
	KnowledgeableGomokuPlayer(const KnowledgeableGomokuPlayer&);
	KnowledgeableGomokuPlayer& operator = (const KnowledgeableGomokuPlayer&);
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

#endif // PLATER_H_