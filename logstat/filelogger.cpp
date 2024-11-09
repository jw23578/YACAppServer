#include "filelogger.h"
#include <iostream>
#include "utils/extstring.h"
#include <unistd.h>

std::string FileLogger::generateFileName()
{
    std::string fn(fileLoggerPath + "/yac-server-");
    fn += ExtString::timepointToISODate(std::chrono::system_clock::now());
    fn += "-";
    fn += ExtString::toString(getpid());
    fn += ".txt";
    return fn;
}

bool FileLogger::openStream()
{
    ++filenameCheckCounter;
    if (fileLoggerStream.good())
    {
        if (filenameCheckCounter < 1000 || filename == generateFileName())
        {
            return true;
        }
        filenameCheckCounter = 0;
        fileLoggerStream.close();
    }
    filename = generateFileName();
    fileLoggerStream.open(filename);
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

    std::string isoNow(ExtString::timepointToISO(std::chrono::system_clock::now()));

    std::string m(isoNow + " " + levelString + " " + file + ":" + ExtString::toString(line) + " " + message);
    fileLoggerStream << m << "\n";
    if (m.size() <= 185)
    {
        std::cout << m << "\n"; // << std::endl;
        return;
    }
    std::cout << isoNow + " " + levelString + " " + file + ":" + ExtString::toString(line) << "\n";
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
    std::cout << "\n";
}

void FileLogger::theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType)
{
}
