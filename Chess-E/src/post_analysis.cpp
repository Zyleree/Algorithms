#include "post_analysis.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

PostAnalysis::PostAnalysis() {
    logFile.open("engine_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "--- New Engine Session --- " << getTimestamp() << " ---" << std::endl;
    }
}

void PostAnalysis::logSearchData(const Board& board, const Move& bestMove, const IterativeDeepening::SearchInfo& searchInfo) {
    if (!logFile.is_open()) return;

    logFile << "--- Search Data --- " << getTimestamp() << std::endl;
    logFile << "Position: " << board.toFen() << std::endl;
    logFile << "Best Move: " << moveToString(bestMove) << std::endl;
    logFile << "Depth: " << searchInfo.depthSearched << std::endl;
    logFile << "Score: " << searchInfo.bestScore << std::endl;
    logFile << "Time: " << searchInfo.timeElapsed.count() << "ms" << std::endl;
    logFile << "Nodes: " << searchInfo.nodesEvaluated << std::endl;
    logFile << "Best Move: " << moveToString(searchInfo.bestMove) << std::endl;
    logFile << "------------------------" << std::endl;
}

void PostAnalysis::analyzeGame(const std::vector<Board>& gameHistory) {
    if (!logFile.is_open()) return;

    logFile << "--- Game Analysis --- " << getTimestamp() << " ---" << std::endl;
    logFile << "Total Moves: " << gameHistory.size() - 1 << std::endl;

    for (size_t i = 0; i < gameHistory.size() - 1; ++i) {
        const Board& currentBoard = gameHistory[i];
        logFile << "Move " << (i + 1) << ":" << std::endl;
        logFile << "Position: " << currentBoard.toFen() << std::endl;
        Move lastMove = deduceLastMove(currentBoard, gameHistory[i+1]);
        if (lastMove.startRow != -1) {
            logFile << "   Move: " << moveToString(lastMove) << std::endl;
        }
        logFile << "------------------------" << std::endl;
    }
}

void PostAnalysis::writeLogEntry(const std::string& entry) {
    if (logFile.is_open()) {
        logFile << entry << std::endl;
    }
}

std::string PostAnalysis::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    struct tm now_tm;
    localtime_s(&now_tm, &now_c);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string PostAnalysis::moveToString(const Move& move) const {
    if (move.startRow == -1) return "nomove";
    char startFile = 'a' + move.startCol;
    int startRank = 8 - move.startRow;
    char endFile = 'a' + move.endCol;
    int endRank = 8 - move.endRow;
    std::stringstream ss;
    ss << startFile << startRank << endFile << endRank;
    return ss.str();
}

Move PostAnalysis::deduceLastMove(const Board& boardBeforeMove, const Board& boardAfterMove) const {
    for (int startRow = 0; startRow < 8; ++startRow) {
        for (int startCol = 0; startCol < 8; ++startCol) {
            Piece beforePiece = boardBeforeMove.getPieceAt(startRow, startCol);
            if (!beforePiece.isEmpty()) {
                for (int endRow = 0; endRow < 8; ++endRow) {
                    for (int endCol = 0; endCol < 8; ++endCol) {
                        Piece afterPiece = boardAfterMove.getPieceAt(endRow, endCol);
                        if (beforePiece.type == afterPiece.type && beforePiece.color == afterPiece.color) {
                            if (startRow != endRow || startCol != endCol) {
                                return Move(startRow, startCol, endRow, endCol);
                            }
                        }
                    }
                }
            }
        }
    }
    return Move(-1, -1, -1, -1);
}