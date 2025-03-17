#ifndef PRINCIPAL_VARIATION_SEARCH_H
#define PRINCIPAL_VARIATION_SEARCH_H

#include "board.h"
#include "move.h"
#include "alphabeta.h"

class PrincipalVariationSearch : public AlphaBetaSearch {
public:
    PrincipalVariationSearch();
    int principalVariationSearch(Board& board, int depth, int alpha, int beta, bool isPVNode);
};

#endif 