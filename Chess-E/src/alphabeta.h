#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "board.h"
#include "movegen.h"
#include "evaluation.h"
#include "transposition_table.h"
#include "quiescence_search.h"
#include "move_ordering.h"

class AlphaBetaSearch {
public:
    AlphaBetaSearch();
    int alphaBeta(Board& board, int depth, int alpha, int beta);
    const TranspositionTable& getTranspositionTable() const;

protected:
    Evaluation evaluationFunction;
    MoveGenerator moveGenerator;
    TranspositionTable transpositionTable;
    QuiescenceSearch quiescenceSearch;
    MoveOrdering moveOrdering;
};

#endif 