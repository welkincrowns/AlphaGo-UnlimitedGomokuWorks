#if !defined(POINT_H_INCLUDED_)
#define POINT_H_INCLUDED_

// Point.h : header file
//

class CPoint
{
public:
	int x, y;

	CPoint() {}
	CPoint(int x, int y) : x(x), y(y) {}

	bool operator ==(const CPoint& other) const {
		return x == other.x && y == other.y;
	}
};

#endif // !defined(POINT_H_INCLUDED_)