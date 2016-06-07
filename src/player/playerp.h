#ifndef PLATERP_H_
#define PLATERP_H_

#include "Chessboard.h"

class move{
public:
	int x, y, turn;
	move(int _x, int _y, int _turn) : x(_x), y(_y), turn(_turn) {}
	move(const move &b){
		if (&b == this)
			return;
		this->x = b.x;
		this->y = b.y;
		this->turn = b.turn;
	}
	move& operator = (const move&b){
		this->x = b.x;
		this->y = b.y;
		this->turn = b.turn;
		return *this;
	}
	~move();
};

class GomokuBoard : Chessboard{
	// Just for a better name
};

class Player{
public:
	Player() {}
	virtual ~Player() {}
	virtual move PlacePawn(const std::vector<move> steps) = 0;
private:
	// Forbid it
	Player(const Player&);
	Player& operator = (const Play&);
};

class GomokuPlayer : public Player{
	GomokuBoard *board;
public:
	GomokuPlayer(GomokuBoard *_board) : Player(), board(_board) {}
	~GomokuPlayer();
private:
	// Forbid it
	GomokuPlayer(const GomokuPlayer&);
	GomokuPlayer& operator = (const GomokuPlayer&);
};

class PsigmaGomokuPlayer : public GomokuPlayer{
	pSigmaOracle psigma_black, psigma_white;
public:
	PsigmaGomokuPlayer(int argc, char* argv[], GomokuBoard *_board,
						const std::string& black_model_name = "pSigma_black",
						const std::string& white_model_name = "pSigma_white")
						: GomokuPlayer(_board),
						  psigma_black(argc, argv, black_model_name),
						  pSigma_white(argc, argv, white_model_name) {
		std::cout << "Trace on" << std::endl;
	}
	move PlacePawn(const std::vector<move> steps);
	~PsigmaGomokuPlayer() {}
private:
	// Forbid it
	PsigmaGomokuPlayer(const PsigmaGomokuPlayer&);
	PsigmaGomokuPlayer& operator = (const PsigmaGomokuPlayer&);	
};