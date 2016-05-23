#ifndef FEATURE_H_
#define FEATURE_H_
#include "board.h"
#include <vector>

class Feature{
public:
	const int dim, n;
	std::vector<Board> tensor;
	Feature(int _n, int _d) : dim(_d), n(_n) {
		tensor.resize(dim);
		for (int i = 0; i < dim; ++i)
			tensor[i].resize(_n);
	}
	~Feature() {}
};

class GomokuFeature : public Feature{
public:
	GomokuFeature(int _n = 15, int _d = 5) : Feature(_n, _d) {}
	~GomokuFeature() {}
};

class GomokuFeatureExtractor{
public:
	void GetFeature(GomokuFeature &feature, const GomokuBoard& board);
};

#endif //FEATURE_H_