#include "board.h"
#include "endgame_tablebase.h"
#include "opening_book.h"
#include <iostream>
#include <iomanip>

void printBoard(const Board& board) {
    std::cout << "\n  a b c d e f g h\n";
    for (int r = 0; r < 8; ++r) {
        std::cout << 8 - r << " ";
        for (int c = 0; c < 8; ++c) {
            Piece piece = board.getPieceAt(r, c);
            char symbol = '.';
            if (!piece.isEmpty()) {
                switch (piece.type) {
                    case PAWN: symbol = piece.color == WHITE ? 'P' : 'p'; break;
                    case KNIGHT: symbol = piece.color == WHITE ? 'N' : 'n'; break;
                    case BISHOP: symbol = piece.color == WHITE ? 'B' : 'b'; break;
                    case ROOK: symbol = piece.color == WHITE ? 'R' : 'r'; break;
                    case QUEEN: symbol = piece.color == WHITE ? 'Q' : 'q'; break;
                    case KING: symbol = piece.color == WHITE ? 'K' : 'k'; break;
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << 8 - r << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

void testEndgameTablebase() {
    std::cout << "Testing Endgame Tablebase...\n\n";
    
    EndgameTablebase tb;
    
    std::cout << "Test 1: King and Pawn Endgame\n";
    Board kpBoard;
    kpBoard.setPieceAt(6, 4, Piece(PAWN, WHITE));  
    kpBoard.setPieceAt(7, 4, Piece(KING, WHITE)); 
    kpBoard.setPieceAt(0, 4, Piece(KING, BLACK)); 
    printBoard(kpBoard);
    
    int eval = tb.probe(kpBoard);
    std::cout << "Position evaluation: " << eval << "\n";
    
    Move bestMove = tb.getBestMove(kpBoard);
    if (bestMove.startRow != -1) {
        std::cout << "Best move: " << char('a' + bestMove.startCol) << 8 - bestMove.startRow
                  << char('a' + bestMove.endCol) << 8 - bestMove.endRow << "\n";
    }
    
    std::cout << "\nTest 2: Rook Endgame\n";
    Board rookBoard;
    rookBoard.setPieceAt(7, 0, Piece(ROOK, WHITE));  
    rookBoard.setPieceAt(7, 4, Piece(KING, WHITE));  
    rookBoard.setPieceAt(0, 0, Piece(ROOK, BLACK)); 
    rookBoard.setPieceAt(0, 4, Piece(KING, BLACK));  
    printBoard(rookBoard);
    
    eval = tb.probe(rookBoard);
    std::cout << "Position evaluation: " << eval << "\n";
    
    bestMove = tb.getBestMove(rookBoard);
    if (bestMove.startRow != -1) {
        std::cout << "Best move: " << char('a' + bestMove.startCol) << 8 - bestMove.startRow
                  << char('a' + bestMove.endCol) << 8 - bestMove.endRow << "\n";
    }
}

void testOpeningBook() {
    std::cout << "\nTesting Opening Book...\n\n";
    
    OpeningBook book;
    
    std::cout << "Test 1: Starting Position\n";
    Board startBoard;
    printBoard(startBoard);
    
    if (book.isInBook(startBoard)) {
        std::cout << "Position is in book\n";
        Move bookMove = book.probe(startBoard);
        if (bookMove.startRow != -1) {
            std::cout << "Book move: " << char('a' + bookMove.startCol) << 8 - bookMove.startRow
                      << char('a' + bookMove.endCol) << 8 - bookMove.endRow << "\n";
        }
        
        std::vector<Move> allMoves = book.getAllBookMoves(startBoard);
        std::cout << "All book moves: ";
        for (const auto& move : allMoves) {
            std::cout << char('a' + move.startCol) << 8 - move.startRow
                      << char('a' + move.endCol) << 8 - move.endRow << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Position is not in book\n";
    }
    
    std::cout << "\nTest 2: After e4\n";
    Board e4Board = startBoard;
    e4Board.movePiece(6, 4, 4, 4); 
    printBoard(e4Board);
    
    if (book.isInBook(e4Board)) {
        std::cout << "Position is in book\n";
        Move bookMove = book.probe(e4Board);
        if (bookMove.startRow != -1) {
            std::cout << "Book move: " << char('a' + bookMove.startCol) << 8 - bookMove.startRow
                      << char('a' + bookMove.endCol) << 8 - bookMove.endRow << "\n";
        }
    } else {
        std::cout << "Position is not in book\n";
    }
}

int main() {
    std::cout << "Chess Engine Test Program\n";
    std::cout << "========================\n\n";
    
    testEndgameTablebase();
    testOpeningBook();
    
    return 0;
} 