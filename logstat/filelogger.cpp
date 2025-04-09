#include "filelogger.h"
#include <iostream>
#include "JWUtils/extstring.h"
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

    std::string part1(isoNow + " " + levelString);
    ExtString::fillUpRightSelf(part1, 29, ' ');

    std::string part2(file + ":" + ExtString::toString(line));
    ExtString::fillUpLeftSelf(part2, 40, ' ');
    part2 = ExtString::right(part2, 40);

    std::string m(part1 + " " + part2 + " " + message);
    fileLoggerStream << m << "\n";
    if (level <= LogStatController::info)
    {
        fileLoggerStream.flush();
    }
}

void FileLogger::theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType)
{
}
