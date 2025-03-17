#ifndef NULL_MOVE_PRUNING_H
#define NULL_MOVE_PRUNING_H

#include "board.h"
#include "alphabeta.h" 
#include <limits>
#include <algorithm>

class NullMovePruning : public AlphaBetaSearch {
public:
    NullMovePruning();
    int nullMoveAlphaBeta(Board& board, int depth, int alpha, int beta);

private:
    int R; 
    bool hasNonPawnMaterial(const Board& board) const;
};

#endif 