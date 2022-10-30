#ifndef LOGSTATCONTROLLER_H
#define LOGSTATCONTROLLER_H

#include <vector>
#include <string>
class LoggerStatterInterface;


class LogStatController
{
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
             const std::string &message);
    void stat(LogStatController::StatType statType);
};

#endif // LOGSTATCONTROLLER_H
