#pragma once

#include "board.h"
#include "move.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <random>

class OpeningBook {
public:
    OpeningBook();
    Move probe(const Board& board) const;
    void loadBook();
    bool isInBook(const Board& board) const;
    std::vector<Move> getAllBookMoves(const Board& board) const;

private:
    struct BookEntry {
        std::vector<Move> moves;
        std::vector<int> weights;  
        int totalWeight;
    };

    std::unordered_map<std::string, BookEntry> bookData; // FEN -> BookEntry mapping
    mutable std::mt19937 rng; 

    Move parseMove(const std::string& moveStr) const;
    std::string normalizeFen(const std::string& fen) const;
    bool isBookMove(const Board& board, const Move& move) const;
    Move selectWeightedMove(const std::vector<Move>& moves, const std::vector<int>& weights) const;
};