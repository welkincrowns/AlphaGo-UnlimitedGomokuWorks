#ifndef MOVE_H_
#define MOVE_H_

class Move{
public:
	int turn, x, y;
	Move() : turn(0), x(0), y(0) {}
	Move(int _turn, int _x, int _y) : turn(_turn), x(_x), y(_y) {}
	Move(const Move &b){
		if (&b == this)
			return;
		this->x = b.x;
		this->y = b.y;
		this->turn = b.turn;
	}
	Move& operator = (const Move&b){
		this->x = b.x;
		this->y = b.y;
		this->turn = b.turn;
		return *this;
	}
	friend std::ostream& operator << (std::ostream& out, const Move& a);
	~Move() {}
};

inline std::ostream& operator << (std::ostream& out, const Move& a){
	out << "(" << a.turn << "," << a.x << "," << a.y << ")";
	return out;
}

#endif //MOVE_H_