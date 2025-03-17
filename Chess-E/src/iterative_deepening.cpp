#include "iterative_deepening.h"
#include <iostream>
#include <algorithm>

IterativeDeepening::IterativeDeepening() :
    searchEngine(),
    bestMoveFound(Move(-1,-1,-1,-1)),
    bestScoreFound(Evaluation::VALUE_UNKNOWN),
    currentDepth(0),
    timeLimit(std::chrono::seconds(5)),
    stopSearch(false),
    lastSearchInfo({0, 0, std::chrono::milliseconds(0), Move(-1,-1,-1,-1), Evaluation::VALUE_UNKNOWN})
{
}

Move IterativeDeepening::getBestMove() const {
    return bestMoveFound;
}

IterativeDeepening::SearchInfo IterativeDeepening::getSearchInfo() const {
    return lastSearchInfo;
}


void IterativeDeepening::startSearch(Board& board, int maxDepth, std::chrono::milliseconds timeLimit) {
    startTime = std::chrono::steady_clock::now();
    this->timeLimit = timeLimit;
    stopSearch = false;
    bestMoveFound = Move(-1, -1, -1, -1); 
    bestScoreFound = Evaluation::VALUE_UNKNOWN;

    iterativeSearch(board, maxDepth);

    lastSearchInfo.bestMove = bestMoveFound;
    lastSearchInfo.bestScore = bestScoreFound;
    lastSearchInfo.timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    lastSearchInfo.depthSearched = currentDepth;
    lastSearchInfo.nodesEvaluated = 0; 
}


void IterativeDeepening::iterativeSearch(Board& board, int maxDepth) {
    for (int depth = 1; depth <= maxDepth; ++depth) {
        currentDepth = depth;
        if (isTimeExpired() || stopSearch) break;

        int score = searchEngine.alphaBeta(board, depth, -Evaluation::INFINITY_VALUE, Evaluation::INFINITY_VALUE);
        
        Move ttBestMove = searchEngine.getTranspositionTable().getBestMove(board);
        
        if (ttBestMove.startRow != -1) {
            bestMoveFound = ttBestMove;
        } else {
            std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
            if (!legalMoves.empty()) {
                Board testBoard = board;
                int bestScore = -Evaluation::INFINITY_VALUE;
                
                for (const auto& move : legalMoves) {
                    Board nextBoard = board;
                    nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
                    nextBoard.switchTurn();
                    
                    int moveScore = -searchEngine.alphaBeta(nextBoard, 1, -Evaluation::INFINITY_VALUE, -bestScore);
                    
                    if (moveScore > bestScore) {
                        bestScore = moveScore;
                        bestMoveFound = move;
                    }
                }
            }
        }
        
        bestScoreFound = score; 

        std::cout << "Depth " << depth << " score: " << score << " move: ";
        if (bestMoveFound.startRow != -1) {
            char startFile = 'a' + bestMoveFound.startCol;
            int startRank = 8 - bestMoveFound.startRow;
            char endFile = 'a' + bestMoveFound.endCol;
            int endRank = 8 - bestMoveFound.endRow;
            std::cout << startFile << startRank << endFile << endRank;
        } else {
            std::cout << "None";
        }
        std::cout << std::endl;

        if (std::abs(score) > Evaluation::CHECKMATE_VALUE - maxDepth) {
            break;
        }
    }
}


bool IterativeDeepening::isTimeExpired() const {
    return std::chrono::steady_clock::now() - startTime > timeLimit;
}