#include "Playerp.h"
#include <string>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string moveToStr(Move a){
	stringstream ss;
	ss << char('A' + a.x + (a.x >= 8 ? 1 : 0) ) << a.y + 1;
	cerr << "moveToStr: " << a << "##" << ss.str() << endl;
	return ss.str();
}

Move strToMove(string turn, string b){
	Move a;
	a.turn = (turn == "black" ? BLACK : WHITE);
	a.x = b[0] - 'A';
	if (a.x > 8) -- a.x;
	if (b.length() == 2) a.y = (b[1] - '0') - 1;
	else a.y = (b[1] - '0') * 10 + (b[2] - '0') - 1;
	return a;
}

vector<string> split(const string &s) {
	vector<string> elems;
	stringstream ss(s);
	string buf;
	while (ss >> buf)
		elems.push_back(buf);
	return elems;
}

void GTP_interact(int argc, char* argv[], GomokuBoard* board, GomokuPlayer* player, int winResult, string color){
	vector<Move> steps;
	steps.clear();

	string command;
	while (getline(cin, command) ) {
		vector<string> s = split(command);
		cerr << "Get Command " << command << endl;
		if (s[0] == "genmove"){
			Move a = player->PlacePawn(steps);
			steps.push_back(a);
			int result = board->AddStone(a.x, a.y, a.turn);
			cout << "= ";
			if (result == winResult) {
				cout << "!";
			}
			cout << moveToStr(a) << endl;
		}
		else if (s[0] == "play"){
			Move b = strToMove(s[1], s[2]);
			steps.push_back(b);
			board->AddStone(b.x, b.y, b.turn);
			cout << "= " << endl;
		}
		else if (s[0] == "undo"){
			steps.pop_back();
			cout << "= " << endl;
		}
		else if (s[0] == "bannedpoint"){
			cout << "= ";
			for (int i = 0; i < 15; ++ i)
				for (int j = 0; j < 15; ++ j)
					if (board->IsForbidden(i, j) )
						cout << i << " " << j << " ";
			cerr << "= " << endl;
			cout << endl;
		}
		else if (s[0] == "showboard"){
			cout << "= ";
			board->Print();
			cout << "" << endl;
		}
		else if (s[0] == "name"){
			cout << "= pSigma GTP demo" << endl;
		}
		else if (s[0] == "version"){
			cout << "= version 0" << endl;
		}
		else if (s[0] == "boardsize"){
			cout << "= " << endl;
		}
		else if (s[0] == "quit"){
			break;
		}
		else {
			cout << "= unknown command" << endl;
		}
	}		
}

int main(int argc, char* argv[]){
	Py_Initialize();
	// test psigma_black
	// psigma_black(argc, argv);
	// test psigma_white
	if (string(argv[1]) == "black"){
		GomokuBoard board;
		pSigmaOracle *psigma_black = new pSigmaOracle(argc, argv, "pSigma_black");
		pSigmaOracle *psigma_white = new pSigmaOracle(argc, argv, "pSigma_white");
		vThetaOracle *vtheta = new pSigmaOracle(argc, argv, "pSigma_black");
		MCTSGomokuPlayer player1(psigma_black, psigma_white, vtheta, 0, 1, 5, 1, 5);
		GTP_interact(argc, argv, &board, &player1, BLACKFIVE, "black");
	}
	else if (string(argv[1]) == "white"){
		GomokuBoard board;
		pSigmaOracle *psigma_black = new pSigmaOracle(argc, argv, "pSigma_black");
		pSigmaOracle *psigma_white = new pSigmaOracle(argc, argv, "pSigma_white");
		vThetaOracle *vtheta = new pSigmaOracle(argc, argv, "pSigma_black");
		MCTSGomokuPlayer player1(psigma_black, psigma_white, vtheta, 1, 1, 5, 1, 5);
		GTP_interact(argc, argv, &board, &player1, WHITEFIVE, "white");
	}
	//GTP_white(argc, argv);
	Py_Finalize();
}
