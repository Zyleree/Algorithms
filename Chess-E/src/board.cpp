#include "board.h"

Board::Board() : turn(WHITE) {
    clearBoard();
}

Board::Board(const std::string& fen) : turn(WHITE) {
    loadFen(fen);
}

Board::Board(const Board& other) : turn(other.turn) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = other.board[i][j];
        }
    }
}

Board& Board::operator=(const Board& other) {
    if (this == &other) {
        return *this;
    }
    turn = other.turn;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = other.board[i][j];
        }
    }
    return *this;
}


void Board::initializeStandardSetup() {
    clearBoard();

    setPieceAt(7, 0, Piece(ROOK, WHITE));
    setPieceAt(7, 1, Piece(KNIGHT, WHITE));
    setPieceAt(7, 2, Piece(BISHOP, WHITE));
    setPieceAt(7, 3, Piece(QUEEN, WHITE));
    setPieceAt(7, 4, Piece(KING, WHITE));
    setPieceAt(7, 5, Piece(BISHOP, WHITE));
    setPieceAt(7, 6, Piece(KNIGHT, WHITE));
    setPieceAt(7, 7, Piece(ROOK, WHITE));
    for (int j = 0; j < 8; ++j) setPieceAt(6, j, Piece(PAWN, WHITE));

    setPieceAt(0, 0, Piece(ROOK, BLACK));
    setPieceAt(0, 1, Piece(KNIGHT, BLACK));
    setPieceAt(0, 2, Piece(BISHOP, BLACK));
    setPieceAt(0, 3, Piece(QUEEN, BLACK));
    setPieceAt(0, 4, Piece(KING, BLACK));
    setPieceAt(0, 5, Piece(BISHOP, BLACK));
    setPieceAt(0, 6, Piece(KNIGHT, BLACK));
    setPieceAt(0, 7, Piece(ROOK, BLACK));
    for (int j = 0; j < 8; ++j) setPieceAt(1, j, Piece(PAWN, BLACK));

    turn = WHITE;
}

void Board::loadFen(const std::string& fen) {
    clearBoard();
    int row = 0, col = 0;
    int fenIndex = 0;

    while (row < 8) {
        char currentChar = fen[fenIndex++];
        if (isdigit(currentChar)) {
            int emptySquares = currentChar - '0';
            for (int i = 0; i < emptySquares; ++i) {
                setPieceAt(row, col++, Piece());
            }
        } else if (isalpha(currentChar)) {
            PieceColor color = isupper(currentChar) ? WHITE : BLACK;
            char pieceChar = tolower(currentChar);
            PieceType type;
            switch (pieceChar) {
                case 'p': type = PAWN;   break;
                case 'n': type = KNIGHT; break;
                case 'b': type = BISHOP; break;
                case 'r': type = ROOK;   break;
                case 'q': type = QUEEN;  break;
                case 'k': type = KING;   break;
                default:  type = EMPTY;  break; 
            }
            setPieceAt(row, col++, Piece(type, color));
        } else if (currentChar == '/') {
            row++;
            col = 0;
        } else if (currentChar == ' ') {
            break;
        }
    }

    if (fen[fenIndex] == 'w') {
        turn = WHITE;
    } else if (fen[fenIndex] == 'b') {
        turn = BLACK;
    } else {
        turn = WHITE; 
    }
}

std::string Board::toFen() const {
    std::string fen = "";
    for (int row = 0; row < 8; ++row) {
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col) {
            Piece piece = getPieceAt(row, col);
            if (piece.isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                char pieceChar;
                switch (piece.type) {
                    case PAWN:   pieceChar = 'p'; break;
                    case KNIGHT: pieceChar = 'n'; break;
                    case BISHOP: pieceChar = 'b'; break;
                    case ROOK:   pieceChar = 'r'; break;
                    case QUEEN:  pieceChar = 'q'; break;
                    case KING:   pieceChar = 'k'; break;
                    default:     pieceChar = ' '; break; 
                }
                fen += (piece.color == WHITE) ? toupper(pieceChar) : pieceChar;
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (row < 7) {
            fen += '/';
        }
    }
    fen += " ";
    fen += (turn == WHITE) ? 'w' : 'b';
    fen += " - - 0 1"; 
    return fen;
}


void Board::printBoard() const {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " |";
        for (int j = 0; j < 8; ++j) {
            Piece piece = getPieceAt(i, j);
            char pieceChar = '.';
            if (!piece.isEmpty()) {
                switch (piece.type) {
                    case PAWN:   pieceChar = 'p'; break;
                    case KNIGHT: pieceChar = 'n'; break;
                    case BISHOP: pieceChar = 'b'; break;
                    case ROOK:   pieceChar = 'r'; break;
                    case QUEEN:  pieceChar = 'q'; break;
                    case KING:   pieceChar = 'k'; break;
                    default:     pieceChar = '?'; break; 
                }
                pieceChar = (piece.color == WHITE) ? toupper(pieceChar) : pieceChar;
            }
            std::cout << pieceChar << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  -----------------" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "Turn: " << (turn == WHITE ? "White" : "Black") << std::endl;
}

Piece Board::getPieceAt(int row, int col) const {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return board[row][col];
    }
    return Piece(); 
}

void Board::setPieceAt(int row, int col, const Piece& piece) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        board[row][col] = piece;
    }
}

void Board::movePiece(int startRow, int startCol, int endRow, int endCol) {
    if (startRow >= 0 && startRow < 8 && startCol >= 0 && startCol < 8 &&
        endRow >= 0 && endRow < 8 && endCol >= 0 && endCol < 8) {
        Piece piece = getPieceAt(startRow, startCol);
        setPieceAt(startRow, startCol, Piece()); 
        setPieceAt(endRow, endCol, piece);       
    }
}

void Board::clearBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = Piece(); 
        }
    }
}

PieceColor Board::getTurn() const {
    return turn;
}

void Board::setTurn(PieceColor color) {
    turn = color;
}

void Board::switchTurn() {
    turn = (turn == WHITE) ? BLACK : WHITE;
}


bool Board::isSquareAttacked(int row, int col, PieceColor attackerColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece piece = getPieceAt(r, c);
            if (piece.color == attackerColor && !piece.isEmpty()) {
                std::vector<std::pair<int, int>> attacks = getAttacksFromSquare(r, c);
                for (const auto& attack : attacks) {
                    if (attack.first == row && attack.second == col) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isKingInCheck(PieceColor color) const {
    int kingPos = getKingPosition(color);
    if (kingPos == -1) return false; 
    PieceColor opponentColor = (color == WHITE) ? BLACK : WHITE;
    return isSquareAttacked(kingPos / 8, kingPos % 8, opponentColor);
}


int Board::getKingPosition(PieceColor color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece piece = getPieceAt(r, c);
            if (piece.type == KING && piece.color == color) {
                return r * 8 + c; 
            }
        }
    }
    return -1; 
}


std::vector<std::pair<int, int>> Board::getAttacksFromSquare(int row, int col) const {
    std::vector<std::pair<int, int>> attacks;
    Piece piece = getPieceAt(row, col);

    if (piece.isEmpty()) return attacks;

    PieceType type = piece.type;
    PieceColor color = piece.color;
    int r, c;

    switch (type) {
        case PAWN: {
            int direction = (color == WHITE) ? -1 : 1; 
            int attackRows[] = {row + direction};
            int attackCols[] = {col - 1, col + 1};

            for (int attackRow : attackRows) {
                for (int attackCol : attackCols) {
                    if (attackRow >= 0 && attackRow < 8 && attackCol >= 0 && attackCol < 8) {
                        attacks.push_back({attackRow, attackCol});
                    }
                }
            }
            break;
        }
        case KNIGHT: {
            int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
            for (const auto& move : knightMoves) {
                r = row + move[0];
                c = col + move[1];
                if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                    attacks.push_back({r, c});
                }
            }
            break;
        }
        case BISHOP:
        case ROOK:
        case QUEEN: {
            int directions[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; 
            int startDirIndex = (type == BISHOP) ? 4 : ((type == ROOK) ? 0 : 0); 
            int endDirIndex   = (type == BISHOP) ? 8 : ((type == ROOK) ? 4 : 8);

            for (int i = startDirIndex; i < endDirIndex; ++i) {
                int dr = directions[i][0];
                int dc = directions[i][1];
                r = row + dr;
                c = col + dc;
                while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                    attacks.push_back({r, c});
                    if (!getPieceAt(r, c).isEmpty()) { 
                        break;
                    }
                    r += dr;
                    c += dc;
                }
            }
            break;
        }
        case KING: {
            int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
            for (const auto& move : kingMoves) {
                r = row + move[0];
                c = col + move[1];
                if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                    attacks.push_back({r, c});
                }
            }
            break;
        }
        default:
            break;
    }
    return attacks;
}

bool Board::isMoveLegal(int startRow, int startCol, int endRow, int endCol) const {
    if (startRow < 0 || startRow >= 8 || startCol < 0 || startCol >= 8 ||
        endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8) {
        return false;
    }

    Piece piece = getPieceAt(startRow, startCol);
    if (piece.isEmpty() || piece.color != turn) {
        return false;
    }

    Piece destPiece = getPieceAt(endRow, endCol);
    if (!destPiece.isEmpty() && destPiece.color == piece.color) {
        return false;
    }

    std::vector<std::pair<int, int>> attacks = getAttacksFromSquare(startRow, startCol);
    
    bool moveFound = false;
    for (const auto& attack : attacks) {
        if (attack.first == endRow && attack.second == endCol) {
            moveFound = true;
            break;
        }
    }
    
    if (!moveFound) {
        return false;
    }

    Board testBoard = *this;
    testBoard.movePiece(startRow, startCol, endRow, endCol);
    if (testBoard.isKingInCheck(turn)) {
        return false;
    }

    return true;
}

bool Board::isCapture(int startRow, int startCol, int endRow, int endCol) const {
    if (startRow < 0 || startRow >= 8 || startCol < 0 || startCol >= 8 ||
        endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8) {
        return false;
    }

    Piece destPiece = getPieceAt(endRow, endCol);
    if (destPiece.isEmpty()) {
        return false;
    }

    Piece srcPiece = getPieceAt(startRow, startCol);
    return !srcPiece.isEmpty() && destPiece.color != srcPiece.color;
}

bool Board::isCastling(int startRow, int startCol, int endRow, int endCol) const {
    Piece piece = getPieceAt(startRow, startCol);
    
    if (piece.type != KING) {
        return false;
    }
    
    if (startRow != endRow || std::abs(startCol - endCol) != 2) {
        return false;
    }

    return true;
}