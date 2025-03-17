#include "move_ordering.h"
#include <algorithm>

MoveOrdering::MoveOrdering() {}

void MoveOrdering::orderMoves(const Board& board, std::vector<Move>& moves) {
    std::sort(moves.begin(), moves.end(), [&](const Move& a, const Move& b) {
        return getMoveScore(board, a) > getMoveScore(board, b);
    });
}

int MoveOrdering::getMoveScore(const Board& board, const Move& move) {
    int score = 0;

    Piece capturedPiece = board.getPieceAt(move.endRow, move.endCol);
    if (!capturedPiece.isEmpty()) {
        score += Evaluation().pieceValue(capturedPiece.type) - Evaluation().pieceValue(board.getPieceAt(move.startRow, move.startCol).type)/10; // MVV-LVA
    }

    if (board.isKingInCheck(board.getTurn())) score += 100; 

    if (board.getPieceAt(move.startRow, move.startCol).type == PAWN && (move.endRow == 0 || move.endRow == 7)) {
        score += 900; 
    }

    return score;
}