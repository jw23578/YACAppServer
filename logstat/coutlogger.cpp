#include "coutlogger.h"
#include <iostream>
#include "utils/extstring.h"

coutLogger::coutLogger() {}

void coutLogger::theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message)
{
    std::string levelString;
    switch (level)
    {
    case LogStatController::never: break;
    case LogStatController::panic: levelString = "Panic! ";break;
    case LogStatController::error: levelString = "Error! ";break;
    case LogStatController::info: levelString = "Info!  ";break;
    case LogStatController::verbose: levelString = "Verbose ";break;
    }

    std::string isoNow(ExtString::timepointToISO(std::chrono::system_clock::now()));

    std::string m(isoNow + " " + levelString + " " + file + ":" + ExtString::toString(line) + " " + message);
    std::cout << m << std::endl;
}

void coutLogger::theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType)
{

}
