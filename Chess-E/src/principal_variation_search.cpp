#include "principal_variation_search.h"
#include <limits>
#include <algorithm>

PrincipalVariationSearch::PrincipalVariationSearch() : AlphaBetaSearch() {}


int PrincipalVariationSearch::principalVariationSearch(Board& board, int depth, int alpha, int beta, bool isPVNode) {
    if (depth == 0) {
        return quiescenceSearch.quiescence(board, alpha, beta);
    }

    int ttValue = transpositionTable.probe(board, depth, alpha, beta);
    if (ttValue != Evaluation::VALUE_UNKNOWN && !isPVNode) {
        return ttValue;
    }

    std::vector<Move> moves = moveGenerator.generateLegalMoves(board);
    moveOrdering.orderMoves(board, moves);

    int bestValue = std::numeric_limits<int>::min();
    Move bestMove = Move(-1, -1, -1, -1);
    bool firstMove = true;

    for (const auto& move : moves) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int currentValue;
        if (firstMove || !isPVNode) {
            currentValue = -principalVariationSearch(nextBoard, depth - 1, -beta, -alpha, !firstMove);
        } else {
            currentValue = -principalVariationSearch(nextBoard, depth - 1, -alpha - 1, -alpha, false);
            if (currentValue > alpha && currentValue < beta) {
                currentValue = -principalVariationSearch(nextBoard, depth - 1, -beta, -alpha, true);
            }
        }

        if (currentValue > bestValue) {
            bestValue = currentValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
        if (beta <= alpha) {
            break;
        }
        firstMove = false;
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