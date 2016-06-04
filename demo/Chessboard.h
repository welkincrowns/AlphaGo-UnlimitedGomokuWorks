#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include "ForbiddenPointFinder.h"

#define BLACK 0
#define WHITE 1

class Chessboard : public CForbiddenPointFinder {
  public:
    Chessboard();
    ~Chessboard();
    static bool IsInBoard(int x, int y);
    bool IsEmpty(int x, int y);
    bool IsForbidden(int x, int y);
    bool IsFive(int x, int y, int color);
    bool IsFour(int x, int y, int color);
    int AddStone(int x, int y, int color);
    void Print();
};

#endif // CHESSBOARD_H_