#include "time_management.h"

TimeManagement::TimeManagement() : baseTimeFraction(0.03), incrementFactor(1.0), aggressiveMode(false) {} 

std::chrono::milliseconds TimeManagement::calculateSearchTime(std::chrono::milliseconds allocatedTime) {
    double timeFraction = baseTimeFraction;
    if (aggressiveMode) timeFraction *= 1.5; // Spend more time in aggressive mode

    // Dynamic adjustment based on game phase (very basic example)
    // if (moveCount < 10) timeFraction *= 0.8; 
    // if (moveCount > 40) timeFraction *= 1.2; 

    return std::chrono::milliseconds(static_cast<int>(allocatedTime.count() * timeFraction * incrementFactor));
}

void TimeManagement::setAggressiveMode(bool aggressive) {
    aggressiveMode = aggressive;
}

void TimeManagement::setIncrementFactor(double factor) {
    incrementFactor = factor;
}