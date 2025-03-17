#include "../src/board.h"
#include "../src/search.h"
#include "../src/quiescence_search.h"
#include "../src/principal_variation_search.h"
#include "../src/evaluation.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
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

bool setPositionFromFen(Board& board, const std::string& fen) {
    // Clear the board
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
                default: return false;  // Invalid character
            }
            
            board.setPieceAt(row, col, Piece(type, color));
            col++;
        }
    }
    
    return true;
}

std::string formatMove(const Move& move) {
    if (move.startRow == -1) return "none";
    std::string result;
    result += char('a' + move.startCol);
    result += char('8' - move.startRow);
    result += char('a' + move.endCol);
    result += char('8' - move.endRow);
    return result;
}


struct TacticalTest {
    std::string name;
    std::string fen;
    std::string expectedBestMove;  
    int depth;
};

void runTacticalTests() {
    std::cout << "Running Tactical Position Tests\n";
    std::cout << "==============================\n\n";

    Search searchEngine;
    QuiescenceSearch quiescenceSearch;
    PrincipalVariationSearch pvsSearch;
    AlphaBetaSearch abSearch;
    
    std::vector<TacticalTest> tests = {
        {
            "Simple Capture",
            "r1bqkbnr/ppp2ppp/2n5/3pp3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1",
            "e4d5",  // exd5
            4
        },
        {
            "Knight Fork",
            "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
            "f3e5",  // Nxe5
            4
        },
        {
            "Discovered Attack",
            "r1bqkb1r/pppp1ppp/2n2n2/8/4P3/2N5/PPPP1PPP/R1BQKBNR w KQkq - 0 1",
            "c3d5",  // Nd5
            4
        },
        {
            "Simple Mate in 1",
            "r1bqk2r/pppp1Qpp/2n2n2/2b5/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 1",
            "e8f7",  // Kxf7 
            2
        },
        {
            "Simple Mate in 2",
            "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1",
            "f3f7",  // Qxf7+
            4
        },
        {
            "Material Gain",
            "rnbqkbnr/pp3ppp/2p5/3pp3/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 0 1",
            "d4e5",  // dxe5
            4
        },
        {
            "Pin",
            "rnbqk2r/pppp1ppp/5n2/2b1p3/4P3/5N2/PPPPBPPP/RNBQK2R w KQkq - 0 1",
            "e2c4",  // Bc4 
            4
        }
    };
    
    int passedTests = 0;
    
    for (const auto& test : tests) {
        std::cout << "Test: " << test.name << "\n";
        std::cout << "FEN: " << test.fen << "\n";
        
        Board board;
        if (!setPositionFromFen(board, test.fen)) {
            std::cout << "Error setting up position from FEN\n";
            continue;
        }
        
        printBoard(board);
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        Move bestMove = searchEngine.findBestMove(board, test.depth, std::chrono::milliseconds(10000));
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::string bestMoveStr = formatMove(bestMove);
        
        std::cout << "Depth: " << test.depth << "\n";
        std::cout << "Best move found: " << bestMoveStr << "\n";
        std::cout << "Expected move: " << test.expectedBestMove << "\n";
        std::cout << "Search time: " << duration.count() << "ms\n";
        
        bool passed = (bestMoveStr == test.expectedBestMove);
        std::cout << "Test result: " << (passed ? "PASSED" : "FAILED") << "\n\n";
        
        if (passed) passedTests++;
    }
    
    std::cout << "Passed " << passedTests << " out of " << tests.size() << " tests\n\n";
}

void testQuiescenceSearch() {
    std::cout << "Testing Quiescence Search\n";
    std::cout << "========================\n\n";
    
    QuiescenceSearch qSearch;
    
    Board board;
    setPositionFromFen(board, "r2qk2r/ppp2ppp/2n5/2bpp3/4n3/2N2N2/PPPPQPPP/R1B1KB1R w KQkq - 0 1");
    
    printBoard(board);
    
    std::cout << "Running quiescence search...\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    
    int alpha = -999999;
    int beta = 999999;
    int score = qSearch.quiescence(board, alpha, beta);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "Quiescence score: " << score << "\n";
    std::cout << "Search time: " << duration.count() << "ms\n\n";
}

void benchmarkSearchAlgorithms() {
    std::cout << "Benchmarking Search Algorithms\n";
    std::cout << "============================\n\n";
    
    Board board;
    setPositionFromFen(board, "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
    
    printBoard(board);
    
    Search search1;
    Search search2;
    
    // Test depths
    std::vector<int> depths = {1, 2, 3, 4};
    
    std::cout << std::left << std::setw(15) << "Algorithm" 
              << std::setw(10) << "Depth" 
              << std::setw(15) << "Best Move" 
              << std::setw(15) << "Score" 
              << std::setw(15) << "Time (ms)" 
              << std::setw(15) << "Nodes" << "\n";
    std::cout << std::string(75, '-') << "\n";
    
    for (int depth : depths) {
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            Move bestMove = search1.findBestMove(board, depth, std::chrono::milliseconds(60000));
            Evaluation eval;
            board.movePiece(bestMove.startRow, bestMove.startCol, bestMove.endRow, bestMove.endCol);
            int score = eval.evaluate(board);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            std::cout << std::left << std::setw(15) << "Standard" 
                      << std::setw(10) << depth 
                      << std::setw(15) << formatMove(bestMove) 
                      << std::setw(15) << score 
                      << std::setw(15) << duration.count() 
                      << std::setw(15) << "N/A" << "\n";
            
            setPositionFromFen(board, "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
        }
        
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            Move bestMove = search2.findBestMove(board, depth, std::chrono::milliseconds(60000));
            Evaluation eval;
            board.movePiece(bestMove.startRow, bestMove.startCol, bestMove.endRow, bestMove.endCol);
            int score = eval.evaluate(board);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            std::cout << std::left << std::setw(15) << "With TT" 
                      << std::setw(10) << depth 
                      << std::setw(15) << formatMove(bestMove) 
                      << std::setw(15) << score 
                      << std::setw(15) << duration.count() 
                      << std::setw(15) << "N/A" << "\n";
            
            setPositionFromFen(board, "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
        }
        
        std::cout << std::string(75, '-') << "\n";
    }
}

int main() {
    std::cout << "Chess Engine Search Tests\n";
    std::cout << "========================\n\n";
    
    runTacticalTests();
    testQuiescenceSearch();
    benchmarkSearchAlgorithms();
    
    return 0;
} 