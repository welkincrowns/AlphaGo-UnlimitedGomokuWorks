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
	const int id = 0;
	GomokuPlayer *player[2];
	GomokuBoard *board;
	static int num_count;
	GomokuArena(GomokuPlayer *player0, GomokuPlayer *player1, GomokuBoard *_board) : Arena(), player[0](player0), player[1](player1), board(_board), id(++num_count) {
		// empty
	}
	double Play(int show = 0);
private:
	void Record(const std::vector<Move>& steps);
	GomokuArena(const GomokuArena&);
	GomokuArena& operator = (const GomokuArena&);
};

#endif // ARENA_H_