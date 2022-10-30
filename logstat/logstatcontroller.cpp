#include "logstatcontroller.h"
#include "loggerstatterinterface.h"

LogStatController::LogStatController(const LogLevel &ld,
                                     std::string const &sourceType,
                                     std::string const &sourceName):
    logDetail(ld),
    sourceType(sourceType),
    sourceName(sourceName)
{
}

void LogStatController::add(LoggerStatterInterface *ls)
{
    loggerStatter.push_back(ls);
}

void LogStatController::log(const std::string &file,
                 int line,
                 LogLevel level,
                 const std::string &message)
{
    if (logDetail < level)
    {
        return;
    }
    for (auto ls : loggerStatter)
    {
        ls->theLogFunction(file, line, level, message);
    }
}

void LogStatController::stat(LogStatController::StatType statType)
{
    for (auto ls : loggerStatter)
    {
        ls->theStatFunction(sourceType, sourceName, statType);
    }
}
