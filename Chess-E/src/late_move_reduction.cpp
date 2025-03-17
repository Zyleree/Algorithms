#include "late_move_reduction.h"
#include <algorithm>
#include <limits>

LateMoveReduction::LateMoveReduction() : AlphaBetaSearch() {}

int LateMoveReduction::lateMoveReductionAlphaBeta(Board& board, int depth, int alpha, int beta, int moveCount) {
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
    int currentMoveCount = 0;

    for (const auto& move : moves) {
        currentMoveCount++;
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int reducedDepth = depth - 1;
        if (currentMoveCount > 2 && depth >= 3 && !isGoodMove(board, move)) {
            reducedDepth -= reduction(depth, currentMoveCount);
            reducedDepth = std::max(1, reducedDepth);
        }

        int currentValue = -lateMoveReductionAlphaBeta(nextBoard, reducedDepth, -beta, -alpha, currentMoveCount);
        if (reducedDepth < depth - 1 && currentValue > alpha && currentValue < beta) {
            currentValue = -lateMoveReductionAlphaBeta(nextBoard, depth - 1, -beta, -alpha, currentMoveCount);
        }

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


int LateMoveReduction::reduction(int depth, int moveCount) {
    return std::max(1, depth / 4 + moveCount / 5);
}


bool LateMoveReduction::isGoodMove(const Board& board, const Move& move) const {
    return board.getPieceAt(move.endRow, move.endCol).type != EMPTY || 
           board.isKingInCheck(board.getTurn()) || 
           getMoveScoreForLMR(board, move) > 0;
}

int LateMoveReduction::getMoveScoreForLMR(const Board& board, const Move& move) const {
    int score = 0;
    Piece capturedPiece = board.getPieceAt(move.endRow, move.endCol);
    if (!capturedPiece.isEmpty()) {
        score += Evaluation().pieceValue(capturedPiece.type) - 
                 Evaluation().pieceValue(board.getPieceAt(move.startRow, move.startCol).type)/2;
    }
    return score;
}