#include "opening_book.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <algorithm>

OpeningBook::OpeningBook() : rng(std::random_device()()) {
    loadBook();
}

Move OpeningBook::probe(const Board& board) const {
    std::string fen = normalizeFen(board.toFen());
    auto it = bookData.find(fen);
    if (it != bookData.end()) {
        return selectWeightedMove(it->second.moves, it->second.weights);
    }
    return Move(-1, -1, -1, -1);
}

bool OpeningBook::isInBook(const Board& board) const {
    return bookData.find(normalizeFen(board.toFen())) != bookData.end();
}

std::vector<Move> OpeningBook::getAllBookMoves(const Board& board) const {
    std::string fen = normalizeFen(board.toFen());
    auto it = bookData.find(fen);
    if (it != bookData.end()) {
        return it->second.moves;
    }
    return std::vector<Move>();
}

void OpeningBook::loadBook() {
    std::ifstream file("data/opening_book.txt");
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open opening book file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string fen, movesStr, weightsStr;
        
        // Read FEN
        std::getline(iss, fen, '|');
        fen = normalizeFen(fen);
        
        // Read moves
        std::getline(iss, movesStr, '|');
        std::vector<Move> moves;
        std::istringstream movesIss(movesStr);
        std::string moveStr;
        while (movesIss >> moveStr) {
            moves.push_back(parseMove(moveStr));
        }
        
        std::vector<int> weights;
        if (std::getline(iss, weightsStr)) {
            std::istringstream weightsIss(weightsStr);
            int weight;
            while (weightsIss >> weight) {
                weights.push_back(weight);
            }
        }
        
        if (weights.empty()) {
            weights.resize(moves.size(), 1);
        }
        
        if (weights.size() != moves.size()) {
            weights.resize(moves.size(), 1);
        }
        
        bookData[fen] = {moves, weights};
    }
}

Move OpeningBook::parseMove(const std::string& moveStr) const {
    if (moveStr.length() == 4) {
        int startCol = moveStr[0] - 'a';
        int startRow = 8 - (moveStr[1] - '0');
        int endCol = moveStr[2] - 'a';
        int endRow = 8 - (moveStr[3] - '0');
        return Move(startRow, startCol, endRow, endCol);
    }
    
    std::regex pieceRegex("^([NBRQK])?([a-h])([1-8])([x:])?([a-h])([1-8])$");
    std::smatch matches;
    if (std::regex_match(moveStr, matches, pieceRegex)) {
        int endCol = matches[5].str()[0] - 'a';
        int endRow = 8 - (matches[6].str()[0] - '0');

        return Move(-1, -1, endRow, endCol);
    }
    
    return Move(-1, -1, -1, -1);
}

std::string OpeningBook::normalizeFen(const std::string& fen) const {
    std::istringstream iss(fen);
    std::string normalized;
    std::string part;
    
    std::getline(iss, part, ' ');
    normalized += part;
    
    std::getline(iss, part, ' ');
    normalized += " " + part;
    
    std::getline(iss, part, ' ');
    normalized += " " + part;
    
    std::getline(iss, part, ' ');
    normalized += " " + part;
    
    return normalized;
}

bool OpeningBook::isBookMove(const Board& board, const Move& move) const {
    std::string fen = normalizeFen(board.toFen());
    auto it = bookData.find(fen);
    if (it != bookData.end()) {
        return std::find(it->second.moves.begin(), it->second.moves.end(), move) != it->second.moves.end();
    }
    return false;
}

Move OpeningBook::selectWeightedMove(const std::vector<Move>& moves, const std::vector<int>& weights) const {
    if (moves.empty()) {
        return Move(-1, -1, -1, -1);
    }
    
    int totalWeight = 0;
    for (int weight : weights) {
        totalWeight += weight;
    }
    
    std::uniform_int_distribution<int> dist(0, totalWeight - 1);
    int randomValue = dist(rng);
    
    int currentSum = 0;
    for (size_t i = 0; i < moves.size(); ++i) {
        currentSum += weights[i];
        if (randomValue < currentSum) {
            return moves[i];
        }
    }
    
    return moves.back();
}