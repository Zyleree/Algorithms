#include "alphabeta.h"
#include <limits>
#include <algorithm>

AlphaBetaSearch::AlphaBetaSearch() :
    evaluationFunction(),
    moveGenerator(),
    transpositionTable(),
    quiescenceSearch(),
    moveOrdering()
{}


int AlphaBetaSearch::alphaBeta(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return quiescenceSearch.quiescence(board, alpha, beta);
    }

    int ttValue = transpositionTable.probe(board, depth, alpha, beta);
    if (ttValue != Evaluation::VALUE_UNKNOWN) {
        return ttValue;
    }

    std::vector<Move> moves = moveGenerator.generateLegalMoves(board);
    moveOrdering.orderMoves(board, moves);

    int bestValue = std::numeric_limits<int>::min();
    Move bestMove = Move(-1, -1, -1, -1);

    for (const auto& move : moves) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int currentValue = -alphaBeta(nextBoard, depth - 1, -beta, -alpha);
        if (currentValue > bestValue) {
            bestValue = currentValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
        if (beta <= alpha) {
            break;
        }
    }

    if (moves.empty()) {
        if (board.isKingInCheck(board.getTurn())) {
            return -Evaluation::CHECKMATE_VALUE - depth; 
        } else {
            return 0;
        }
    }

    transpositionTable.store(board, depth, bestValue, TranspositionTable::EXACT_VALUE, bestMove);

    return bestValue;
}

const TranspositionTable& AlphaBetaSearch::getTranspositionTable() const {
    return transpositionTable;
}