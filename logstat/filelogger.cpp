#include "filelogger.h"
#include <iostream>

bool FileLogger::openStream()
{
    if (fileLoggerStream.good())
    {
        return true;
    }
    fileLoggerStream.open(fileLoggerPath + "/logfile.txt");
    return fileLoggerStream.good();
}

FileLogger::FileLogger(const std::string &fileLoggerPath):LoggerStatterInterface(),
    fileLoggerPath(fileLoggerPath)
{

}

void FileLogger::theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message)
{
    if (!openStream())
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

    std::string m(levelString + " " + file + ":" + ExtString::toString(line) + " " + message);
    fileLoggerStream << m << std::endl;
    if (m.size() <= 185)
    {
        std::cout << m << "\n" << std::endl;
        return;
    }
    std::cout << levelString + " " + file + ":" + ExtString::toString(line) << "\n";
    m = message;
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
    std::cout << std::endl;
}

void FileLogger::theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType)
{
}
