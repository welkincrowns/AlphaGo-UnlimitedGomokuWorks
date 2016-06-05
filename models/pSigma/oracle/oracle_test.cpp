#include "oracle.h"
using namespace std;

int main(int argc, char* argv[]){
	Py_Initialize();
	
	string model_name = "pSigma";
	pSigmaOracle t(argc, argv, model_name);

	vector<pair<int, int> > steps;
	steps.clear();
	t.Execute(steps, "pSigma_feature.al", "pSigma_distribution.al");
	cout << t.distribution[7 * 15 + 7] << endl;

	Py_Finalize();
}