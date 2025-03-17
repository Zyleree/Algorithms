#ifndef MOVE_ORDERING_H
#define MOVE_ORDERING_H

#include "board.h"
#include "move.h"
#include "evaluation.h"
#include <vector>

class MoveOrdering {
public:
    MoveOrdering();
    void orderMoves(const Board& board, std::vector<Move>& moves);

private:
    int getMoveScore(const Board& board, const Move& move); 
};

#endif 