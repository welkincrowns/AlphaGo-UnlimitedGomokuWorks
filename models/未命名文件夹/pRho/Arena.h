#ifndef ARENA_H_
#define ARENA_H_

#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"

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
	//static int num_count;
	//GomokuArena() : id(0) {}
	GomokuArena(GomokuPlayer *player0, GomokuPlayer *player1, GomokuBoard *_board) : Arena(), board(_board), id(0){
		// empty
		player[0] = player0;
		player[1] = player1;
	}
	double Play(int show = 0);
private:
	void Record(const std::vector<Move>& steps);
	GomokuArena(const GomokuArena&);
	GomokuArena& operator = (const GomokuArena&);
};

#endif // ARENA_H_