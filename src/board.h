#ifndef BOARD_H_
#define BOARD_H_

#include <vector>

class Board{
public:
	int n;
	std::vector<std::vector<int> > slice;
	Board(int _n = 0);
	~Board() {}
	void resize(int _n);
};

class GomokuBoard : public Board{
	int turn_;
public:
	GomokuBoard(int _n) : Board(_n) { turn_ = 1; }
	~GomokuBoard() {}
	int turn() const {return turn_;}
	void place(int x, int y){
		slice[x][y] = turn_;
		turn_ = 3 - turn_;
	}
};

#endif //BOARD_H_