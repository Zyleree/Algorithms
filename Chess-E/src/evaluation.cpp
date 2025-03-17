#include "evaluation.h"
#include "movegen.h"
#include <cmath>

Evaluation::Evaluation() {}

int Evaluation::evaluate(const Board& board) const {
    if (board.isKingInCheck(WHITE) && MoveGenerator().generateLegalMoves(board).empty()) return -CHECKMATE_VALUE;
    if (board.isKingInCheck(BLACK) && MoveGenerator().generateLegalMoves(board).empty()) return CHECKMATE_VALUE;
    if (MoveGenerator().generateLegalMoves(board).empty()) return 0;

    int score = 0;
    score += materialValue(board);
    score += positionValue(board);
    score += mobilityValue(board);
    score += pawnStructureValue(board);
    score += kingSafetyValue(board);

    return (board.getTurn() == WHITE) ? score : -score;
}

int Evaluation::pieceValue(PieceType type) const {
    switch (type) {
        case PAWN: return 100;
        case KNIGHT: return 320;
        case BISHOP: return 330;
        case ROOK: return 500;
        case QUEEN: return 900;
        case KING: return 20000;
        default: return 0;
    }
}

int Evaluation::pieceSquareBonus(PieceType type, PieceColor color, int row, int col, const Board& board) const {
    int bonus = 0;
    
    if (type == PAWN) {
        int centralBonus = abs(3.5 - col);
        bonus = (color == WHITE) ? (7 - row) * 5 : row * 5;
        bonus += (4 - centralBonus) * 2;
    }
    else if (type == KNIGHT) {
        int edgePenalty = (col == 0 || col == 7) ? -15 : 0;
        edgePenalty += (row == 0 || row == 7) ? -15 : 0;
        int centralBonus = (col >= 2 && col <= 5 && row >= 2 && row <= 5) ? 20 : 0;
        bonus = centralBonus + edgePenalty;
    }
    else if (type == BISHOP) {
        int diagonalBonus = (abs(row - col) == 0 || abs(row - col) == 7) ? 15 : 0;
        bonus = diagonalBonus;
    }
    else if (type == ROOK) {
        int seventhRank = (color == WHITE && row == 1) || (color == BLACK && row == 6) ? 30 : 0;
        bonus = seventhRank;
    }
    else if (type == KING) {
        bool endgame = isEndgame(board);
        if (endgame) {
            int centralBonus = 14 - (abs(3.5 - row) + abs(3.5 - col)) * 2;
            bonus = centralBonus;
        }
        else {
            int cornerBonus = (row >= 6 && (col <= 2 || col >= 5)) ? 20 : 0;
            cornerBonus = (color == BLACK) ? (row <= 1 && (col <= 2 || col >= 5)) ? 20 : 0 : cornerBonus;
            bonus = cornerBonus;
        }
    }
    
    return bonus;
}

int Evaluation::materialValue(const Board& board) const {
    int score = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board.getPieceAt(row, col);
            if (!piece.isEmpty()) {
                int value = pieceValue(piece.type);
                score += (piece.color == WHITE) ? value : -value;
            }
        }
    }
    return score;
}

int Evaluation::positionValue(const Board& board) const {
    int score = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board.getPieceAt(row, col);
            if (!piece.isEmpty()) {
                int bonus = pieceSquareBonus(piece.type, piece.color, row, col, board);
                score += (piece.color == WHITE) ? bonus : -bonus;
            }
        }
    }
    return score;
}

int Evaluation::mobilityValue(const Board& board) const {
    MoveGenerator moveGen;
    int whiteMobility = 0;
    int blackMobility = 0;
    
    Board whiteToMove = board;
    Board blackToMove = board;
    
    whiteToMove.setTurn(WHITE);
    blackToMove.setTurn(BLACK);
    
    std::vector<Move> whiteMoves = moveGen.generateLegalMoves(whiteToMove);
    std::vector<Move> blackMoves = moveGen.generateLegalMoves(blackToMove);
    
    whiteMobility = whiteMoves.size();
    blackMobility = blackMoves.size();
    
    for (const Move& move : whiteMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == BISHOP || piece.type == KNIGHT) {
            whiteMobility += 2;
        }
    }
    
    for (const Move& move : blackMoves) {
        Piece piece = board.getPieceAt(move.startRow, move.startCol);
        if (piece.type == BISHOP || piece.type == KNIGHT) {
            blackMobility += 2;
        }
    }
    
    return (whiteMobility - blackMobility) * 5;
}

int Evaluation::pawnStructureValue(const Board& board) const {
    int score = 0;
    
    bool whitePawns[8] = {false};
    bool blackPawns[8] = {false};
    
    int whiteDoubledPawns = 0;
    int blackDoubledPawns = 0;
    int whiteIsolatedPawns = 0;
    int blackIsolatedPawns = 0;
    int whitePassedPawns = 0;
    int blackPassedPawns = 0;
    
    for (int col = 0; col < 8; ++col) {
        int whitePawnsInCol = 0;
        int blackPawnsInCol = 0;
        
        for (int row = 0; row < 8; ++row) {
            Piece piece = board.getPieceAt(row, col);
            if (piece.type == PAWN) {
                if (piece.color == WHITE) {
                    whitePawns[col] = true;
                    whitePawnsInCol++;
                } else {
                    blackPawns[col] = true;
                    blackPawnsInCol++;
                }
            }
        }
        
        if (whitePawnsInCol > 1) whiteDoubledPawns += whitePawnsInCol - 1;
        if (blackPawnsInCol > 1) blackDoubledPawns += blackPawnsInCol - 1;
    }
    
    for (int col = 0; col < 8; ++col) {
        if (!whitePawns[col] && !blackPawns[col]) continue;
        
        bool whiteIsolated = whitePawns[col] && 
                            (col == 0 || !whitePawns[col-1]) && 
                            (col == 7 || !whitePawns[col+1]);
        bool blackIsolated = blackPawns[col] && 
                            (col == 0 || !blackPawns[col-1]) && 
                            (col == 7 || !blackPawns[col+1]);
        
        if (whiteIsolated) whiteIsolatedPawns++;
        if (blackIsolated) blackIsolatedPawns++;
        
        int mostAdvancedWhitePawn = 8;
        int mostAdvancedBlackPawn = -1;
        
        for (int row = 0; row < 8; ++row) {
            Piece piece = board.getPieceAt(row, col);
            if (piece.type == PAWN) {
                if (piece.color == WHITE && row < mostAdvancedWhitePawn) {
                    mostAdvancedWhitePawn = row;
                } else if (piece.color == BLACK && row > mostAdvancedBlackPawn) {
                    mostAdvancedBlackPawn = row;
                }
            }
        }
        
        if (mostAdvancedWhitePawn < 8) {
            bool passed = true;
            
            for (int c = col-1; c <= col+1; c++) {
                if (c < 0 || c > 7) continue;
                
                for (int r = 0; r < mostAdvancedWhitePawn; r++) {
                    Piece piece = board.getPieceAt(r, c);
                    if (piece.type == PAWN && piece.color == BLACK) {
                        passed = false;
                        break;
                    }
                }
                if (!passed) break;
            }
            
            if (passed) whitePassedPawns++;
        }
        
        if (mostAdvancedBlackPawn >= 0) {
            bool passed = true;
            
            for (int c = col-1; c <= col+1; c++) {
                if (c < 0 || c > 7) continue;
                
                for (int r = 7; r > mostAdvancedBlackPawn; r--) {
                    Piece piece = board.getPieceAt(r, c);
                    if (piece.type == PAWN && piece.color == WHITE) {
                        passed = false;
                        break;
                    }
                }
                if (!passed) break;
            }
            
            if (passed) blackPassedPawns++;
        }
    }
    
    score -= whiteDoubledPawns * 20;
    score += blackDoubledPawns * 20;
    score -= whiteIsolatedPawns * 15;
    score += blackIsolatedPawns * 15;
    score += whitePassedPawns * 30;
    score -= blackPassedPawns * 30;
    
    return score;
}

int Evaluation::kingSafetyValue(const Board& board) const {
    int score = 0;
    
    if (board.isKingInCheck(WHITE)) score -= 50;
    if (board.isKingInCheck(BLACK)) score += 50;
    
    int whiteKingPos = board.getKingPosition(WHITE);
    int blackKingPos = board.getKingPosition(BLACK);
    
    int whiteKingRow = whiteKingPos / 8;
    int whiteKingCol = whiteKingPos % 8;
    int blackKingRow = blackKingPos / 8;
    int blackKingCol = blackKingPos % 8;
    
    int whitePawnShield = 0;
    if (whiteKingRow == 7) {
        for (int c = std::max(0, whiteKingCol - 1); c <= std::min(7, whiteKingCol + 1); c++) {
            Piece piece = board.getPieceAt(6, c);
            if (piece.type == PAWN && piece.color == WHITE) {
                whitePawnShield++;
            }
        }
    }
    
    int blackPawnShield = 0;
    if (blackKingRow == 0) {
        for (int c = std::max(0, blackKingCol - 1); c <= std::min(7, blackKingCol + 1); c++) {
            Piece piece = board.getPieceAt(1, c);
            if (piece.type == PAWN && piece.color == BLACK) {
                blackPawnShield++;
            }
        }
    }
    
    score += whitePawnShield * 15;
    score -= blackPawnShield * 15;
    
    return score;
}

bool Evaluation::isEndgame(const Board& board) const {
    int pieceCount = 0;
    bool hasQueen = false;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board.getPieceAt(row, col);
            if (!piece.isEmpty() && piece.type != KING && piece.type != PAWN) {
                pieceCount++;
                if (piece.type == QUEEN) hasQueen = true;
            }
        }
    }
    
    return pieceCount <= 6 || !hasQueen;
}