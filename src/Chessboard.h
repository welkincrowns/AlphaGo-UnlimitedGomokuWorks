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

inline Chessboard::Chessboard() {
}

inline Chessboard::~Chessboard() {
}

inline bool Chessboard::IsInBoard(int x, int y) {
    return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}

inline bool Chessboard::IsBlack(int x, int y) {
    return cBoard[x + 1][y + 1] == BLACKSTONE;
}

inline bool Chessboard::IsWhite(int x, int y) {
    return cBoard[x + 1][y + 1] == WHITESTONE;
}

inline bool Chessboard::IsEmpty(int x, int y) {
    return cBoard[x + 1][y + 1] == EMPTYSTONE;
}

inline bool Chessboard::IsForbidden(int x, int y) {
    return bForbidden[x][y];
}

inline bool Chessboard::IsFive(int x, int y, int color) {
    return CForbiddenPointFinder::IsFive(x, y, color);
}

inline bool Chessboard::IsFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsFour(x, y, color, i))
            return true;
    return false;
}

inline bool Chessboard::IsOpenFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenFour(x, y, color, i) == 1)
            return true;
    return false;
}

inline bool Chessboard::IsOpenThree(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            return true;
    return false;
}

inline bool Chessboard::IsDoubleFour(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleFour(x, y, color);
}

inline bool Chessboard::IsDoubleThree(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleThree(x, y, color);
}

inline int Chessboard::CountOpenFour(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenFour(x, y, color, i) == 1)
            ++count;
    return count;
}

inline int Chessboard::CountSimpleFour(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i) {
        int tmp = CForbiddenPointFinder::IsOpenFour(x, y, color, i);
        if (tmp == 2)
            count += 2;
        else if (!tmp && CForbiddenPointFinder::IsFour(x, y, color, i))
            ++count;
    }
    return count;
}

inline int Chessboard::CountOpenThree(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            ++count;
    return count;
}

inline int Chessboard::CountSimpleThree(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsThree(x, y, color, i) &&
            !CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            ++count;
    return count;
}

inline int Chessboard::CountOpenTwo(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenTwo(x, y, color, i))
            ++count;
    return count;
}

inline int Chessboard::AddStone(int x, int y, int color) {
    return CForbiddenPointFinder::AddStone(x, y, color == BLACK ? BLACKSTONE : WHITESTONE);
}

#endif // CHESSBOARD_H_