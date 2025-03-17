#ifndef SEARCH_EXTENSIONS_H
#define SEARCH_EXTENSIONS_H

#include "board.h"
#include "move.h"

class SearchExtensions {
public:
    SearchExtensions();
    bool shouldExtendSearch(const Board& board, const Move& move, int depth) const;

private:
    bool isCheckExtension(const Board& board, const Move& move) const;
    bool isCaptureExtension(const Board& board, const Move& move) const;
    bool isPawnPromotionExtension(const Board& board, const Move& move) const;
    bool isPassedPawnPushExtension(const Board& board, const Move& move, int depth) const;
    bool isPassedPawn(const Board& board, int row, int col, PieceColor color) const;
};

#endif