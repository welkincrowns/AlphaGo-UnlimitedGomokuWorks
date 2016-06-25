#include "Playerp.h"

int main() {
    FILE* fin = fopen("data.txt", "r");
    int t, x, y;
    int black_acc = 0, black_tot = 0, white_acc = 0, white_tot = 0;
    while (fscanf(fin, "%d%d%d", &t, &x, &y) != EOF) {
        GomokuBoard board;
        //GreedyGomokuPlayer player1(&board, 0);
        //GreedyGomokuPlayer player2(&board, 1);
        KnowledgeableGomokuPlayer player1(&board, 0);
        KnowledgeableGomokuPlayer player2(&board, 1);
        std::vector<Move> steps;
        int color = BLACK;
        while (t != -1) {
            --x;
            --y;
            if (t == 1) {
                if (color == BLACK) {
                    Move a = player1.PlacePawn(steps);
                    if (a.x == x && a.y == y)
                        ++black_acc;
                    ++black_tot;
                    if (black_tot % 1000 == 0)
                        printf("Black: acc = %d tot = %d\n", black_acc, black_tot);
                }
                else {
                    Move a = player2.PlacePawn(steps);
                    if (a.x == x && a.y == y)
                        ++white_acc;
                    ++white_tot;
                    if (white_tot % 1000 == 0)
                        printf("White: acc = %d tot = %d\n", white_acc, white_tot);
                }
            }
            board.AddStone(x, y, color);
            color ^= 1;
            fscanf(fin, "%d%d%d", &t, &x, &y);
        }
    }
    printf("Black: acc = %d tot = %d\n", black_acc, black_tot);
    printf("White: acc = %d tot = %d\n", white_acc, white_tot);
    return 0;
}