#ifndef LOGGERSTATTERINTERFACE_H
#define LOGGERSTATTERINTERFACE_H

#include <string>
#include "logstatcontroller.h"

class LoggerStatterInterface
{
    friend LogStatController;
public:
    LoggerStatterInterface();

protected:
    virtual void theLogFunction(std::string const &file,
                                int line,
                                LogStatController::LogLevel level,
                                std::string const &message) = 0;
    virtual void theStatFunction(std::string const &sourceType,
                                 std::string const &sourceName,
                                 LogStatController::StatType statType) = 0;
};

#endif // LOGGERSTATTERINTERFACE_H
