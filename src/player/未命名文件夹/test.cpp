#include <iostream>
#include <Python.h>
#include <vector>
#include <cstring>
#include "Move.h"
#include <fstream>
#include "Chessboard.h"
#include "ForbiddenPointFinder.h"

Chessboard::Chessboard() {
}

Chessboard::~Chessboard() {
}

bool Chessboard::IsInBoard(int x, int y) {
    return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}

bool Chessboard::IsEmpty(int x, int y) {
    return cBoard[x + 1][y + 1] == EMPTYSTONE;
}

bool Chessboard::IsForbidden(int x, int y) {
    return bForbidden[x][y];
}

bool Chessboard::IsFive(int x, int y, int color) {
    return CForbiddenPointFinder::IsFive(x, y, color);
}

bool Chessboard::IsFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsFour(x, y, color, i))
            return true;
    return false;
}

bool Chessboard::IsOpenFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenFour(x, y, color, i) == 1)
            return true;
    return false;
}

bool Chessboard::IsOpenThree(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            return true;
    return false;
}

bool Chessboard::IsDoubleFour(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleFour(x, y, color);
}

bool Chessboard::IsDoubleThree(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleThree(x, y, color);
}

int Chessboard::AddStone(int x, int y, int color) {
    return CForbiddenPointFinder::AddStone(x, y, color == BLACK ? BLACKSTONE : WHITESTONE);
}

void Chessboard::Print() {
    for (int j = BOARDSIZE; j >= 1; --j) {
        printf("%2d", j);
        for (int i = 1; i <= BOARDSIZE; ++i)
            printf(" %c", cBoard[i][j]);
        printf("\n");
    }
    printf("  ");
    for (int i = 0; i < BOARDSIZE; ++i)
        printf(" %c", i + 'a');
    printf("\n");
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForbiddenPointFinder::CForbiddenPointFinder()
{
	Clear();
}

CForbiddenPointFinder::~CForbiddenPointFinder()
{
}

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

void CForbiddenPointFinder::Clear()
{

	for (int i=0; i<BOARDSIZE+2; i++)
	{
		cBoard[0][i] = '$';
		cBoard[(BOARDSIZE+1)][i] = '$';
		cBoard[i][0] = '$';
		cBoard[i][(BOARDSIZE+1)] = '$';
	}

	for (int i=1; i<=BOARDSIZE; i++)
		for (int j=1; j<=BOARDSIZE; j++)
		{
			cBoard[i][j] = EMPTYSTONE;
			bForbidden[i-1][j-1] = false;
		}
}

int CForbiddenPointFinder::AddStone(int x, int y, char cStone)
{
	int nResult = -1;

	if (cStone == BLACKSTONE)
	{
		if (IsFive(x, y, 0))
			nResult = BLACKFIVE;
		if (bForbidden[x][y])
			nResult = BLACKFORBIDDEN;
	}
	else if (cStone == WHITESTONE)
	{
		if (IsFive(x, y, 1))
			nResult = WHITEFIVE;
	}
	
	cBoard[x+1][y+1] = cStone;
	if (nResult == -1)
		FindForbiddenPoints();
	return nResult;
}

void CForbiddenPointFinder::SetStone(int x, int y, char cStone)
{
	cBoard[x+1][y+1] = cStone;
}

bool CForbiddenPointFinder::IsFive(int x, int y, int nColor)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;

	if (nColor == 0)	// black
	{
		SetStone(x, y, BLACKSTONE);
		
		// detect black five
		int i, j;
		
		// 1 - horizontal direction
		int nLine = 1;
		i = x;
		while (i > 0)
		{
			if (cBoard[i--][y+1] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[i++][y+1] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 2 - vertical direction
		nLine = 1;
		i = y;
		while (i > 0)
		{
			if (cBoard[x+1][i--] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		i = y+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[x+1][i++] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 3 - diagonal direction (lower-left to upper-right: '/')
		nLine = 1;
		i = x;
		j = y;
		while ((i > 0) && (j > 0))
		{
			if (cBoard[i--][j--] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y+2;
		while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i++][j++] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 4 - diagonal direction (upper-left to lower-right: '\')
		nLine = 1;
		i = x;
		j = y+2;
		while ((i > 0) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i--][j++] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y;
		while ((i < (BOARDSIZE+1)) && (j > 0))
		{
			if (cBoard[i++][j--] == BLACKSTONE)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else if (nColor == 1)	// white
	{
		SetStone(x, y, WHITESTONE);
		
		// detect white five or more
		int i, j;
		
		// 1 - horizontal direction
		int nLine = 1;
		i = x;
		while (i > 0)
		{
			if (cBoard[i--][y+1] == WHITESTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[i++][y+1] == WHITESTONE)
				nLine++;
			else
				break;
		}
		if (nLine >= 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 2 - vertical direction
		nLine = 1;
		i = y;
		while (i > 0)
		{
			if (cBoard[x+1][i--] == WHITESTONE)
				nLine++;
			else
				break;
		}
		i = y+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[x+1][i++] == WHITESTONE)
				nLine++;
			else
				break;
		}
		if (nLine >= 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 3 - diagonal direction (lower-left to upper-right: '/')
		nLine = 1;
		i = x;
		j = y;
		while ((i > 0) && (j > 0))
		{
			if (cBoard[i--][j--] == WHITESTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y+2;
		while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i++][j++] == WHITESTONE)
				nLine++;
			else
				break;
		}
		if (nLine >= 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		// 4 - diagonal direction (upper-left to lower-right: '\')
		nLine = 1;
		i = x;
		j = y+2;
		while ((i > 0) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i--][j++] == WHITESTONE)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y;
		while ((i < (BOARDSIZE+1)) && (j > 0))
		{
			if (cBoard[i++][j--] == WHITESTONE)
				nLine++;
			else
				break;
		}
		if (nLine >= 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}

		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else 
		return false;
}

bool CForbiddenPointFinder::IsOverline(int x, int y)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;

	SetStone(x, y, BLACKSTONE);
		
	// detect black overline
	int i, j;
	bool bOverline = false;
		
	// 1 - horizontal direction
	int nLine = 1;
	i = x;
	while (i > 0)
	{
		if (cBoard[i--][y+1] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	i = x+2;
	while (i < (BOARDSIZE+1))
	{
		if (cBoard[i++][y+1] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	if (nLine == 5)
	{
		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else
		bOverline |= (nLine >= 6);
		
	// 2 - vertical direction
	nLine = 1;
	i = y;
	while (i > 0)
	{
		if (cBoard[x+1][i--] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	i = y+2;
	while (i < (BOARDSIZE+1))
	{
		if (cBoard[x+1][i++] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	if (nLine == 5)
	{
		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else
		bOverline |= (nLine >= 6);

	// 3 - diagonal direction (lower-left to upper-right: '/')
	nLine = 1;
	i = x;
	j = y;
	while ((i > 0) && (j > 0))
	{
		if (cBoard[i--][j--] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	i = x+2;
	j = y+2;
	while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
	{
		if (cBoard[i++][j++] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	if (nLine == 5)
	{
		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else
		bOverline |= (nLine >= 6);

	// 4 - diagonal direction (upper-left to lower-right: '\')
	nLine = 1;
	i = x;
	j = y+2;
	while ((i > 0) && (j < (BOARDSIZE+1)))
	{
		if (cBoard[i--][j++] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	i = x+2;
	j = y;
	while ((i < (BOARDSIZE+1)) && (j > 0))
	{
		if (cBoard[i++][j--] == BLACKSTONE)
			nLine++;
		else
			break;
	}
	if (nLine == 5)
	{
		SetStone(x, y, EMPTYSTONE);
		return false;
	}
	else
		bOverline |= (nLine >= 6);

	SetStone(x, y, EMPTYSTONE);
	return bOverline;
}

bool CForbiddenPointFinder::IsFive(int x, int y, int nColor, int nDir)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;
	
	char c;
	if (nColor == 0)	// black
		c = BLACKSTONE;
	else if (nColor == 1)	// white
		c = WHITESTONE;
	else
		return false;
		
	SetStone(x, y, c);

	int i, j;
	int nLine;
			
	switch (nDir)
	{
	case 1:		// horizontal direction
		nLine = 1;
		i = x;
		while (i > 0)
		{
			if (cBoard[i--][y+1] == c)
				nLine++;
			else
				break;
		}
		i = x+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[i++][y+1] == c)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}
		else
		{
			SetStone(x, y, EMPTYSTONE);
			return false;
		}
		break;
	case 2:		// vertial direction
		nLine = 1;
		i = y;
		while (i > 0)
		{
			if (cBoard[x+1][i--] == c)
				nLine++;
			else
				break;
		}
		i = y+2;
		while (i < (BOARDSIZE+1))
		{
			if (cBoard[x+1][i++] == c)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}
		else
		{
			SetStone(x, y, EMPTYSTONE);
			return false;
		}
		break;
	case 3:		// diagonal direction - '/'
		nLine = 1;
		i = x;
		j = y;
		while ((i > 0) && (j > 0))
		{
			if (cBoard[i--][j--] == c)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y+2;
		while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i++][j++] == c)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}
		else
		{
			SetStone(x, y, EMPTYSTONE);
			return false;
		}
		break;
	case 4:		// diagonal direction - '\'
		nLine = 1;
		i = x;
		j = y+2;
		while ((i > 0) && (j < (BOARDSIZE+1)))
		{
			if (cBoard[i--][j++] == c)
				nLine++;
			else
				break;
		}
		i = x+2;
		j = y;
		while ((i < (BOARDSIZE+1)) && (j > 0))
		{
			if (cBoard[i++][j--] == c)
				nLine++;
			else
				break;
		}
		if (nLine == 5)
		{
			SetStone(x, y, EMPTYSTONE);
			return true;
		}
		else
		{
			SetStone(x, y, EMPTYSTONE);
			return false;
		}
		break;
	default:
		SetStone(x, y, EMPTYSTONE);
		return false;
		break;
	}
}

bool CForbiddenPointFinder::IsFour(int x, int y, int nColor, int nDir)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;

	if (IsFive(x, y, nColor))	// five?
		return false;
	else if ((nColor == 0) && (IsOverline(x, y)))	// black overline?
		return false;
	else
	{
		char c;
		if (nColor == 0)	// black
			c = BLACKSTONE;
		else if (nColor == 1)	// white
			c = WHITESTONE;
		else
			return false;
		
		SetStone(x, y, c);

		int i, j;
			
		switch (nDir)
		{
		case 1:		// horizontal direction
			i = x;
			while (i > 0)
			{
				if (cBoard[i][y+1] == c)
				{
					i--;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if (IsFive(i-1, y, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[i][y+1] == c)
				{
					i++;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if (IsFive(i-1, y, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 2:		// vertial direction
			i = y;
			while (i > 0)
			{
				if (cBoard[x+1][i] == c)
				{
					i--;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if (IsFive(x, i-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = y+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[x+1][i] == c)
				{
					i++;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if (IsFive(x, i-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 3:		// diagonal direction - '/'
			i = x;
			j = y;
			while ((i > 0) && (j > 0))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j--;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			j = y+2;
			while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 4:		// diagonal direction - '\'
			i = x;
			j = y+2;
			while ((i > 0) && (j < (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			j = y;
			while ((i < (BOARDSIZE+1)) && (j > 0))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j--;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		default:
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		}
	}
}

int CForbiddenPointFinder::IsOpenFour(int x, int y, int nColor, int nDir)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return 0;
	
	if (IsFive(x, y, nColor))	// five?
		return 0;
	else if ((nColor == 0) && (IsOverline(x, y)))	// black overline?
		return 0;
	else
	{
		char c;
		if (nColor == 0)	// black
			c = BLACKSTONE;
		else if (nColor == 1)	// white
			c = WHITESTONE;
		else
			return 0;
		
		SetStone(x, y, c);

		int i, j;
		int nLine;
			
		switch (nDir)
		{
		case 1:		// horizontal direction
			nLine = 1;
			i = x;
			while (i >= 0)
			{
				if (cBoard[i][y+1] == c)
				{
					i--;
					nLine++;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if (!IsFive(i-1, y, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return 0;
					}
					else 
						break;
				}
				else
				{
					SetStone(x, y, EMPTYSTONE);
					return 0;
				}
			}
			i = x+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[i][y+1] == c)
				{
					i++;
					nLine++;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if (IsFive(i-1, y, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return (nLine==4 ? 1 : 2);
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return 0;
			break;
		case 2:		// vertial direction
			nLine = 1;
			i = y;
			while (i >= 0)
			{
				if (cBoard[x+1][i] == c)
				{
					i--;
					nLine++;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if (!IsFive(x, i-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return 0;
					}
					else 
						break;
				}
				else
				{
					SetStone(x, y, EMPTYSTONE);
					return 0;
				}
			}
			i = y+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[x+1][i] == c)
				{
					i++;
					nLine++;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if (IsFive(x, i-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return (nLine==4 ? 1 : 2);
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return 0;
			break;
		case 3:		// diagonal direction - '/'
			nLine = 1;
			i = x;
			j = y;
			while ((i >= 0) && (j >= 0))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j--;
					nLine++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (!IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return 0;
					}
					else 
						break;
				}
				else
				{
					SetStone(x, y, EMPTYSTONE);
					return 0;
				}
			}
			i = x+2;
			j = y+2;
			while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j++;
					nLine++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return (nLine==4 ? 1 : 2);
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return 0;
			break;
		case 4:		// diagonal direction - '\'
			nLine = 1;
			i = x;
			j = y+2;
			while ((i >= 0) && (j <= (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j++;
					nLine++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (!IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return 0;
					}
					else 
						break;
				}
				else
				{
					SetStone(x, y, EMPTYSTONE);
					return 0;
				}
			}
			i = x+2;
			j = y;
			while ((i < (BOARDSIZE+1)) && (j > 0))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j--;
					nLine++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if (IsFive(i-1, j-1, nColor, nDir))
					{
						SetStone(x, y, EMPTYSTONE);
						return (nLine==4 ? 1 : 2);
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return 0;
			break;
		default:
			SetStone(x, y, EMPTYSTONE);
			return 0;
			break;
		}
	}
}

bool CForbiddenPointFinder::IsDoubleFour(int x, int y, int nColor)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;
	
	if (IsFive(x, y, nColor))	// five?
		return 0;
	else if ((nColor == 0) && (IsOverline(x, y)))	// black overline?
		return 0;
	
	int nFour = 0;
	for (int i=1; i<=4; i++)
	{
		if (IsOpenFour(x, y, nColor, i) == 2)
			nFour += 2;
		else if (IsFour(x, y, nColor, i))
			nFour++;
	}

	if (nFour >= 2)
		return true;
	else
		return false;
}

bool CForbiddenPointFinder::IsOpenThree(int x, int y, int nColor, int nDir)
{
	if (IsFive(x, y, nColor))	// five?
		return false;
	else if ((nColor == 0) && (IsOverline(x, y)))	// black overline?
		return false;
	else
	{
		char c;
		if (nColor == 0)	// black
			c = BLACKSTONE;
		else if (nColor == 1)	// white
			c = WHITESTONE;
		else
			return false;
		
		SetStone(x, y, c);

		int i, j;
			
		switch (nDir)
		{
		case 1:		// horizontal direction
			i = x;
			while (i > 0)
			{
				if (cBoard[i][y+1] == c)
				{
					i--;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, y, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, y, 0)) && (!IsDoubleThree(i-1, y, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[i][y+1] == c)
				{
					i++;
					continue;
				}
				else if (cBoard[i][y+1] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, y, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, y, 0)) && (!IsDoubleThree(i-1, y, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 2:		// vertial direction
			i = y;
			while (i > 0)
			{
				if (cBoard[x+1][i] == c)
				{
					i--;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if ((IsOpenFour(x, i-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(x, i-1, 0)) && (!IsDoubleThree(x, i-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = y+2;
			while (i < (BOARDSIZE+1))
			{
				if (cBoard[x+1][i] == c)
				{
					i++;
					continue;
				}
				else if (cBoard[x+1][i] == EMPTYSTONE)
				{
					if ((IsOpenFour(x, i-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(x, i-1, 0)) && (!IsDoubleThree(x, i-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 3:		// diagonal direction - '/'
			i = x;
			j = y;
			while ((i > 0) && (j > 0))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j--;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, j-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, j-1, 0)) && (!IsDoubleThree(i-1, j-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			j = y+2;
			while ((i < (BOARDSIZE+1)) && (j < (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, j-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, j-1, 0)) && (!IsDoubleThree(i-1, j-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		case 4:		// diagonal direction - '\'
			i = x;
			j = y+2;
			while ((i > 0) && (j < (BOARDSIZE+1)))
			{
				if (cBoard[i][j] == c)
				{
					i--;
					j++;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, j-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, j-1, 0)) && (!IsDoubleThree(i-1, j-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else 
						break;
				}
				else
					break;
			}
			i = x+2;
			j = y;
			while ((i < (BOARDSIZE+1)) && (j > 0))
			{
				if (cBoard[i][j] == c)
				{
					i++;
					j--;
					continue;
				}
				else if (cBoard[i][j] == EMPTYSTONE)
				{
					if ((IsOpenFour(i-1, j-1, nColor, nDir) == 1) && (nColor == 1 || (nColor == 0 && (!IsDoubleFour(i-1, j-1, 0)) && (!IsDoubleThree(i-1, j-1, 0)))))
					{
						SetStone(x, y, EMPTYSTONE);
						return true;
					}
					else
						break;
				}
				else
					break;
			}
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		default:
			SetStone(x, y, EMPTYSTONE);
			return false;
			break;
		}
	}
}

bool CForbiddenPointFinder::IsDoubleThree(int x, int y, int nColor)
{
	if (cBoard[x+1][y+1] != EMPTYSTONE)
		return false;

	if (IsFive(x, y, nColor))	// five?
		return 0;
	else if ((nColor == 0) && (IsOverline(x, y)))	// black overline?
		return 0;
	
	int nThree = 0;
	for (int i=1; i<=4; i++)
	{
		if (IsOpenThree(x, y, nColor, i))
			nThree++;
	}

	if (nThree >= 2)
		return true;
	else
		return false;
}

void CForbiddenPointFinder::FindForbiddenPoints()
{
	for (int i=0; i<BOARDSIZE; i++)
	{
		for (int j=0; j<BOARDSIZE; j++)
		{
			bForbidden[i][j] = false;
			if (cBoard[i+1][j+1] != EMPTYSTONE)
				continue;
			else
			{
				if (IsOverline(i, j) || IsDoubleFour(i, j, 0) || IsDoubleThree(i, j, 0))
				{
					bForbidden[i][j] = true;
				}
			}
		}
	}
}

class Oracle{
public:
	PyObject* pModule;
	Oracle(int argc, char* argv[], const std::string& model_name);
	virtual ~Oracle();
	virtual void WriteFeature(const std::vector<Move>& steps, const std::string& filename) = 0;
	virtual void ReadDistribution(const std::string& filename) = 0;
	void Execute(const std::vector<Move>& steps, const std::string& feature_filename, const std::string& distribution_filename);
	std::vector<double> distribution;
};

class pSigmaOracle : public Oracle{
	const int d_size;
public:
	pSigmaOracle(int argc, char* argv[], const std::string& model_name, int _size = 225) : Oracle(argc, argv, model_name), d_size(_size) {}
	void WriteFeature(const std::vector<Move>& steps, const std::string& filename);
	void ReadDistribution(const std::string& filename);
};

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

class HumanGomokuPlayer : public GomokuPlayer{
	const int TURN;
public:
	HumanGomokuPlayer(int argc, char* argv[], GomokuBoard *_board, int turn = 1)
						: GomokuPlayer(_board),
						  TURN(turn) {
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


Oracle::Oracle(int argc, char* argv[], const std::string& model_name) : pModule(NULL){
	PySys_SetArgv(argc, argv);
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	pModule = PyImport_ImportModule(model_name.c_str());
	if (!pModule){
		PyErr_Print();
		std::cout << "Oracle::Oracle() ERROR : get module failed!" << std::endl;
		assert(0);
	}
}

void Oracle::Execute(const std::vector<Move>& steps, const std::string& feature_filename, const std::string& distribution_filename){
	// write feature
	WriteFeature(steps, feature_filename);

	// call the tensorflow main body
	PyObject* pFunc = PyObject_GetAttrString(pModule, "player");
	if (!pFunc){
		PyErr_Print();
		std::cout << "Oracle::Execute() ERROR: get func failed!" << std::endl;
		assert(0);
	}
	PyObject* pParam = Py_BuildValue("(ss)", feature_filename.c_str(), distribution_filename.c_str());
	PyObject* pResult = PyEval_CallObject(pFunc, pParam);
	if (!pResult){
		PyErr_Print();
		std::cout << "Oracle::Execute() ERROR: get result failed!" << std::endl;
		assert(0);
	}

	// read the distribution
	distribution.clear();
	ReadDistribution(distribution_filename);
}

Oracle::~Oracle(){
	// empty
}

void pSigmaOracle::WriteFeature(const std::vector<Move>& steps, const std::string& filename){
	std::ofstream out(filename.c_str());
	if (!out.is_open()){
		std::cout << "pSigmaOracle::WriteFeature() ERROR: can't open file!" << std::endl;
		assert(0);
	}
	for (int i = 0; i < steps.size(); ++i)
		out << steps[i].x << std::endl << steps[i].y << std::endl;
	out.close();
}

void pSigmaOracle::ReadDistribution(const std::string& filename){
	std::ifstream in(filename.c_str());
	if (!in.is_open()){
		std::cout << "pSigmaOracle::ReadDistribution() ERROR: can't open file!" << std::endl;
		assert(0);
	}

	for (int i = 0; i < d_size; ++i){
		double t = 0;
		in >> t;
		distribution.push_back(t);
	}

	in.close();
}

Move PsigmaGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	psigma.Execute(steps, feature_file, distribution_file);

	double maxd = 0.0;
	int posi = 0;
	int posj = 0;

	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j){
			if (board->IsEmpty(i, j) && (!board->IsForbidden(i, j) || TURN == WHITE)){
				if (psigma.distribution[i * 15 + j] > maxd){
					maxd = psigma.distribution[i * 15 + j];
					posi = i;
					posj = j;
				}
			}
		}

	return Move(TURN, posi, posj);
}

void HumanGomokuPlayer::DisplayBoard(const std::string& info, const std::vector<Move>& steps){
	std::cout << "INFO: " << info << std::endl;
	std::cout << "=========================================" << std::endl;

	int board[15][15];
	memset(board, -1, sizeof board);
	for (int i = 0; i < steps.size(); ++i)
		board[steps[i].x][steps[i].y] = steps[i].turn;

	printf(" ");
	for (int i = 0; i < 15; ++i)
		printf(" %c", i + 'A');

	for (int i = 0; i < 15; ++i){
		printf("%c", i + 'a');

		for (int j = 0; j < 15; ++j){
			printf(" ");
			switch(board[i][j]){
				case -1: {printf("."); break;}
				case 0:  {printf("X"); break;}
				case 1:  {printf("O"); break;}
			}
		}

		printf("\n");
	}

	printf("Place your pawn: ");
}

Move HumanGomokuPlayer::PlacePawn(const std::vector<Move> steps){
	std::string info = "Sample: 'hH' for (8, 8)";
	int x = 0, y = 0;
	char order[10];
	while (true){
		DisplayBoard(info, steps);

		scanf("%s", order);
		if (strlen(order) != 2){
			info = "Invalid Order (Such as hH for (8, 8))";
			continue;
		}
		x = order[0] - 'a', y = order[1] - 'A';
		if (!board->IsInBoard(x, y)){
			info = "Out of Range";
			continue;
		}
		if (!board->IsEmpty(x, y)){
			info = "Stone Exists";
			continue;
		}
		if (TURN == BLACK && board->IsForbidden(x, y)) {
			info = "Forbidden Move";
			continue;
		}
		break;
	}

	return Move(TURN, x, y);
}

int main(int argc, char* argv[]){
	Py_Initialize();

	// test psigma
	GomokuBoard board;
	PsigmaGomokuPlayer player1(argc, argv, &board);

	/*vector<Move> steps;
	steps.clear();
	Move a = player1.PlacePawn(steps);

	cout << a << endl;
*/
	Py_Finalize();
}