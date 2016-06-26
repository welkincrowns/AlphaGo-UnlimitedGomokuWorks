#ifndef ARENA_H_
#define ARENA_H_

#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
#include <iostream>

class Arena{
public:
	Arena() {}
	~Arena() {}
};

class GomokuArena : public Arena{
public:
	// player[0] : black
	// player[1] : white
	const int id;
	GomokuPlayer *player[2];
	GomokuBoard *board;
	static int num_count;
	//GomokuArena() : id(0) {}
	GomokuArena(GomokuPlayer *player0, GomokuPlayer *player1, GomokuBoard *_board) : Arena(), board(_board), id(++num_count){
		// empty
		player[0] = player0;
		player[1] = player1;
		//std::cout << "I have created over " << id << "blades, unaware of a loss, nor aware of gain" << std::endl;
	}
	double Play(int show = 0);
private:
	void Record(const std::vector<Move>& steps, double result);
	GomokuArena(const GomokuArena&);
	GomokuArena& operator = (const GomokuArena&);
};

#endif // ARENA_H_