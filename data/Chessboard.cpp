#include "Chessboard.h"
#include <cstdio>

Chessboard::Chessboard() {
}

Chessboard::~Chessboard() {
}

bool Chessboard::IsInBoard(int x, int y) {
    return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}

bool Chessboard::IsEmpty(int x, int y) {
    return cBoard[x + 1][y + 1] == EMPTYSTONE;
}

bool Chessboard::IsForbidden(int x, int y) {
    return bForbidden[x][y];
}

bool Chessboard::IsFive(int x, int y, int color) {
    return CForbiddenPointFinder::IsFive(x, y, color);
}

bool Chessboard::IsFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsFour(x, y, color, i))
            return true;
    return false;
}

bool Chessboard::IsOpenFour(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenFour(x, y, color, i) == 1)
            return true;
    return false;
}

bool Chessboard::IsOpenThree(int x, int y, int color) {
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            return true;
    return false;
}

bool Chessboard::IsDoubleFour(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleFour(x, y, color);
}

bool Chessboard::IsDoubleThree(int x, int y, int color) {
    return CForbiddenPointFinder::IsDoubleThree(x, y, color);
}

int Chessboard::AddStone(int x, int y, int color) {
    return CForbiddenPointFinder::AddStone(x, y, color == BLACK ? BLACKSTONE : WHITESTONE);
}

void Chessboard::Print() {
    for (int j = BOARDSIZE; j >= 1; --j) {
        printf("%2d", j);
        for (int i = 1; i <= BOARDSIZE; ++i)
            printf(" %c", cBoard[i][j]);
        printf("\n");
    }
    printf("  ");
    for (int i = 0; i < BOARDSIZE; ++i)
        printf(" %c", i + 'a');
    printf("\n");
}