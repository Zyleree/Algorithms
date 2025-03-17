#include "quiescence_search.h"
#include "evaluation.h"

QuiescenceSearch::QuiescenceSearch() {}

std::vector<Move> QuiescenceSearch::generateCaptureMoves(const Board& board) const {
    std::vector<Move> moves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board.getPieceAt(row, col);
            if (!piece.isEmpty() && piece.color == board.getTurn()) {
                for (int targetRow = 0; targetRow < 8; ++targetRow) {
                    for (int targetCol = 0; targetCol < 8; ++targetCol) {
                        if (board.isMoveLegal(row, col, targetRow, targetCol) && 
                            board.isCapture(row, col, targetRow, targetCol)) {
                            moves.emplace_back(row, col, targetRow, targetCol);
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int QuiescenceSearch::quiescence(Board& board, int alpha, int beta) {
    int standPat = Evaluation().evaluate(board);
    
    if (standPat >= beta) {
        return beta;
    }
    
    if (alpha < standPat) {
        alpha = standPat;
    }
    
    std::vector<Move> captures = generateCaptureMoves(board);
    for (const Move& move : captures) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();
        
        int score = -quiescence(nextBoard, -beta, -alpha);
        
        if (score >= beta) {
            return beta;
        }
        
        if (score > alpha) {
            alpha = score;
        }
    }
    
    return alpha;
}