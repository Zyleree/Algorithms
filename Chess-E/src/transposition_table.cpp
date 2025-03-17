#include "transposition_table.h"

TranspositionTable::TranspositionTable() : table() {} 

int TranspositionTable::probe(const Board& board, int depth, int alpha, int beta) {
    std::string fen = board.toFen();
    auto it = table.find(fen);
    if (it != table.end()) {
        const TTEntry& entry = it->second;
        if (entry.depth >= depth) {
            if (entry.type == EXACT_VALUE) {
                return entry.value;
            } else if (entry.type == ALPHA_VALUE && entry.value <= alpha) {
                return alpha;
            } else if (entry.type == BETA_VALUE && entry.value >= beta) {
                return beta;
            }
        }
    }
    return Evaluation::VALUE_UNKNOWN;
}

void TranspositionTable::store(const Board& board, int depth, int value, int type, const Move& bestMove) {
    std::string fen = board.toFen();
    TTEntry entry;
    entry.value = value;
    entry.depth = depth;
    entry.type = type;
    entry.bestMove = bestMove;
    table[fen] = entry;
}

Move TranspositionTable::getBestMove(const Board& board) const {
    std::string fen = board.toFen();
    auto it = table.find(fen);
    if (it != table.end()) {
        return it->second.bestMove;
    }
    return Move(-1, -1, -1, -1); 
}