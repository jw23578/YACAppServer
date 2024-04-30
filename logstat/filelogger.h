#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "loggerstatterinterface.h"
#include <fstream>

class FileLogger : public LoggerStatterInterface
{
    std::string const fileLoggerPath;
    std::string filename;
    size_t filenameCheckCounter;
    std::string generateFileName();
    std::ofstream fileLoggerStream;
    bool openStream();
public:
    FileLogger(std::string const &fileLoggerPath);

    void theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message) override;
    void theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType) override;
};

#endif // FILELOGGER_H
