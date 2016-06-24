#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include <vector>
#include "ForbiddenPointFinder.h"

#define BLACK 0
#define WHITE 1

class Chessboard : public CForbiddenPointFinder {
  public:
    Chessboard();
    ~Chessboard();
    static bool IsInBoard(int x, int y);
    bool IsBlack(int x, int y);
    bool IsWhite(int x, int y);
    bool IsEmpty(int x, int y);
    bool IsForbidden(int x, int y);
    bool IsFive(int x, int y, int color);
    bool IsFour(int x, int y, int color);
    bool IsOpenFour(int x, int y, int color);
    bool IsOpenThree(int x, int y, int color);
    bool IsDoubleFour(int x, int y, int color);
    bool IsDoubleThree(int x, int y, int color);
    int CountOpenFour(int x, int y, int color);
    int CountSimpleFour(int x, int y, int color);
    int CountOpenThree(int x, int y, int color);
    int CountSimpleThree(int x, int y, int color);
    int CountOpenTwo(int x, int y, int color);
    int AddStone(int x, int y, int color);
    std::vector<int> GetFeature(int color);
    void Print();
};

#endif // CHESSBOARD_H_