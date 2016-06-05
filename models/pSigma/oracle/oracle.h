#ifndef ORACLE_H_
#define ORACLE_H_

#include <iostream>
#include <Python.h>
#include <vector>
#include <cstring>

class Oracle{
public:
	PyObject* pModule;
	Oracle(int argc, char* argv[], const std::string& model_name);
	virtual ~Oracle();
	virtual void WriteFeature(const std::vector<std::pair<int, int> >& steps, const std::string& filename) = 0;
	virtual void ReadDistribution(const std::string& filename) = 0;
	void Execute(const std::vector<std::pair<int, int> >& steps, const std::string& feature_filename, const std::string& distribution_filename);
	std::vector<double> distribution;
};

class pSigmaOracle : public Oracle{
	const int d_size;
public:
	pSigmaOracle(int argc, char* argv[], const std::string& model_name, int _size = 225) : Oracle(argc, argv, model_name), d_size(_size) {}
	void WriteFeature(const std::vector<std::pair<int, int> >& steps, const std::string& filename);
	void ReadDistribution(const std::string& filename);
};

#endif //ORACLE_H_