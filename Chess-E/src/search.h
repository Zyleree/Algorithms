#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "movegen.h"
#include "evaluation.h"
#include "transposition_table.h"
#include "quiescence_search.h"
#include "iterative_deepening.h"
#include "alphabeta.h"
#include "move_ordering.h"
#include "principal_variation_search.h"
#include "null_move_pruning.h"
#include "late_move_reduction.h"
#include "aspiration_windows.h"
#include "opening_book.h"
#include "endgame_tablebase.h"
#include "time_management.h"
#include "search_extensions.h"
#include "post_analysis.h"
#include "thread_pool.h"

#include <chrono>

class Search {
public:
    Search();

    Move findBestMove(Board& board, int depth, std::chrono::milliseconds timeLimit);
    const TranspositionTable& getTranspositionTable() const;

private:
    Evaluation evaluationFunction;
    MoveGenerator moveGenerator;
    TranspositionTable transpositionTable;
    QuiescenceSearch quiescenceSearch;
    IterativeDeepening iterativeDeepening;
    AlphaBetaSearch alphaBetaSearch;
    MoveOrdering moveOrdering;
    PrincipalVariationSearch pvSearch;
    NullMovePruning nullMovePruning;
    LateMoveReduction lateMoveReduction;
    AspirationWindows aspirationWindows;
    OpeningBook openingBook;
    EndgameTablebase endgameTablebase;
    TimeManagement timeManagement;
    SearchExtensions searchExtensions;
    PostAnalysis postAnalysis;
    ThreadPool threadPool;

    int searchDepth;
    std::chrono::milliseconds searchTimeLimit;

    int alphaBeta(Board& board, int depth, int alpha, int beta);
    int quiescence(Board& board, int alpha, int beta);
};

#endif