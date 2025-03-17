#include "null_move_pruning.h"
#include <limits>
#include <algorithm>

NullMovePruning::NullMovePruning() : AlphaBetaSearch(), R(2) {}


int NullMovePruning::nullMoveAlphaBeta(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return quiescenceSearch.quiescence(board, alpha, beta);
    }

    int ttValue = transpositionTable.probe(board, depth, alpha, beta);
    if (ttValue != Evaluation::VALUE_UNKNOWN) {
        return ttValue;
    }

    if (!board.isKingInCheck(board.getTurn()) && hasNonPawnMaterial(board) && depth >= 3) { 
        Board nullMoveBoard = board;
        nullMoveBoard.switchTurn();
        int nullMoveValue = -nullMoveAlphaBeta(nullMoveBoard, depth - 1 - R, -beta, -beta + 1);

        if (nullMoveValue >= beta) {
            return beta;
        }
    }


    std::vector<Move> moves = moveGenerator.generateLegalMoves(board);
    moveOrdering.orderMoves(board, moves);

    int bestValue = std::numeric_limits<int>::min();
    Move bestMove = Move(-1, -1, -1, -1);

    for (const auto& move : moves) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int currentValue = -nullMoveAlphaBeta(nextBoard, depth - 1, -beta, -alpha);
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


bool NullMovePruning::hasNonPawnMaterial(const Board& board) const {
    PieceColor turn = board.getTurn();
    int nonPawnCount = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece piece = board.getPieceAt(r, c);
            if (piece.color == turn && piece.type != PAWN && piece.type != KING && !piece.isEmpty()) {
                nonPawnCount++;
            }
        }
    }
    return nonPawnCount >= 2; 
}