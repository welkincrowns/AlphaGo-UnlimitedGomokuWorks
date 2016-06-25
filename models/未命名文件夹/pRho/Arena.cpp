#include "Arena.h"
#include "Chessboard.h"
#include "ForbiddenPointFinder.h"
#include "Playerp.h"
#include "Move.h"
#include "Oracle.h"
#include <cstdio>

//int GomokuArena::num_count = 0;

double GomokuArena::Play(int show /* = 0 */){
	int turn = 0;
	int tot_pawn = 0;

	std::vector<Move> steps;
	steps.clear();
	
	while (true){
		Move step = player[turn]->PlacePawn(steps);

		if (!board->IsInBoard(step.x, step.y)) return -1;
		if (!board->IsEmpty(step.x, step.y)) return -1;
		if (turn == BLACK && board->IsForbidden(step.x, step.y)) return -1;
		
		steps.push_back(step);
		int result = board->AddStone(step.x, step.y, step.turn);
		tot_pawn ++;

		if (result == turn){
			Record(steps);
			return turn;
		}
		if (tot_pawn == 15 * 15) break;
	}
	Record(steps);
	return 0.5;
}

void GomokuArena::Record(const std::vector<Move>& steps){
	char filename[10];
	sprintf(filename, "Arena/round%d.rif");
	
	FILE* fout = fopen(filename, "w");
	fprintf(fout, "<?xml version=\"1.0\"?>\n<database>\n<games>\n<game>\n<move>");
	fprintf(fout, "%c%d", steps[0].x + 'a', steps[0].y + 1);

	for (int i = 1; i < steps.size(); i ++)
		fprintf(fout, " %c%d", steps[i].x + 'a', steps[i].y + 1);
	fprintf(fout, "</move>\n</game>\n</games>\n</database>\n%d\n", int(steps.size()));

	fclose(fout);
}