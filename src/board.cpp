#include "board.h"

Board::Board(int _n) : n(_n){
	slice.resize(n);
	for (int i = 0; i < n; ++i)
		slice[i].resize(n);
		
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			slice[i][j] = 0;	
}

void Board::resize(int _n){
	n = _n;
	slice.resize(n);
	for (int i = 0; i < n; ++i)
		slice[i].resize(n);
		
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			slice[i][j] = 0;	
}