#pragma once

#include "board.h"
#include "move.h"
#include "evaluation.h"
#include <unordered_map>

class TranspositionTable {
public:
    TranspositionTable();

    int probe(const Board& board, int depth, int alpha, int beta);
    void store(const Board& board, int depth, int value, int type, const Move& bestMove);
    Move getBestMove(const Board& board) const;

    static const int EXACT_VALUE = 0;
    static const int ALPHA_VALUE = 1;
    static const int BETA_VALUE = 2;

private:
    struct TTEntry {
        int value;
        int depth;
        int type;
        Move bestMove;
    };

    std::unordered_map<std::string, TTEntry> table; // Key is FEN string for simplicity (hash is better in real engine)
};