#pragma once
#include <time.h>

char* timestring() {
    time_t currentTime = time(0);
    tm* timeInfo;
    char* timeString = new char[30];
    timeInfo = localtime(&currentTime);
    strftime(timeString,29,"%F-%H:%M:%S",timeInfo);
    return timeString;
}