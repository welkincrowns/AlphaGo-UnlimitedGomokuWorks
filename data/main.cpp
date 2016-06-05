#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include "Chessboard.h"

using namespace std;

int start;

pair<bool, string> Search(Chessboard board, int color, int winner) {
    pair<bool, string> temp;
    if (color == winner) {
        if (clock() - start > CLOCKS_PER_SEC * 3)
            return make_pair(false, string());
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && board.IsFive(i, j, color)) {
                    string str;
                    str += char(j);
                    str += char(i);
                    return make_pair(true, str);
                }
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && (color == WHITE || (color == BLACK && !board.IsForbidden(i, j))) && (board.IsOpenFour(i, j, color) || (board.IsFour(i, j, color) && board.IsOpenThree(i, j, color)) || board.IsDoubleFour(i, j, color) || board.IsDoubleThree(i, j, color))) {
                    Chessboard newboard = board;
                    newboard.AddStone(i, j, color);
                    temp = Search(newboard, color ^ 1, winner);
                    if (!temp.first) {
                        string str = temp.second;
                        str += char(j);
                        str += char(i);
                        return make_pair(true, str);
                    }
                }
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && (color == WHITE || (color == BLACK && !board.IsForbidden(i, j))) && board.IsFour(i, j, color)) {
                    Chessboard newboard = board;
                    newboard.AddStone(i, j, color);
                    temp = Search(newboard, color ^ 1, winner);
                    if (!temp.first) {
                        string str = temp.second;
                        str += char(j);
                        str += char(i);
                        return make_pair(true, str);
                    }
                }
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && (color == WHITE || (color == BLACK && !board.IsForbidden(i, j))) && board.IsOpenThree(i, j, color)) {
                    Chessboard newboard = board;
                    newboard.AddStone(i, j, color);
                    temp = Search(newboard, color ^ 1, winner);
                    if (!temp.first) {
                        string str = temp.second;
                        str += char(j);
                        str += char(i);
                        return make_pair(true, str);
                    }
                }
        return make_pair(false, string());
    }
    else {
        if (clock() - start > CLOCKS_PER_SEC * 3)
            return make_pair(true, string());
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && board.IsFive(i, j, color)) {
                    string str;
                    str += char(j);
                    str += char(i);
                    return make_pair(true, str);
                }
        string ret;
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && (color == WHITE || (color == BLACK && !board.IsForbidden(i, j))) && board.IsFive(i, j, color ^ 1)) {
                    Chessboard newboard = board;
                    newboard.AddStone(i, j, color);
                    temp = Search(newboard, color ^ 1, winner);
                    if (!temp.first) {
                        string str = temp.second;
                        str += char(j);
                        str += char(i);
                        return make_pair(true, string());
                    }
                    else if (ret.empty()) {
                        ret = temp.second;
                        ret += char(j);
                        ret += char(i);
                    }
                }
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && !board.IsForbidden(i, j) && (board.IsOpenFour(i, j, color) || (board.IsFour(i, j, color ^ 1) && board.IsOpenThree(i, j, color ^ 1)) || board.IsDoubleFour(i, j, color ^ 1) || board.IsDoubleThree(i, j, color ^ 1))) {
                    Chessboard newboard = board;
                    newboard.AddStone(i, j, color);
                    temp = Search(newboard, color ^ 1, winner);
                    if (!temp.first) {
                        string str = temp.second;
                        str += char(j);
                        str += char(i);
                        return make_pair(true, string());
                    }
                    else if (ret.empty()) {
                        ret = temp.second;
                        ret += char(j);
                        ret += char(i);
                    }
                }
        if (!ret.empty())
            return make_pair(false, ret);
        return make_pair(true, string());
    }
}

int main() {
    const int LEN = 1000;
    FILE* fin = fopen("renjunet_v10_20160529.rif.html", "r");
    FILE* fou = fopen("data.txt", "w");
    char str[LEN];
    while (fgets(str, LEN, fin) != NULL)
        if (!strcmp(str, "<games>\r\n"))
            break;
    int count = 0, win = 0;
    while (fgets(str, LEN, fin) != NULL) {
        if (!strcmp(str, "</games>\r\n"))
            break;
        int rule;
        double result;
        sscanf(str, "%*s%*s%*s%*s%*s rule=\"%d\"%*s%*s bresult=\"%lf\"", &rule, &result);
        if (rule != 1) {
            fgets(str, LEN, fin);
            fgets(str, LEN, fin);
            if (str[1] == 'i')
                fgets(str, LEN, fin);
            continue;
        }
        fscanf(fin, "<move>");
        Chessboard board;
        bool valid = true;
        int color = BLACK;
        string moves;
        while (fscanf(fin, "%s", str) == 1) {
            if (!islower(str[0])) {
                valid = false;
                break;
            }
            int x = str[0] - 'a', y;
            sscanf(str + 1, "%d", &y);
            --y;
            assert(board.IsInBoard(x, y));
            assert(board.IsEmpty(x, y));
            if (color == BLACK && board.IsForbidden(x, y)) {
                valid = false;
                break;
            }
            assert(color == WHITE || color == BLACK && !board.IsForbidden(x, y));
            if (board.AddStone(x, y, color) != -1) {
                valid = false;
                break;
            }
            assert(board.AddStone(x, y, color) == -1);
            moves += char(x);
            moves += char(y);
            color ^= 1;
            if (strlen(str) > 3)
                break;
        }
        if (valid) {
            ++count;
            for (int i = 0; i < moves.length(); i += 2)
                fprintf(fou, "1\n%d\n%d\n", moves[i] + 1, moves[i + 1] + 1);

            // result = 0.5
            if (fabs(result - 0.5) < 1e-6)
                fprintf(fou, "-1\n-1\n-1\n");
            else{
                int winner = fabs(result - 1) < 1e-6 ? BLACK : WHITE;
                start = clock();
                pair<bool, string> temp = Search(board, color, winner);
                if (temp.first == (color == winner)) {
                    ++win;
                    char filename[10];
                    sprintf(filename, "ForRenLib/%d.rif", win);
                    FILE* fout = fopen(filename, "w");
                    fprintf(fout, "<?xml version=\"1.0\"?>\n<database>\n<games>\n<game>\n<move>");
                    fprintf(fout, "%c%d", moves[0] + 'a', moves[1] + 1);
                    for (int i = 2; i < moves.length(); i += 2)
                        fprintf(fout, " %c%d", moves[i] + 'a', moves[i + 1] + 1);
                    for (int i = temp.second.length() - 1; i >= 0; i -= 2)
                        fprintf(fout, " %c%d", temp.second[i] + 'a', temp.second[i - 1] + 1);
                    fprintf(fout, "</move>\n</game>\n</games>\n</database>\n%d\n", int(moves.length()) / 2);
                    fclose(fout);
                    for (int i = temp.second.length() - 1; i >= 0; i -= 2) {
                        fprintf(fou, "%d\n%d\n%d\n", color == winner ? 1 : 0, temp.second[i] + 1, temp.second[i - 1] + 1);
                        color ^= 1;
                    }
                }
            }
            fprintf(fou, "-1\n-1\n-1\n");
            printf("count = %d win = %d\n", count, win);
        }
        fgets(str, LEN, fin);
        fgets(str, LEN, fin);
        if (str[1] == 'i')
            fgets(str, LEN, fin);
    }
    printf("%d %d\n", count, win);
    return 0;
}