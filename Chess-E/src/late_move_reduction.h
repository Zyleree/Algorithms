#ifndef LATE_MOVE_REDUCTION_H
#define LATE_MOVE_REDUCTION_H

#include "board.h"
#include "alphabeta.h" 
#include <limits>
#include <algorithm>

class LateMoveReduction : public AlphaBetaSearch {
public:
    LateMoveReduction();
    int lateMoveReductionAlphaBeta(Board& board, int depth, int alpha, int beta, int moveCount);

private:
    int reduction(int depth, int moveCount);
    bool isGoodMove(const Board& board, const Move& move) const;
    int getMoveScoreForLMR(const Board& board, const Move& move) const;
};

#endif 