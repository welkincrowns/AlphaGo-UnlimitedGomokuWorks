#ifndef PLAYERP_H_
#define PLAYERP_H_

#include "Chessboard.h"
#include "Oracle.h"
#include "Move.h"
#include <iostream>
#include <random>
#include <chrono>

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
		    
    		// initialize the random number generator with time-dependent seed
    		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    		std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    		rng.seed(ss);
	}
	// directly call the psigma deep neural network to get the answer
	Move PlacePawn(const std::vector<Move> steps);
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

#endif // PLATER_H_