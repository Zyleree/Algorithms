#pragma once

#include "board.h"

class Evaluation {
public:
    Evaluation();
    int evaluate(const Board& board) const;

    static const int CHECKMATE_VALUE = 10000;
    static const int INFINITY_VALUE = 100000;
    static const int VALUE_UNKNOWN = -INFINITY_VALUE;

    int pieceValue(PieceType type) const;
    int pieceSquareBonus(PieceType type, PieceColor color, int row, int col, const Board& board) const;
    int materialValue(const Board& board) const;
    int positionValue(const Board& board) const;
    int mobilityValue(const Board& board) const;
    int pawnStructureValue(const Board& board) const;
    int kingSafetyValue(const Board& board) const;
    bool isEndgame(const Board& board) const;
};