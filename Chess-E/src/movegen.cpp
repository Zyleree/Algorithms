#include "movegen.h"

std::vector<Move> MoveGenerator::generateLegalMoves(const Board& board) const {
    std::vector<Move> pseudoLegalMoves = generatePseudoLegalMoves(board);
    std::vector<Move> legalMoves;

    for (const auto& move : pseudoLegalMoves) {
        if (isMoveLegal(board, move)) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;
    PieceColor turn = board.getTurn();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board.getPieceAt(row, col);
            if (piece.color == turn) {
                std::vector<Move> pieceMoves;
                switch (piece.type) {
                    case PAWN:   pieceMoves = generatePawnMoves(board, row, col);   break;
                    case KNIGHT: pieceMoves = generateKnightMoves(board, row, col); break;
                    case BISHOP: pieceMoves = generateBishopMoves(board, row, col); break;
                    case ROOK:   pieceMoves = generateRookMoves(board, row, col);   break;
                    case QUEEN:  pieceMoves = generateQueenMoves(board, row, col);  break;
                    case KING:  pieceMoves = generateKingMoves(board, row, col);   break;
                    default: break;
                }
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return moves;
}


std::vector<Move> MoveGenerator::generatePawnMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    PieceColor color = board.getPieceAt(row, col).color;
    int direction = (color == WHITE) ? -1 : 1; 
    int startRow = (color == WHITE) ? 6 : 1;    

    int advanceRow = row + direction;
    if (advanceRow >= 0 && advanceRow < 8 && board.getPieceAt(advanceRow, col).isEmpty()) {
        moves.push_back(Move(row, col, advanceRow, col));
        if (row == startRow && board.getPieceAt(row + 2 * direction, col).isEmpty()) {
            moves.push_back(Move(row, col, row + 2 * direction, col));
        }
    }

    int captureCols[] = {col - 1, col + 1};
    for (int captureCol : captureCols) {
        if (captureCol >= 0 && captureCol < 8) {
            Piece targetPiece = board.getPieceAt(advanceRow, captureCol);
            if (!targetPiece.isEmpty() && targetPiece.color != color) {
                moves.push_back(Move(row, col, advanceRow, captureCol));
            }
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    PieceColor color = board.getPieceAt(row, col).color;
    int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    for (const auto& move : knightMoves) {
        int endRow = row + move[0];
        int endCol = col + move[1];
        if (endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8) {
            Piece targetPiece = board.getPieceAt(endRow, endCol);
            if (targetPiece.isEmpty() || targetPiece.color != color) {
                moves.push_back(Move(row, col, endRow, endCol));
            }
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    PieceColor color = board.getPieceAt(row, col).color;
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; 

    for (const auto& dir : directions) {
        int dr = dir[0];
        int dc = dir[1];
        int endRow = row + dr;
        int endCol = col + dc;
        while (endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8) {
            Piece targetPiece = board.getPieceAt(endRow, endCol);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(row, col, endRow, endCol));
            } else {
                if (targetPiece.color != color) {
                    moves.push_back(Move(row, col, endRow, endCol)); 
                }
                break; 
            }
            endRow += dr;
            endCol += dc;
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    PieceColor color = board.getPieceAt(row, col).color;
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; 

    for (const auto& dir : directions) {
        int dr = dir[0];
        int dc = dir[1];
        int endRow = row + dr;
        int endCol = col + dc;
        while (endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8) {
            Piece targetPiece = board.getPieceAt(endRow, endCol);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(row, col, endRow, endCol));
            } else {
                if (targetPiece.color != color) {
                    moves.push_back(Move(row, col, endRow, endCol)); 
                }
                break; 
            }
            endRow += dr;
            endCol += dc;
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    std::vector<Move> bishopMoves = generateBishopMoves(board, row, col);
    std::vector<Move> rookMoves = generateRookMoves(board, row, col);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(const Board& board, int row, int col) const {
    std::vector<Move> moves;
    PieceColor color = board.getPieceAt(row, col).color;
    int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (const auto& move : kingMoves) {
        int endRow = row + move[0];
        int endCol = col + move[1];
        if (endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8) {
            Piece targetPiece = board.getPieceAt(endRow, endCol);
            if (targetPiece.isEmpty() || targetPiece.color != color) {
                moves.push_back(Move(row, col, endRow, endCol));
            }
        }
    }
    return moves;
}


bool MoveGenerator::isMoveLegal(const Board& board, const Move& move) const {
    Board testBoard = board;
    Piece movingPiece = testBoard.getPieceAt(move.startRow, move.startCol);
    testBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
    testBoard.setTurn((testBoard.getTurn() == WHITE) ? BLACK : WHITE); 

    return !testBoard.isKingInCheck(movingPiece.color);
}