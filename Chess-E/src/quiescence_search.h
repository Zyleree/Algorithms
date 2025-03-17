#pragma once

#include "board.h"
#include "movegen.h"
#include "evaluation.h"
#include <vector>

class QuiescenceSearch {
public:
    QuiescenceSearch();
    std::vector<Move> generateCaptureMoves(const Board& board) const;
    int quiescence(Board& board, int alpha, int beta);
};