#pragma once

#include <iostream>
#include <string>
#include <vector>

enum PieceType {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
};

enum PieceColor {
    WHITE, BLACK, NONE
};

struct Piece {
    PieceType type;
    PieceColor color;

    Piece() : type(EMPTY), color(NONE) {}
    Piece(PieceType t, PieceColor c) : type(t), color(c) {}

    bool isEmpty() const { return type == EMPTY; }
};

class Board {
public:
    Board();
    Board(const std::string& fen);
    Board(const Board& other);
    Board& operator=(const Board& other);

    void initializeStandardSetup();
    void loadFen(const std::string& fen);
    std::string toFen() const;
    void printBoard() const;

    Piece getPieceAt(int row, int col) const;
    void setPieceAt(int row, int col, const Piece& piece);
    void movePiece(int startRow, int startCol, int endRow, int endCol);
    void clearBoard();

    PieceColor getTurn() const;
    void setTurn(PieceColor color);
    void switchTurn();

    bool isSquareAttacked(int row, int col, PieceColor attackerColor) const;
    bool isKingInCheck(PieceColor color) const;
    bool isMoveLegal(int startRow, int startCol, int endRow, int endCol) const;
    bool isCapture(int startRow, int startCol, int endRow, int endCol) const;
    bool isCastling(int startRow, int startCol, int endRow, int endCol) const;
    int getKingPosition(PieceColor color) const;

private:
    Piece board[8][8];
    PieceColor turn; 

    std::vector<std::pair<int, int>> getAttacksFromSquare(int row, int col) const;
};