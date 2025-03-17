#ifndef ASPIRATION_WINDOWS_H
#define ASPIRATION_WINDOWS_H

#include "board.h"
#include "alphabeta.h" 

class AspirationWindows : public AlphaBetaSearch {
public:
    AspirationWindows();
    int aspirationWindowSearch(Board& board, int depth, int previousScore);

private:
    int windowSize; 
};

#endif