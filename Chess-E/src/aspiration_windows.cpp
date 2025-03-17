#include "aspiration_windows.h"
#include <algorithm>

AspirationWindows::AspirationWindows() : AlphaBetaSearch(), windowSize(50) {}

int AspirationWindows::aspirationWindowSearch(Board& board, int depth, int previousScore) {
    int alpha = previousScore - windowSize;
    int beta = previousScore + windowSize;
    int score = alphaBeta(board, depth, alpha, beta);
    int reSearchCount = 0;

    while (reSearchCount < 2) { 
        if (score <= alpha) {
            alpha = -Evaluation::INFINITY_VALUE;
            beta = score;
            score = alphaBeta(board, depth, alpha, beta);
            windowSize *= 2; 
            reSearchCount++;
        } else if (score >= beta) {
            alpha = score;
            beta = Evaluation::INFINITY_VALUE;
            score = alphaBeta(board, depth, alpha, beta);
            windowSize *= 2; 
            reSearchCount++;
        } else {
            break; 
        }
    }
    return score;
}