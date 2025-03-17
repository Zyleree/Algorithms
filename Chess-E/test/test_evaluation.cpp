#include "../src/board.h"
#include "../src/evaluation.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// Helper function to print board
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

// Set up a position from FEN string
bool setPositionFromFen(Board& board, const std::string& fen) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board.setPieceAt(r, c, Piece());
        }
    }
    
    int row = 0;
    int col = 0;
    
    for (char c : fen) {
        if (c == ' ') break;  
        
        if (c == '/') {
            row++;
            col = 0;
        } else if (c >= '1' && c <= '8') {
            col += (c - '0');
        } else {
            PieceType type;
            PieceColor color;
            
            switch (c) {
                case 'P': type = PAWN; color = WHITE; break;
                case 'N': type = KNIGHT; color = WHITE; break;
                case 'B': type = BISHOP; color = WHITE; break;
                case 'R': type = ROOK; color = WHITE; break;
                case 'Q': type = QUEEN; color = WHITE; break;
                case 'K': type = KING; color = WHITE; break;
                case 'p': type = PAWN; color = BLACK; break;
                case 'n': type = KNIGHT; color = BLACK; break;
                case 'b': type = BISHOP; color = BLACK; break;
                case 'r': type = ROOK; color = BLACK; break;
                case 'q': type = QUEEN; color = BLACK; break;
                case 'k': type = KING; color = BLACK; break;
                default: return false; 
            }
            
            board.setPieceAt(row, col, Piece(type, color));
            col++;
        }
    }
    
    return true;
}

struct TestPosition {
    std::string name;
    std::string fen;
    int expectedEvalRange[2];  
};

void runEvaluationTests() {
    Evaluation evaluator;
    
    std::vector<TestPosition> positions = {
        {
            "Starting Position",
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            {-10, 10}  
        },
        {
            "Material Advantage for White",
            "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPPQPPP/RNB1KBNR b KQkq - 0 1",
            {800, 1000} 
        },
        {
            "Material Advantage for Black",
            "rnb1kbnr/ppppqppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1",
            {-1000, -800} 
        },
        {
            "Positional Advantage",
            "r1bqk2r/ppp2ppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQK2R w KQkq - 0 1",
            {-50, 50}  
        },
        {
            "Open File Control",
            "r3k2r/pppq1ppp/2n1bn2/3pp3/3PP3/2N1BN2/PPPQ1PPP/R3K2R w KQkq - 0 1",
            {-30, 30}  
        },
        {
            "Pawn Structure Imbalance",
            "r1bqkbnr/pp2pppp/2n5/2pp4/3PP3/2N5/PPP2PPP/R1BQKBNR w KQkq - 0 1",
            {-30, 30} 
        },
        {
            "Late Middlegame Position",
            "r4rk1/pp2ppbp/2n3p1/q1pp4/3P4/P1P1PN2/1P2BPPP/R2Q1RK1 w - - 0 1",
            {-50, 50} 
        },
        {
            "Endgame Position",
            "4k3/pp3pp1/2p5/4P3/2P5/PP3PP1/8/4K3 w - - 0 1",
            {0, 100}  
        }
    };
    
    int passedTests = 0;
    
    std::cout << "Running Evaluation Function Tests\n";
    std::cout << "================================\n\n";
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for (const auto& pos : positions) {
        std::cout << "Test: " << pos.name << "\n";
        
        Board board;
        if (!setPositionFromFen(board, pos.fen)) {
            std::cout << "Error setting up position from FEN\n";
            continue;
        }
        
        printBoard(board);
        
        int evaluation = evaluator.evaluate(board);
        std::cout << "Evaluation: " << evaluation << "\n";
        
        bool passed = (evaluation >= pos.expectedEvalRange[0] && 
                       evaluation <= pos.expectedEvalRange[1]);
        
        std::cout << "Expected range: [" << pos.expectedEvalRange[0] << ", " 
                  << pos.expectedEvalRange[1] << "]\n";
        std::cout << "Test result: " << (passed ? "PASSED" : "FAILED") << "\n\n";
        
        if (passed) passedTests++;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "Tests completed in " << duration.count() << "ms\n";
    std::cout << "Passed " << passedTests << " out of " << positions.size() << " tests\n";
}

int main() {
    runEvaluationTests();
    return 0;
} 