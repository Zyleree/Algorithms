#include "search_extensions.h"

SearchExtensions::SearchExtensions() {}

bool SearchExtensions::shouldExtendSearch(const Board& board, const Move& move, int depth) const {
    if (depth <= 1) return false;

    if (isCheckExtension(board, move)) return true;
    if (isCaptureExtension(board, move)) return true;
    if (isPawnPromotionExtension(board, move)) return true;
    if (isPassedPawnPushExtension(board, move, depth)) return true; 

    return false;
}

bool SearchExtensions::isCheckExtension(const Board& board, const Move& move) const {
    Board nextBoard = board;
    nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
    nextBoard.switchTurn();
    return nextBoard.isKingInCheck(nextBoard.getTurn());
}

bool SearchExtensions::isCaptureExtension(const Board& board, const Move& move) const {
    return !board.getPieceAt(move.endRow, move.endCol).isEmpty();
}

bool SearchExtensions::isPawnPromotionExtension(const Board& board, const Move& move) const {
    return board.getPieceAt(move.startRow, move.startCol).type == PAWN && (move.endRow == 0 || move.endRow == 7);
}

bool SearchExtensions::isPassedPawnPushExtension(const Board& board, const Move& move, int depth) const {
    if (board.getPieceAt(move.startRow, move.startCol).type == PAWN) {
        if (isPassedPawn(board, move.startRow, move.startCol, board.getPieceAt(move.startRow, move.startCol).color)) {
            return true; 
        }
    }
    return false;
}


bool SearchExtensions::isPassedPawn(const Board& board, int row, int col, PieceColor color) const {
    int direction = (color == WHITE) ? -1 : 1;
    for (int r = row + direction; (color == WHITE ? r >= 0 : r < 8); r += direction) {
        for (int c = std::max(0, col - 1); c <= std::min(7, col + 1); ++c) {
            Piece piece = board.getPieceAt(r, c);
            if (piece.type == PAWN && piece.color != color) return false; 
        }
    }
    return true; 
}