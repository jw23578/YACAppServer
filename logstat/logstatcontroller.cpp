#include "logstatcontroller.h"
#include "loggerstatterinterface.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"


LogStatController *LogStatController::instance(0);
LogStatController::LogStatController(const LogLevel &ld,
                                     std::string const &sourceType,
                                     std::string const &sourceName):
    logDetail(ld),
    sourceType(sourceType),
    sourceName(sourceName)
{
    if (!instance)
    {
        instance = this;
    }
}

LogStatController::~LogStatController()
{
    instance = 0;
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

void LogStatController::log(const std::string &file,
                            int line,
                            LogLevel level,
                            const rapidjson::Document &doc)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    buffer.Put('\n');
    log(file, line, level, buffer.GetString());
}

void LogStatController::stat(LogStatController::StatType statType)
{
    for (auto ls : loggerStatter)
    {
        ls->theStatFunction(sourceType, sourceName, statType);
    }
}

void LogStatController::slog(const std::string &file, int line, LogLevel level, const std::string &message)
{
    if (!instance)
    {
        return;
    }
    instance->log(file, line, level, message);
}
