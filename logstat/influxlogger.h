#ifndef INFLUXLOGGER_H
#define INFLUXLOGGER_H

#include <string>
#include "loggerstatterinterface.h"
#include "influxdb/influxdb.hpp"
// https://github.com/orca-zhang/influxdb-cpp
// https://docs.influxdata.com/influxdb/v1.8/supported_protocols/udp/
// bei udp muss die datenbank in influx für den port eingestellt werden!

class InfluxLogger: public LoggerStatterInterface
{
    std::string const udpHost;
    int udpPort;
protected:
    virtual void theLogFunction(std::string const &file,
                                int line,
                                LogStatController::LogLevel level,
                                std::string const &message) override;
    virtual void theStatFunction(std::string const &sourceType,
                                 std::string const &sourceName,
                                 LogStatController::StatType statType) override;
public:
    InfluxLogger(std::string const &udpHost,
                 int udpPort);
    ~InfluxLogger();
};

#endif // INFLUXLOGGER_H
