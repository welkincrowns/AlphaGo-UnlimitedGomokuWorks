#include "feature.h"

void GomokuFeatureExtractor::GetFeature(GomokuFeature &feature, const GomokuBoard& board){
	int n = feature.n;
	// empty
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			feature.tensor[0].slice[i][j] = 0;
	// full
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			feature.tensor[1].slice[i][j] = 1;
	// me
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			feature.tensor[2].slice[i][j] = (board.slice[i][j] == board.turn());
	// op
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			feature.tensor[3].slice[i][j] = (board.slice[i][j] && board.slice[i][j] != board.turn());
	// none
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			feature.tensor[4].slice[i][j] = (board.slice[i][j] == 0);
}