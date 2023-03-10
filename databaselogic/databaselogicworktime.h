#ifndef DATABASELOGICWORKTIME_H
#define DATABASELOGICWORKTIME_H

#include "postgres/pgconnectionpool.h"
#include "sole/sole.hpp"
#include "logstat/logstatcontroller.h"
#include "tablenames.h"
#include "yacAppAndServer/tablefields.h"
#include "rapidjson/document.h"
#include "definitions.h"
#include "pgexecutor.h"

class DatabaseLogicWorktime
{
    LogStatController &logStatController;
    PGConnectionPool &pool;
    TableNames tableNames;
    TableFields tableFields;

    bool selectWorktimeBefore(const sole::uuid &user_id,
                              const TimePoint &ts,
                              PGExecutor &e);
    bool selectWorktimeAfter(const sole::uuid &user_id,
                              const TimePoint &ts,
                              PGExecutor &e);
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
    enum UserMood
    {
        UserMoodUnknown = 0,
        UserMood1Best,
        UserMood2,
        UserMood3,
        UserMood4,
        UserMood5
    };

    enum DayRating
    {
        DayRatingUnknown = 0,
        DayRating1Best,
        DayRating2,
        DayRating3,
        DayRating4,
        DayRating5
    };

    bool currentState(const sole::uuid &user_id,
                      std::chrono::system_clock::time_point &workStart,
                      std::chrono::system_clock::time_point &pauseStart,
                      std::chrono::system_clock::time_point &offSiteStart);

    bool insertWorktime(const sole::uuid &user_id,
                        const std::chrono::system_clock::time_point ts,
                        const WorktimeType type,
                        const UserMood user_mood,
                        const DayRating day_rating,
                        std::chrono::system_clock::time_point &workStart,
                        std::chrono::system_clock::time_point &pauseStart,
                        std::chrono::system_clock::time_point &offSiteWorkStart,
                        std::string &message);

    bool insertWorktimeBeginEnd(const sole::uuid &user_id,
                                const TimePoint &begin,
                                const TimePoint &end,
                                const WorktimeType type,
                                std::string &message);

    bool fetchWorktimes(const sole::uuid &user_id,
                        const TimePoint &since,
                        const TimePoint &until,
                        rapidjson::Value &targetArray,
                        rapidjson::MemoryPoolAllocator<> &alloc,
                        std::string &message);
};

#endif // DATABASELOGICWORKTIME_H
