#pragma once

#include "board.h"
#include <string>

struct Move {
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    PieceType promotionPiece;
    bool isCapture;
    bool isCastling;
    bool isEnPassant;
    
    Move(int sr = -1, int sc = -1, int er = -1, int ec = -1) 
        : startRow(sr), startCol(sc), endRow(er), endCol(ec),
          promotionPiece(EMPTY), isCapture(false), isCastling(false), isEnPassant(false) {}
    
    bool operator==(const Move& other) const {
        return startRow == other.startRow && startCol == other.startCol &&
               endRow == other.endRow && endCol == other.endCol;
    }
    
    bool isValid() const {
        return startRow >= 0 && startRow < 8 && startCol >= 0 && startCol < 8 &&
               endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8;
    }
    
    std::string toString() const {
        if (!isValid()) return "Invalid";
        
        std::string result;
        result += char('a' + startCol);
        result += char('8' - startRow);
        result += char('a' + endCol);
        result += char('8' - endRow);
        
        if (promotionPiece != EMPTY) {
            char promChar = 'q';
            switch (promotionPiece) {
                case KNIGHT: promChar = 'n'; break;
                case BISHOP: promChar = 'b'; break;
                case ROOK: promChar = 'r'; break;
                case QUEEN: promChar = 'q'; break;
                default: break;
            }
            result += promChar;
        }
        
        return result;
    }
}; 