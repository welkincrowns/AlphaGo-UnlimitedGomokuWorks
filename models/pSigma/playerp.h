#ifndef PLATERP_H_
#define PLATERP_H_

#include ""
class Player{
	Player() {}
	virtual ~Player() {}
	virtual std::pair<int, int> PlacePawn(const std::vector<std::pair<int, int> > steps) = 0;
};

class GomokuPlayer : public Player{
	GomokuPlayer();
	~GomokuPlayer();
}

