#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "Chessboard.h"
#include "Move.h"

class TreeNode;

class TreeEdge {
  public:
    TreeEdge(Move _move, double _p, TreeNode* _node)
            : move(_move), nv(0), nr(0), p(_p), q(0), wv(0), wr(0), node(_node) {
        //printf("move = (%d, %d, %d) nv = %d nr = %d p = %.1f q = %.1f wv = %.1f wr = %.1f\n", move.turn, move.x, move.y, nv, nr, p, q, wv, wr);
    }
    ~TreeEdge() {
        delete node;
    }
    Move move;
    int nv, nr;
    double p, q, wv, wr;
    TreeNode* node;
};

class TreeNode {
  public:
    static const int BASE1 = 10007;
    static const int BASE2 = 10009;
    static const int MOD1 = 1000000007;
    static const int MOD2 = 1000000009;
    TreeNode(int _turn = 0) : is_in_tree(false), turn(_turn), result(-1) {
        CalcHash();
    }
    TreeNode(Chessboard _pre_board, Move _pre_move)
            : is_in_tree(false), turn(_pre_move.turn ^ 1), board(_pre_board) {
        result = board.AddStone(_pre_move.x, _pre_move.y, _pre_move.turn);
        //printf("New TreeNode\n");
        //printf("pre board \n");
        //_pre_board.Print();
        //printf("new board \n");
        //board.Print();
        CalcHash();
    }
    ~TreeNode() {
        for (auto it = edges.begin(); it != edges.end(); ++it)
            delete *it;
    }
    void CalcHash() {
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j) {
                hash1 = ((long long)hash1 * BASE1 + (board.IsEmpty(i, j) ? 0 : board.IsBlack(i, j) ? 1 : 2)) % MOD1;
                hash2 = ((long long)hash2 * BASE2 + (board.IsEmpty(i, j) ? 0 : board.IsBlack(i, j) ? 1 : 2)) % MOD2;
            }
    }
    void Activate(std::vector<double>& distribution) {
        is_in_tree = true;
        for (int i = 0; i < BOARDSIZE; ++i)
            for (int j = 0; j < BOARDSIZE; ++j)
                if (board.IsEmpty(i, j) && (!board.IsForbidden(i, j) || turn == WHITE))
                    edges.push_back(new TreeEdge(Move(turn, i, j), distribution[i * 15 + j], new TreeNode(board, Move(turn, i, j))));
    }
    bool is_in_tree;
    int turn, hash1, hash2, result;
    Chessboard board;
    std::vector<TreeEdge*> edges;
};

#endif // TREE_H_