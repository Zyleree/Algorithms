
#pragma once

#include "board.h"
#include "move.h"
#include <vector>

class MoveGenerator {
public:
    std::vector<Move> generateLegalMoves(const Board& board) const;
    std::vector<Move> generatePseudoLegalMoves(const Board& board) const;

private:
    std::vector<Move> generatePawnMoves(const Board& board, int row, int col) const;
    std::vector<Move> generateKnightMoves(const Board& board, int row, int col) const;
    std::vector<Move> generateBishopMoves(const Board& board, int row, int col) const;
    std::vector<Move> generateRookMoves(const Board& board, int row, int col) const;
    std::vector<Move> generateQueenMoves(const Board& board, int row, int col) const;
    std::vector<Move> generateKingMoves(const Board& board, int row, int col) const;

    bool isMoveLegal(const Board& board, const Move& move) const;
};