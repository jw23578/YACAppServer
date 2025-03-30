#ifndef LOGSTATCONTROLLER_H
#define LOGSTATCONTROLLER_H

#include <vector>
#include <string>
#include "rapidjson/document.h"
class LoggerStatterInterface;

#define MACRO_NV(name) \
std::string(#name) + ": " + ExtString::toString(name)


namespace Log
{
class Scope
{
    std::string scope;
public:
    Scope(std::string const &scope);
    ~Scope();
};
}

class LogStatController
{
    friend Log::Scope;
    static LogStatController *instance;
    std::vector<std::string> scopes;
    static void pushScope(std::string const &scope);
    static void popScope(std::string const &scope);
public:
    enum LogLevel
    {
        never = 0,
        panic,
        error,
        info,
        verbose
    };
private:
    LogLevel logDetail;
    std::string sourceType;
    std::string sourceName;
    std::vector<LoggerStatterInterface*> loggerStatter;

public:
    LogStatController(const LogLevel &ld,
                      std::string const &sourceType,
                      std::string const &sourceName);
    ~LogStatController();

    void add(LoggerStatterInterface *ls);

    enum StatType
    {
        postgresqlInsertQuery,
        postgresqlSelectQuery,
        postgresqlQuery,
        postgresqlConnectionCreated,
        postgresqlCreateDatabase,
        postgresqlInstallPGCrypto,
        getRequest,
        postRequest,
        putRequest,
        request
    };

    void log(const std::string &file,
             int line,
             LogLevel level,
             std::string message);
    void log(const std::string &file,
             int line,
             LogLevel level,
             const rapidjson::Document &doc);
    void stat(LogStatController::StatType statType);

    static void slog(const std::string &file,
                     int line,
                     LogLevel level,
                     const std::string &message);
};

#endif // LOGSTATCONTROLLER_H
