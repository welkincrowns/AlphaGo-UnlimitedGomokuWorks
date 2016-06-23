#include <cstdio>
#include <vector>
#include "Chessboard.h"

using namespace std;

int main() {
    FILE* fin = fopen("data.txt", "r");
    FILE* fout[] = {fopen("data_pPhi_black.txt", "w"),
                    fopen("data_pPhi_white.txt", "w")};
    int t, x, y;
    while (fscanf(fin, "%d%d%d", &t, &x, &y) != EOF) {
        Chessboard board;
        int color = BLACK;
        while (t != -1) {
            if (t == 1) {
                auto feature = board.GetFeature(color);
                for (auto it = feature.begin(); it != feature.end(); ++it)
                    fprintf(fout[color], "%d\n", *it);
            }
            board.AddStone(x, y, color);
            color ^= 1;
            fscanf(fin, "%d%d%d", &t, &x, &y);
        }
    }
    return 0;
}