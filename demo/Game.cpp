#include "Game.h"
#include <cctype>
#include <cstdio>

void Game::Run() {
    int color = BLACK;
    while (true) {
        board_.Print();
        int x, y;
        while (true) {
            printf("%s: ", color == BLACK ? "Black" : "White");
            char buffer[10];
            scanf("%s", buffer);
            bool valid = islower(buffer[0]);
            for (int i = 1; valid && buffer[i]; ++i)
                valid = valid && isdigit(buffer[i]);
            if (!valid) {
                printf("Invalid input (Sample input: h8)\n");
                continue;
            }
            x = buffer[0] - 'a';
            sscanf(buffer + 1, "%d", &y);
            --y;
            if (!board_.IsInBoard(x, y)) {
                printf("Out of range\n");
                continue;
            }
            if (!board_.IsEmpty(x, y)) {
                printf("Stone exists\n");
                continue;
            }
            if (color == BLACK && board_.IsForbidden(x, y)) {
                printf("Forbidden move\n");
                continue;
            }
            break;
        }
        int result = board_.AddStone(x, y, color);
        if (result == BLACKFIVE) {
            printf("Black wins\n");
            break;
        }
        if (result == WHITEFIVE) {
            printf("White wins\n");
            break;
        }
        color = color == BLACK ? WHITE : BLACK;
    }
}