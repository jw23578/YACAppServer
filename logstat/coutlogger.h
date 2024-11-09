#ifndef COUTLOGGER_H
#define COUTLOGGER_H

#include "loggerstatterinterface.h"

class coutLogger : public LoggerStatterInterface
{
public:
    coutLogger();

protected:
    void theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message) override;
    void theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType) override;
};

#endif // COUTLOGGER_H
