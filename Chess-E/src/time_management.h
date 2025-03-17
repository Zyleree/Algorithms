#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

#include <chrono>

class TimeManagement {
public:
    TimeManagement();
    std::chrono::milliseconds calculateSearchTime(std::chrono::milliseconds allocatedTime);
    void setAggressiveMode(bool aggressive);
    void setIncrementFactor(double factor);

private:
    double baseTimeFraction; 
    double incrementFactor;  
    bool aggressiveMode;     
};

#endif 