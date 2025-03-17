#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "search.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>

class UCI {
public:
    UCI();
    void uciLoop();

private:
    Board currentBoard;
    Search searchEngine;
    TimeManagement timeManager;
    std::vector<Board> gameHistory;  

    void processCommand(const std::string& command);
    void handleUciCommand();
    void handleIsReadyCommand();
    void handleUciNewGameCommand();
    void handlePositionCommand(std::stringstream& ss);
    void handleGoCommand(std::stringstream& ss);
    void handleStopCommand();
    void handleQuitCommand();
    void handleSetOptionCommand(std::stringstream& ss);  
    void handleEvalCommand();  

    void sendUciResponse(const std::string& response);
    std::string moveToString(const Move& move) const;
    Move parseUCIMove(const std::string& moveStr) const;  
};

#endif 