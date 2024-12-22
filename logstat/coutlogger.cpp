#include "coutlogger.h"
#include <iostream>
#include "utils/extstring.h"

int coutLogger::visibleLoggingActive(0);
bool coutLogger::definitelyNoVisibleLogging(false);


coutLogger::coutLogger() {}

void coutLogger::theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message)
{
    if (definitelyNoVisibleLogging)
    {
        return;
    }
    if (visibleLoggingActive <= 0 && level != LogStatController::panic && level != LogStatController::error)
    {
        return;
    }
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

    {
        std::string m(isoNow + " " + levelString + " " + file + ":" + ExtString::toString(line) + " " + message);
        if (m.size() <= 185)
        {
            std::cout << m << "\n" << std::flush;
            return;
        }
    }
    std::cout << isoNow + " " + levelString + " " + file + ":" + ExtString::toString(line) << "\n";
    std::string m(message);
    while (m.size())
    {
        size_t pos(185);
        while (pos > 0 && m[pos] != ' ')
        {
            --pos;
        }
        if (pos == 0)
        {
            pos = std::min(m.size(), (size_t)185);
        }
        std::cout << "   " << m.substr(0, pos + 1) << "\n";
        m.erase(0, pos + 1);
    }
    std::cout << "\n" << std::flush;
}

void coutLogger::theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType)
{

}
