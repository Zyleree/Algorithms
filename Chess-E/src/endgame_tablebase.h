#pragma once

#include "board.h"
#include "move.h"
#include "evaluation.h"
#include <unordered_map>
#include <vector>

class EndgameTablebase {
public:
    EndgameTablebase();
    int probe(const Board& board); 
    Move getBestMove(const Board& board); 

private:
    struct EndgamePattern {
        int whitePieces[6];  
        int blackPieces[6];
        bool isEndgame;
    };

    bool isEndgamePosition(const Board& board) const; 
    EndgamePattern analyzePosition(const Board& board) const;
    int evaluateEndgame(const Board& board, const EndgamePattern& pattern) const;
    Move findBestEndgameMove(const Board& board, const EndgamePattern& pattern) const;
    
    Move findKingAndPawnEndgameMove(const Board& board) const;
    Move findRookEndgameMove(const Board& board) const;
    Move findBishopEndgameMove(const Board& board) const;
    Move findKnightEndgameMove(const Board& board) const;
    Move findQueenEndgameMove(const Board& board) const;
    
    bool isKingAndPawnEndgame(const EndgamePattern& pattern) const;
    bool isRookEndgame(const EndgamePattern& pattern) const;
    bool isBishopEndgame(const EndgamePattern& pattern) const;
    bool isKnightEndgame(const EndgamePattern& pattern) const;
    bool isQueenEndgame(const EndgamePattern& pattern) const;
    
    mutable std::unordered_map<std::string, EndgamePattern> patternCache;
};