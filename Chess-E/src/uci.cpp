#include "uci.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <vector>

UCI::UCI() : currentBoard(), searchEngine(), timeManager() {
    std::cout << "Advanced Chess Engine UCI" << std::endl;
}

void UCI::uciLoop() {
    std::string command;
    while (true) {
        std::getline(std::cin, command);
        processCommand(command);
        if (command == "quit") break;
    }
}

void UCI::processCommand(const std::string& command) {
    std::stringstream ss(command);
    std::string token;
    ss >> token;

    if (token == "uci") {
        handleUciCommand();
    } else if (token == "isready") {
        handleIsReadyCommand();
    } else if (token == "ucinewgame") {
        handleUciNewGameCommand();
    } else if (token == "position") {
        handlePositionCommand(ss);
    } else if (token == "go") {
        handleGoCommand(ss);
    } else if (token == "stop") {
        handleStopCommand();
    } else if (token == "quit") {
        handleQuitCommand();
    } else if (token == "setoption") {
        handleSetOptionCommand(ss);
    } else if (token == "eval") { 
        handleEvalCommand();
    }
}


void UCI::handleUciCommand() {
    sendUciResponse("id name Chess-E");
    sendUciResponse("id author Zyleree");
    sendUciResponse("option name AggressiveMode type check default false"); 
    sendUciResponse("uciok");
}

void UCI::handleIsReadyCommand() {
    sendUciResponse("readyok");
}

void UCI::handleUciNewGameCommand() {
    currentBoard.initializeStandardSetup();
    gameHistory.clear(); 
}

void UCI::handlePositionCommand(std::stringstream& ss) {
    std::string token;
    ss >> token;
    if (token == "fen") {
        std::string fenStr;
        std::getline(ss >> std::ws, fenStr);
        currentBoard.loadFen(fenStr);
    } else if (token == "startpos") {
        currentBoard.initializeStandardSetup();
    }
    gameHistory.clear(); 

    ss >> token;
    if (token == "moves") {
        while (ss >> token) {
            Move move = parseUCIMove(token);
            if (move.startRow != -1) {
                gameHistory.push_back(currentBoard);
                currentBoard.movePiece(move.startRow, move.startCol, move.endRow, move.endCol);
                currentBoard.switchTurn();
            }
        }
    }
}

void UCI::handleGoCommand(std::stringstream& ss) {
    int depth = 6;
    std::chrono::milliseconds timeLimit = std::chrono::seconds(10);
    std::string token;
    int wtime = -1, btime = -1, winc = 0, binc = 0; 

    while (ss >> token) {
        if (token == "depth") {
            ss >> depth;
        } else if (token == "movetime") {
            int moveTimeMs;
            ss >> moveTimeMs;
            timeLimit = std::chrono::milliseconds(moveTimeMs);
        } else if (token == "wtime") {
            ss >> wtime;
        } else if (token == "btime") {
            ss >> btime;
        } else if (token == "winc") {
            ss >> winc;
        } else if (token == "binc") {
            ss >> binc;
        }
    }

    if (currentBoard.getTurn() == WHITE && wtime != -1) {
        timeLimit = timeManager.calculateSearchTime(std::chrono::milliseconds(wtime + winc)); 
    } else if (currentBoard.getTurn() == BLACK && btime != -1) {
        timeLimit = timeManager.calculateSearchTime(std::chrono::milliseconds(btime + binc));
    }

    Move bestMove = searchEngine.findBestMove(currentBoard, depth, timeLimit);
    sendUciResponse("bestmove " + moveToString(bestMove));
    gameHistory.push_back(currentBoard);
    currentBoard.movePiece(bestMove.startRow, bestMove.startCol, bestMove.endRow, bestMove.endCol);
    currentBoard.switchTurn();
}

void UCI::handleStopCommand() {
}

void UCI::handleQuitCommand() {
}

void UCI::handleSetOptionCommand(std::stringstream& ss) {
    std::string token, name, valueStr;
    ss >> token; 
    while (ss >> token && token != "value") {
        name += token + " ";
    }
    name.pop_back(); 
    ss >> token >> valueStr; 


    if (name == "AggressiveMode") {
        bool value = (valueStr == "true");
        timeManager.setAggressiveMode(value);
        std::cout << "Set option AggressiveMode to " << value << std::endl;
    } 
}

void UCI::handleEvalCommand() {
    int evalScore = Evaluation().evaluate(currentBoard);
    sendUciResponse("info string Evaluation: " + std::to_string(evalScore)); 
}


void UCI::sendUciResponse(const std::string& response) {
    std::cout << response << std::endl;
}

std::string UCI::moveToString(const Move& move) const {
    if (move.startRow == -1) return "nomove";
    char startFile = 'a' + move.startCol;
    int startRank = 8 - move.startRow;
    char endFile = 'a' + move.endCol;
    int endRank = 8 - move.endRow;
    std::stringstream ss;
    ss << startFile << startRank << endFile << endRank;
    return ss.str();
}

Move UCI::parseUCIMove(const std::string& moveStr) const {
    if (moveStr.length() == 4 || moveStr.length() == 5) {
        int startCol = moveStr[0] - 'a';
        int startRow = 8 - (moveStr[1] - '0');
        int endCol = moveStr[2] - 'a';
        int endRow = 8 - (moveStr[3] - '0');
        return Move(startRow, startCol, endRow, endCol);
    }
    return Move(-1, -1, -1, -1);
}