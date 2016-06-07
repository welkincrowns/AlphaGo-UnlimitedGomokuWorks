#if !defined(FORBIDDENPOINTFINDER_H_INCLUDED_)
#define FORBIDDENPOINTFINDER_H_INCLUDED_

// ForbiddenPointFinder.h : header file
//

#define BOARDSIZE 15
#define BLACKSTONE 'X'
#define WHITESTONE 'O'
#define EMPTYSTONE '.'
#define BLACKFIVE 0
#define WHITEFIVE 1
#define BLACKFORBIDDEN 2

class CForbiddenPointFinder
{
protected:
	CForbiddenPointFinder();
	virtual ~CForbiddenPointFinder();
	void Clear();
	int  AddStone(int x, int y, char cStone);
	void SetStone(int x, int y, char cStone);
	bool IsFive(int x, int y, int nColor);
	bool IsOverline(int x, int y);
	bool IsFive(int x, int y, int nColor, int nDir);
	bool IsFour(int x, int y, int nColor, int nDir);
	int  IsOpenFour(int x, int y, int nColor, int nDir);
	bool IsOpenThree(int x, int y, int nColor, int nDir);
	bool IsDoubleFour(int x, int y, int nColor);
	bool IsDoubleThree(int x, int y, int nColor);
	void FindForbiddenPoints();
	bool bForbidden[BOARDSIZE][BOARDSIZE];
	char cBoard[BOARDSIZE+2][BOARDSIZE+2];	
};

#endif // !defined(FORBIDDENPOINTFINDER_H_INCLUDED_)