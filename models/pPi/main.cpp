#include <cstdio>
#include <vector>
#include "Chessboard.h"

using namespace std;

int main() {
    FILE* fin = fopen("data.txt", "r");
    FILE* fout[] = {fopen("data_pPi_black.txt", "w"),
                    fopen("data_pPi_white.txt", "w")};
    FILE* outlog = fopen("log.txt", "w")
    int t, x, y;

    // for test
    int tot = 0;
    // end of for test

    while (fscanf(fin, "%d%d%d", &t, &x, &y) != EOF) {
        Chessboard board;
        
        int color = BLACK;
        while (t != -1) {
            // for test
            // get 10000 data
            //if (++tot > 10000)
            //    break;
            // end of for test

            if (t == 1) {
                auto feature = board.GetFeature(color);
                for (auto it = feature.begin(); it != feature.end(); ++it)
                    fprintf(fout[color], "%d\n", *it);
                // start from index 0
                fprintf(fout[color], "%d\n%d\n", x - 1, y - 1);
            }
            board.AddStone(x, y, color);
            color ^= 1;
            fscanf(fin, "%d%d%d", &t, &x, &y);

            if (tot % 10000 == 0)
                fprintf(outlog, "%d tests computed\n", tot);
        }

        if (tot > 10000) break;
    }
    return 0;
}