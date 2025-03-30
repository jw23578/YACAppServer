#include "logstatcontroller.h"
#include "loggerstatterinterface.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "utils/extvector.h"


LogStatController *LogStatController::instance(0);
void LogStatController::pushScope(const std::string &scope)
{
    if (!instance)
    {
        return;
    }
    instance->scopes.push_back(scope);
}

void LogStatController::popScope(const std::string &scope)
{
    if (!instance)
    {
        return;
    }
    std::vector<std::string> &scopes(instance->scopes);
    if (!scopes.size())
    {
        return;
    }
    if (scopes[scopes.size() - 1] != scope)
    {
        return;
    }
    scopes.pop_back();
}

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
                            std::string message)
{
    if (logDetail < level)
    {
        return;
    }
    message = ExtVector::concat(scopes, "#", "sc:", " ", "") + message;
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

Log::Scope::Scope(std::string const &scope):scope(scope)
{
    LogStatController::pushScope(scope);
}

Log::Scope::~Scope()
{
    LogStatController::popScope(scope);
}
