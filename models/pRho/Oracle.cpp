#include "Oracle.h"
#include "Move.h"
#include <fstream>
#include <iostream>

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
	//return;
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
		out << steps[i].x + 1 << std::endl << steps[i].y + 1 << std::endl;
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

pRhoOracle::pRhoOracle(int argc, char* argv[], const std::string& model_name, const std::string& swordname, int _size /*= 225*/) : Oracle(argc, argv, model_name), d_size(_size) {
	//std::cout << "Trace on:(Oracle) " << model_name << std::endl;
	PyObject* pFunc = PyObject_GetAttrString(pModule, "load_player");
	if (!pFunc){
		PyErr_Print();
		std::cout << "pRhoOracle::pRhoOracle() ERROR: get func failed!" << std::endl;
		assert(0);
	}
	PyObject* pParam = Py_BuildValue("(s)", swordname.c_str());
	PyObject* pResult = PyEval_CallObject(pFunc, pParam);
	if (!pResult){
		PyErr_Print();
		std::cout << "pRhoOracle::pRhoOracle() ERROR: get result failed!" << std::endl;
		assert(0);
	}
	/*
	std::string p1;
	if(PyArg_Parse(pResult, "(s)", &p1)){
		std::cout << p1 << std::endl;
	}*/
}

void pRhoOracle::WriteFeature(const std::vector<Move>& steps, const std::string& filename){
	std::ofstream out(filename.c_str());
	if (!out.is_open()){
		std::cout << "pRhoOracle::WriteFeature() ERROR: can't open file!" << std::endl;
		assert(0);
	}
	for (int i = 0; i < steps.size(); ++i)
		out << steps[i].x + 1 << std::endl << steps[i].y + 1 << std::endl;
	out.close();
}

void pRhoOracle::ReadDistribution(const std::string& filename){
	std::ifstream in(filename.c_str());
	if (!in.is_open()){
		std::cout << "pRhoOracle::ReadDistribution() ERROR: can't open file!" << std::endl;
		assert(0);
	}

	for (int i = 0; i < d_size; ++i){
		double t = 0;
		in >> t;
		distribution.push_back(t);
	}

	in.close();
}
