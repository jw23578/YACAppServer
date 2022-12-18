#ifndef DATABASELOGICWORKTIME_H
#define DATABASELOGICWORKTIME_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "pgutils.h"
#include "tablenames.h"

class DatabaseLogicWorktime
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    PGUtils utils;
    TableNames tableNames;

public:
    DatabaseLogicWorktime(LogStatController &logStatController,
                          PGConnectionPool &pool);

    enum WorktimeType
    {
        WorkStartType = 1,
        WorkEndType,
        PauseStartType,
        PauseEndType,
        OffSiteWorkStartType,
        OffSiteWorkEndType,
        WorktimeTypeCount
    };

    bool currentState(const sole::uuid &user_id,
                      std::chrono::system_clock::time_point &workStart,
                      std::chrono::system_clock::time_point &pauseStart,
                      std::chrono::system_clock::time_point &offSiteStart);

    bool insertWorktime(const sole::uuid &user_id,
                        const std::chrono::system_clock::time_point ts,
                        const WorktimeType type,
                        std::chrono::system_clock::time_point &workStart,
                        std::chrono::system_clock::time_point &pauseStart,
                        std::chrono::system_clock::time_point &offSiteWorkStart,
                        std::string &message);
};

#endif // DATABASELOGICWORKTIME_H
