#include "endgame_tablebase.h"
#include "evaluation.h"
#include "movegen.h"
#include <iostream>
#include <algorithm>

EndgameTablebase::EndgameTablebase() {
}

int EndgameTablebase::probe(const Board& board) {
    if (isEndgamePosition(board)) {
        EndgamePattern pattern = analyzePosition(board);
        return evaluateEndgame(board, pattern);
    }
    return Evaluation::VALUE_UNKNOWN;
}

Move EndgameTablebase::getBestMove(const Board& board) {
    if (isEndgamePosition(board)) {
        EndgamePattern pattern = analyzePosition(board);
        return findBestEndgameMove(board, pattern);
    }
    return Move(-1, -1, -1, -1);
}

bool EndgameTablebase::isEndgamePosition(const Board& board) const {
    EndgamePattern pattern = analyzePosition(board);
    return pattern.isEndgame;
}

EndgameTablebase::EndgamePattern EndgameTablebase::analyzePosition(const Board& board) const {
    std::string fen = board.toFen();
    auto it = patternCache.find(fen);
    if (it != patternCache.end()) {
        return it->second;
    }

    EndgamePattern pattern = {};
    int totalPieces = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece piece = board.getPieceAt(r, c);
            if (!piece.isEmpty()) {
                totalPieces++;
                int pieceIndex = static_cast<int>(piece.type);
                if (piece.color == WHITE) {
                    pattern.whitePieces[pieceIndex]++;
                } else {
                    pattern.blackPieces[pieceIndex]++;
                }
            }
        }
    }

    pattern.isEndgame = totalPieces <= 12 || // Few pieces
                       (pattern.whitePieces[QUEEN] == 0 && pattern.blackPieces[QUEEN] == 0) || 
                       (totalPieces <= 16 && pattern.whitePieces[PAWN] + pattern.blackPieces[PAWN] <= 4);

    patternCache[fen] = pattern;
    return pattern;
}

int EndgameTablebase::evaluateEndgame(const Board& board, const EndgamePattern& pattern) const {
    if (!pattern.isEndgame) {
        return Evaluation::VALUE_UNKNOWN;
    }

    int whiteMaterial = 0, blackMaterial = 0;
    for (int i = 0; i < 6; ++i) {
        whiteMaterial += pattern.whitePieces[i] * Evaluation().pieceValue(static_cast<PieceType>(i));
        blackMaterial += pattern.blackPieces[i] * Evaluation().pieceValue(static_cast<PieceType>(i));
    }

    if (isKingAndPawnEndgame(pattern)) {

        int whiteKingDist = 0, blackKingDist = 0;

        return whiteMaterial - blackMaterial + (whiteKingDist - blackKingDist) * 10;
    }
    else if (isRookEndgame(pattern)) {

        return whiteMaterial - blackMaterial;
    }
    else if (isBishopEndgame(pattern)) {
        return whiteMaterial - blackMaterial;
    }

    return whiteMaterial - blackMaterial;
}

Move EndgameTablebase::findBestEndgameMove(const Board& board, const EndgamePattern& pattern) const {
    if (!pattern.isEndgame) {
        return Move(-1, -1, -1, -1);
    }

    if (isKingAndPawnEndgame(pattern)) {
        return findKingAndPawnEndgameMove(board);
    }
    else if (isRookEndgame(pattern)) {
        return findRookEndgameMove(board);
    }
    else if (isBishopEndgame(pattern)) {
        return findBishopEndgameMove(board);
    }
    else if (isKnightEndgame(pattern)) {
        return findKnightEndgameMove(board);
    }
    else if (isQueenEndgame(pattern)) {
        return findQueenEndgameMove(board);
    }

    return Move(-1, -1, -1, -1);
}

bool EndgameTablebase::isKingAndPawnEndgame(const EndgamePattern& pattern) const {
    int whiteNonPawns = 0, blackNonPawns = 0;
    for (int i = 1; i < 6; ++i) { // Skip PAWN
        whiteNonPawns += pattern.whitePieces[i];
        blackNonPawns += pattern.blackPieces[i];
    }
    return whiteNonPawns == 1 && blackNonPawns == 1; // Only kings
}

bool EndgameTablebase::isRookEndgame(const EndgamePattern& pattern) const {
    return pattern.whitePieces[ROOK] > 0 && pattern.blackPieces[ROOK] > 0 &&
           pattern.whitePieces[QUEEN] == 0 && pattern.blackPieces[QUEEN] == 0;
}

bool EndgameTablebase::isBishopEndgame(const EndgamePattern& pattern) const {
    return pattern.whitePieces[BISHOP] > 0 && pattern.blackPieces[BISHOP] > 0 &&
           pattern.whitePieces[QUEEN] == 0 && pattern.blackPieces[QUEEN] == 0 &&
           pattern.whitePieces[ROOK] == 0 && pattern.blackPieces[ROOK] == 0;
}

bool EndgameTablebase::isKnightEndgame(const EndgamePattern& pattern) const {
    return pattern.whitePieces[KNIGHT] > 0 && pattern.blackPieces[KNIGHT] > 0 &&
           pattern.whitePieces[QUEEN] == 0 && pattern.blackPieces[QUEEN] == 0 &&
           pattern.whitePieces[ROOK] == 0 && pattern.blackPieces[ROOK] == 0 &&
           pattern.whitePieces[BISHOP] == 0 && pattern.blackPieces[BISHOP] == 0;
}

bool EndgameTablebase::isQueenEndgame(const EndgamePattern& pattern) const {
    return pattern.whitePieces[QUEEN] > 0 && pattern.blackPieces[QUEEN] > 0;
}

Move EndgameTablebase::findKingAndPawnEndgameMove(const Board& board) const {
    std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
    if (legalMoves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == PAWN) {
            if ((piece.color == WHITE && move.endRow == 0) ||
                (piece.color == BLACK && move.endRow == 7)) {
                return move; 
            }
            if (abs(move.endRow - move.startRow) == 1 && move.startCol == move.endCol) {
                return move;
            }
        }
    }

    Move bestKingMove = Move(-1, -1, -1, -1);
    int bestScore = -Evaluation::INFINITY_VALUE;

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == KING) {
            int score = 0;
            score += (3 - abs(move.endCol - 3.5)) * 10;
            score += (3 - abs(move.endRow - 3.5)) * 10;
            
            if (score > bestScore) {
                bestScore = score;
                bestKingMove = move;
            }
        }
    }

    return bestKingMove.startRow != -1 ? bestKingMove : legalMoves[0];
}

Move EndgameTablebase::findRookEndgameMove(const Board& board) const {
    std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
    if (legalMoves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    Move bestRookMove = Move(-1, -1, -1, -1);
    int bestScore = -Evaluation::INFINITY_VALUE;

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == ROOK) {
            int score = 0;
            bool isOpenFile = true;
            for (int r = 0; r < 8; ++r) {
                if (r != move.endRow) {
                    Piece blockingPiece = board.getPieceAt(r, move.endCol);
                    if (!blockingPiece.isEmpty()) {
                        isOpenFile = false;
                        break;
                    }
                }
            }
            if (isOpenFile) score += 50;

            Piece targetPiece = board.getPieceAt(move.endRow, move.endCol);
            if (!targetPiece.isEmpty() && targetPiece.type == PAWN && targetPiece.color != piece.color) {
                score += 30;
            }

            for (int r = 0; r < 8; ++r) {
                if (r != move.endRow) {
                    Piece friendlyPiece = board.getPieceAt(r, move.endCol);
                    if (!friendlyPiece.isEmpty() && friendlyPiece.type == PAWN && friendlyPiece.color == piece.color) {
                        score += 20;
                    }
                }
            }

            if (score > bestScore) {
                bestScore = score;
                bestRookMove = move;
            }
        }
    }

    return bestRookMove.startRow != -1 ? bestRookMove : legalMoves[0];
}

Move EndgameTablebase::findBishopEndgameMove(const Board& board) const {
    std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
    if (legalMoves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    Move bestBishopMove = Move(-1, -1, -1, -1);
    int bestScore = -Evaluation::INFINITY_VALUE;

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == BISHOP) {
            int score = 0;
            if (move.endRow + move.endCol == 7 || move.endRow - move.endCol == 0) {
                score += 30;
            }

            Piece targetPiece = board.getPieceAt(move.endRow, move.endCol);
            if (!targetPiece.isEmpty() && targetPiece.type == PAWN && targetPiece.color != piece.color) {
                score += 30;
            }

            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if (r + c == move.endRow + move.endCol || r - c == move.endRow - move.endCol) {
                        Piece friendlyPiece = board.getPieceAt(r, c);
                        if (!friendlyPiece.isEmpty() && friendlyPiece.type == PAWN && friendlyPiece.color == piece.color) {
                            score += 20;
                        }
                    }
                }
            }

            if (score > bestScore) {
                bestScore = score;
                bestBishopMove = move;
            }
        }
    }

    return bestBishopMove.startRow != -1 ? bestBishopMove : legalMoves[0];
}

Move EndgameTablebase::findKnightEndgameMove(const Board& board) const {
    std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
    if (legalMoves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    Move bestKnightMove = Move(-1, -1, -1, -1);
    int bestScore = -Evaluation::INFINITY_VALUE;

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == KNIGHT) {
            int score = 0;
            if (move.endRow >= 2 && move.endRow <= 5 && move.endCol >= 2 && move.endCol <= 5) {
                score += 30;
            }

            Piece targetPiece = board.getPieceAt(move.endRow, move.endCol);
            if (!targetPiece.isEmpty() && targetPiece.type == PAWN && targetPiece.color != piece.color) {
                score += 30;
            }

            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if ((abs(r - move.endRow) == 2 && abs(c - move.endCol) == 1) ||
                        (abs(r - move.endRow) == 1 && abs(c - move.endCol) == 2)) {
                        Piece friendlyPiece = board.getPieceAt(r, c);
                        if (!friendlyPiece.isEmpty() && friendlyPiece.type == PAWN && friendlyPiece.color == piece.color) {
                            score += 20;
                        }
                    }
                }
            }

            if (score > bestScore) {
                bestScore = score;
                bestKnightMove = move;
            }
        }
    }

    return bestKnightMove.startRow != -1 ? bestKnightMove : legalMoves[0];
}

Move EndgameTablebase::findQueenEndgameMove(const Board& board) const {
    std::vector<Move> legalMoves = MoveGenerator().generateLegalMoves(board);
    if (legalMoves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    Move bestQueenMove = Move(-1, -1, -1, -1);
    int bestScore = -Evaluation::INFINITY_VALUE;

    for (const auto& move : legalMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == QUEEN) {
            int score = 0;

            if (move.endRow >= 2 && move.endRow <= 5 && move.endCol >= 2 && move.endCol <= 5) {
                score += 30;
            }

            Piece targetPiece = board.getPieceAt(move.endRow, move.endCol);
            if (!targetPiece.isEmpty() && targetPiece.color != piece.color) {
                score += 30;
            }

            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if (r == move.endRow || c == move.endCol ||
                        r + c == move.endRow + move.endCol || r - c == move.endRow - move.endCol) {
                        Piece friendlyPiece = board.getPieceAt(r, c);
                        if (!friendlyPiece.isEmpty() && friendlyPiece.color == piece.color) {
                            score += 20;
                        }
                    }
                }
            }

            if (score > bestScore) {
                bestScore = score;
                bestQueenMove = move;
            }
        }
    }

    return bestQueenMove.startRow != -1 ? bestQueenMove : legalMoves[0];
}