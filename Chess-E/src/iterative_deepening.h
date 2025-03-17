#ifndef ITERATIVE_DEEPENING_H
#define ITERATIVE_DEEPENING_H

#include "board.h"
#include "move.h"
#include "alphabeta.h"
#include <chrono>
#include <vector>
#include <atomic>

class Search;

class IterativeDeepening {
public:
    IterativeDeepening();
    Move getBestMove() const;
    void startSearch(Board& board, int maxDepth, std::chrono::milliseconds timeLimit);
    struct SearchInfo {
        int depthSearched;
        int nodesEvaluated;
        std::chrono::milliseconds timeElapsed;
        Move bestMove;
        int bestScore;
    };
    SearchInfo getSearchInfo() const;


private:
    AlphaBetaSearch searchEngine;
    Move bestMoveFound;
    int bestScoreFound;
    int currentDepth;
    std::chrono::milliseconds timeLimit;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::atomic<bool> stopSearch;
    SearchInfo lastSearchInfo;

    void iterativeSearch(Board& board, int maxDepth);
    bool isTimeExpired() const;
};

#endif 