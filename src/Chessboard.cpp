#include "Chessboard.h"
#include <cstdio>

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