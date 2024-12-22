#ifndef COUTLOGGER_H
#define COUTLOGGER_H

#include "loggerstatterinterface.h"

class coutLogger : public LoggerStatterInterface
{
    static int visibleLoggingActive;
    static bool definitelyNoVisibleLogging;
public:
    coutLogger();

    class DisableCoutLogging
    {
    public:
        DisableCoutLogging()
        {
            visibleLoggingActive = true;
        }
        ~DisableCoutLogging()
        {
            visibleLoggingActive = false;
        }
    };

    class ActivateVisibleLogging
    {
        int value;
    public:
        ActivateVisibleLogging():value(1)
        {
            visibleLoggingActive += value;
        }
        void stop()
        {
            visibleLoggingActive -= value;
            value = 0;
        }
        ~ActivateVisibleLogging()
        {
            visibleLoggingActive -= value;
        }
    };

protected:
    void theLogFunction(const std::string &file, int line, LogStatController::LogLevel level, const std::string &message) override;
    void theStatFunction(const std::string &sourceType, const std::string &sourceName, LogStatController::StatType statType) override;
};

#endif // COUTLOGGER_H
