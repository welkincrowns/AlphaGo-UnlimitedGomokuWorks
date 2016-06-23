#include "Chessboard.h"
#include <cstdio>

Chessboard::Chessboard() {
}

Chessboard::~Chessboard() {
}

bool Chessboard::IsInBoard(int x, int y) {
    return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}

bool Chessboard::IsBlack(int x, int y) {
    return cBoard[x + 1][y + 1] == BLACKSTONE;
}

bool Chessboard::IsWhite(int x, int y) {
    return cBoard[x + 1][y + 1] == WHITESTONE;
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

int Chessboard::CountOpenFour(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenFour(x, y, color, i) == 1)
            ++count;
    return count;
}

int Chessboard::CountSimpleFour(int x, int y, int color) {
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

int Chessboard::CountOpenThree(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            ++count;
    return count;
}

int Chessboard::CountSimpleThree(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsThree(x, y, color, i) &&
            !CForbiddenPointFinder::IsOpenThree(x, y, color, i))
            ++count;
    return count;
}

int Chessboard::CountOpenTwo(int x, int y, int color) {
    int count = 0;
    for (int i = 1; i <= 4; ++i)
        if (CForbiddenPointFinder::IsOpenTwo(x, y, color, i))
            ++count;
    return count;
}

int Chessboard::AddStone(int x, int y, int color) {
    return CForbiddenPointFinder::AddStone(x, y, color == BLACK ? BLACKSTONE : WHITESTONE);
}

std::vector<int> Chessboard::GetFeature(int color) {
    std::vector<int> feature;
    for (int i = 0; i < BOARDSIZE; ++i)
        for (int j = 0; j < BOARDSIZE; ++j) {
            feature.push_back(color == BLACK ? IsBlack(i, j) : IsWhite(i, j));
            feature.push_back(color == WHITE ? IsWhite(i, j) : IsBlack(i, j));
            feature.push_back(IsEmpty(i, j));
            feature.push_back(CountOpenThree(i, j, color));
            feature.push_back(CountSimpleThree(i, j, color));
            feature.push_back(CountOpenFour(i, j, color));
            feature.push_back(CountSimpleFour(i, j, color));
            feature.push_back(CountOpenTwo(i, j, color));
            feature.push_back(IsFive(i, j, color));
            feature.push_back(CountOpenThree(i, j, color ^ 1));
            feature.push_back(CountSimpleThree(i, j, color ^ 1));
            feature.push_back(CountOpenFour(i, j, color ^ 1));
            feature.push_back(CountSimpleFour(i, j, color ^ 1));
            feature.push_back(CountOpenTwo(i, j, color ^ 1));
            feature.push_back(IsFive(i, j, color ^ 1));
        }
    return feature;
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