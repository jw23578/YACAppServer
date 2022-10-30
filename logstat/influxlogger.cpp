#include "influxlogger.h"

void InfluxLogger::theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message)
{
}

void InfluxLogger::theStatFunction(const std::string &sourceType,
                                   const std::string &sourceName,
                                   LogStatController::StatType statType)
{
    std::string logType;
    std::string subLogType;
    if (statType == LogStatController::postgresqlInsertQuery)
    {
        logType = "postgresqlQuery";
        subLogType = "postgresqlInsertQuery";
    }
    if (statType == LogStatController::postgresqlSelectQuery)
    {
        logType = "postgresqlQuery";
        subLogType = "postgresqlSelectQuery";
    }
    if (statType == LogStatController::postgresqlQuery)
    {
        logType = "postgresqlQuery";
        subLogType = "postgresqlQuery";
    }
    if (statType == LogStatController::postgresqlConnectionCreated)
    {
        logType = "postgresqlConnectionCreated";
        subLogType = "postgresqlConnectionCreated";
    }
    if (statType == LogStatController::postgresqlCreateDatabase)
    {
        logType = "postgresqlCreateDatabase";
        subLogType = "postgresqlCreateDatabase";
    }
    if (statType == LogStatController::postgresqlInstallPGCrypto)
    {
        logType = "postgresqlInstallPGCrypto";
        subLogType = "postgresqlInstallPGCrypto";
    }

    if (statType == LogStatController::getRequest)
    {
        logType = "request";
        subLogType = "getRequest";
    }
    if (statType == LogStatController::putRequest)
    {
        logType = "request";
        subLogType = "putRequest";
    }
    if (statType == LogStatController::postRequest)
    {
        logType = "request";
        subLogType = "postRequest";
    }
    if (statType == LogStatController::request)
    {
        logType = "request";
        subLogType = "request";
    }
    std::string resp;
    influxdb_cpp::builder().meas("stats")
            .tag("sourceType", sourceType)
            .tag("sourceName", sourceName)
            .tag("logtype", logType)
            .tag("subLogType", subLogType)
            .field("v", 1)
            .send_udp(udpHost, udpPort);
}

InfluxLogger::InfluxLogger(std::string const &udpHost,
                           int udpPort):
    udpHost(udpHost),
    udpPort(udpPort)
{

}

InfluxLogger::~InfluxLogger()
{
}
