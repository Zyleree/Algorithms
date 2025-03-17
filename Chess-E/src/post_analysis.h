#ifndef POST_ANALYSIS_H
#define POST_ANALYSIS_H

#include "board.h"
#include "move.h"
#include "iterative_deepening.h"
#include <string>
#include <fstream>
#include <vector>

class PostAnalysis {
public:
    PostAnalysis();
    void logSearchData(const Board& board, const Move& bestMove, const IterativeDeepening::SearchInfo& searchInfo);
    void analyzeGame(const std::vector<Board>& gameHistory);

private:
    void writeLogEntry(const std::string& entry);
    std::string getTimestamp() const;
    std::string moveToString(const Move& move) const;
    Move deduceLastMove(const Board& boardBeforeMove, const Board& boardAfterMove) const;
    
    std::ofstream logFile;
};

#endif 