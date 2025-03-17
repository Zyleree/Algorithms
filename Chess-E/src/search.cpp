#include "search.h"
#include <limits>
#include <algorithm>

Search::Search() :
    evaluationFunction(),
    moveGenerator(),
    transpositionTable(),
    quiescenceSearch(),
    iterativeDeepening(),
    alphaBetaSearch(),
    moveOrdering(),
    pvSearch(),
    nullMovePruning(),
    lateMoveReduction(),
    aspirationWindows(),
    openingBook(),
    endgameTablebase(),
    timeManagement(),
    searchExtensions(),
    postAnalysis(),
    threadPool(std::thread::hardware_concurrency()) 
{
    searchDepth = 6;
    searchTimeLimit = std::chrono::seconds(5);
}


Move Search::findBestMove(Board& board, int depth, std::chrono::milliseconds timeLimit) {
    searchDepth = depth;
    searchTimeLimit = timeLimit;

    Move bookMove = openingBook.probe(board);
    if (bookMove.startRow != -1) {
        return bookMove;
    }

    iterativeDeepening.startSearch(board, searchDepth, searchTimeLimit);
    Move bestMove = iterativeDeepening.getBestMove();

    postAnalysis.logSearchData(board, bestMove, iterativeDeepening.getSearchInfo());

    return bestMove;
}


int Search::alphaBeta(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return quiescence(board, alpha, beta);
    }

    int ttValue = transpositionTable.probe(board, depth, alpha, beta);
    if (ttValue != Evaluation::VALUE_UNKNOWN) {
        return ttValue;
    }

    std::vector<Move> moves = moveGenerator.generateLegalMoves(board);
    moveOrdering.orderMoves(board, moves);

    int bestValue = std::numeric_limits<int>::min();
    Move bestMove = Move(-1, -1, -1, -1);

    for (const auto& move : moves) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int extendedDepth = depth - 1;
        if (searchExtensions.shouldExtendSearch(nextBoard, move, depth)) {
            extendedDepth++;
        }

        int currentValue = -alphaBeta(nextBoard, extendedDepth, -beta, -alpha);
        if (currentValue > bestValue) {
            bestValue = currentValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
        if (beta <= alpha) {
            break;
        }
    }

    if (moves.empty()) {
        if (board.isKingInCheck(board.getTurn())) {
            return -Evaluation::CHECKMATE_VALUE - depth;
        } else {
            return 0; 
        }
    }

    transpositionTable.store(board, depth, bestValue, TranspositionTable::EXACT_VALUE, bestMove);

    return bestValue;
}


int Search::quiescence(Board& board, int alpha, int beta) {
    int staticEval = evaluationFunction.evaluate(board);

    if (staticEval >= beta) {
        return beta;
    }
    if (staticEval > alpha) {
        alpha = staticEval;
    }

    std::vector<Move> captureMoves = quiescenceSearch.generateCaptureMoves(board);
    moveOrdering.orderMoves(board, captureMoves);

    for (const auto& move : captureMoves) {
        Board nextBoard = board;
        nextBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
        nextBoard.switchTurn();

        int currentValue = -quiescence(nextBoard, -beta, -alpha);

        if (currentValue >= beta) {
            return beta;
        }
        if (currentValue > alpha) {
            alpha = currentValue;
        }
    }
    return alpha;
}

const TranspositionTable& Search::getTranspositionTable() const {
    return alphaBetaSearch.getTranspositionTable();
}