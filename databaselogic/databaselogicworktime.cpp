#include "databaselogicworktime.h"

bool DatabaseLogicWorktime::selectWorktimeBefore(const sole::uuid &user_id, const TimePoint &ts, PGExecutor &e)
{
    PGSqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql.addCompare("and", tableFields.ts, "<", ts);
    sql += std::string(" order by ts desc limit 1");
    e.exec(sql);
    return e.size() > 0;
}

bool DatabaseLogicWorktime::selectWorktimeAfter(const sole::uuid &user_id, const TimePoint &ts, PGExecutor &e)
{
    PGSqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql.addCompare("and", tableFields.ts, ">", ts);
    sql += std::string(" order by ts limit 1");
    e.exec(sql);
    return e.size() > 0;
}

DatabaseLogicWorktime::DatabaseLogicWorktime(LogStatController &logStatController,
                                             PGConnectionPool &pool):
    logStatController(logStatController),
    pool(pool)
{

}

bool DatabaseLogicWorktime::currentState(const sole::uuid &user_id,
                                         std::chrono::system_clock::time_point &workStart,
                                         std::chrono::system_clock::time_point &pauseStart,
                                         std::chrono::system_clock::time_point &offSiteStart)
{
    PGSqlString sql("select * from ( ");
    for (int wt(WorkStartType); wt < WorktimeTypeCount; ++wt)
    {
        if (wt > 1)
        {
            sql += std::string(" union ");
        }
        sql += std::string(" select * from (select ts, type from ");
        sql += tableNames.t0012_worktime;
        sql += std::string(" where user_id = :user_id and type = :type") + ExtString::toString(wt);
        sql += std::string(" order by ts desc limit 1) a") + ExtString::toString(wt) + " ";
        sql.set(std::string("type") + ExtString::toString(wt), wt);
    }
    sql += std::string(" ) worktimes order by ts");
    sql.set("user_id", user_id);

    PGExecutor e(pool, sql);
    for (size_t i(0); i < e.size(); ++i)
    {
        std::chrono::system_clock::time_point ts(e.timepoint("ts"));
        switch (e.integer("type"))
        {
        case WorkStartType: workStart = ts; break;
        case WorkEndType: workStart = std::chrono::system_clock::time_point(); break;
        case PauseStartType: pauseStart = ts; break;
        case PauseEndType: pauseStart = std::chrono::system_clock::time_point(); break;
        case OffSiteWorkStartType: offSiteStart = ts; break;
        case OffSiteWorkEndType: offSiteStart = std::chrono::system_clock::time_point(); break;
        }
        e.next();
    }
    return true;
}

bool DatabaseLogicWorktime::insertWorktime(const sole::uuid &user_id,
                                           const std::chrono::system_clock::time_point ts,
                                           const WorktimeType type,
                                           const UserMood user_mood,
                                           const DayRating day_rating,
                                           std::chrono::system_clock::time_point &workStart,
                                           std::chrono::system_clock::time_point &pauseStart,
                                           std::chrono::system_clock::time_point &offSiteWorkStart,
                                           std::string &message)
{
    if (!currentState(user_id, workStart, pauseStart, offSiteWorkStart))
    {
        message = "could not retrieve current state";
        return false;
    }
    if (type == WorkStartType)
    {
        if (workStart != std::chrono::system_clock::time_point())
        {
            message = "work already started";
            return false;
        }
    }
    if (type == WorkEndType)
    {
        if (workStart == std::chrono::system_clock::time_point())
        {
            message = "please start work before";
            return false;
        }
        if (pauseStart != std::chrono::system_clock::time_point())
        {
            message = "please end your pause before";
            return false;
        }
        if (offSiteWorkStart != std::chrono::system_clock::time_point())
        {
            message = "please end your offSiteWork before";
            return false;
        }
    }
    if (type == PauseStartType)
    {
        if (workStart == std::chrono::system_clock::time_point())
        {
            message = "please start your work before";
            return false;
        }
        if (pauseStart != std::chrono::system_clock::time_point())
        {
            message = "pause already started";
            return false;
        }
    }
    if (type == PauseEndType)
    {
        if (pauseStart == std::chrono::system_clock::time_point())
        {
            message = "please start your pause before";
            return false;
        }
    }
    if (type == OffSiteWorkStartType)
    {
        if (workStart == std::chrono::system_clock::time_point())
        {
            message = "please start your work before";
            return false;
        }
        if (pauseStart != std::chrono::system_clock::time_point())
        {
            message = "please end your pause before";
            return false;
        }
        if (offSiteWorkStart != std::chrono::system_clock::time_point())
        {
            message = "offSiteWork already started";
            return false;
        }
    }
    if (type == OffSiteWorkEndType)
    {
        if (pauseStart != std::chrono::system_clock::time_point())
        {
            message = "please end your pause before";
            return false;
        }
        if (offSiteWorkStart == std::chrono::system_clock::time_point())
        {
            message = "please start your offSiteWork before";
            return false;
        }
    }

    switch (type)
    {
    case WorkStartType: workStart = ts; break;
    case WorkEndType: workStart = std::chrono::system_clock::time_point(); break;
    case PauseStartType: pauseStart = ts; break;
    case PauseEndType: pauseStart = std::chrono::system_clock::time_point(); break;
    case OffSiteWorkStartType: offSiteWorkStart = ts; break;
    case OffSiteWorkEndType: offSiteWorkStart = std::chrono::system_clock::time_point(); break;
    case WorktimeTypeCount: break;
    }

    PGSqlString sql;
    sql.insert(tableNames.t0012_worktime);
    sql.addInsert(tableFields.id, sole::uuid4());
    sql.addInsert(tableFields.user_id, user_id);
    sql.addInsert(tableFields.ts, ts);
    sql.addInsert(tableFields.type, type);
    sql.addInsert(tableFields.user_mood, user_mood);
    sql.addInsert(tableFields.day_rating, day_rating);

    PGExecutor e(pool, sql);
    return true;
}

bool DatabaseLogicWorktime::insertWorktimeBeginEnd(const sole::uuid &user_id,
                                                   const TimePoint &begin,
                                                   const TimePoint &end,
                                                   const WorktimeType type,
                                                   std::string &message)
{
    if (type != WorkStartType && type != PauseStartType && type != OffSiteWorkStartType)
    {
        message = "Only Workstart, Pausestart and OffsiteWorkStart can be inserted with begin/end";
        return false;
    }
    PGExecutor e(pool);
    if (selectWorktimeBefore(user_id, begin, e))
    {
        if (type == WorkStartType)
        {
            if (e.integer("type") != WorkEndType)
            {
                message = "Before Begin was not a WorkEnd";
                return false;
            }
        }
    }
    if (selectWorktimeAfter(user_id, begin, e))
    {
        if (type == WorkStartType)
        {
            if (e.timepoint("ts") < end)
            {
                message = "There are already entries between Begin and End";
                return false;
            }
        }
    }
    if (selectWorktimeAfter(user_id, end, e))
    {
        if (type == WorkStartType)
        {
            if (e.integer("type") != WorkStartType)
            {
                message = "After End was not a WorkStart";
                return false;
            }
        }
    }

    {
        PGSqlString sql;
        sql.insert(tableNames.t0012_worktime);
        sql.addInsert(tableFields.id, sole::uuid4());
        sql.addInsert(tableFields.user_id, user_id);
        sql.addInsert(tableFields.ts, begin);
        sql.addInsert(tableFields.type, type);
        sql.addInsert(tableFields.user_mood, UserMoodUnknown);
        sql.addInsert(tableFields.day_rating, DayRatingUnknown);
        PGExecutor e(pool, sql);
    }
    {
        PGSqlString sql;
        sql.insert(tableNames.t0012_worktime);
        sql.addInsert(tableFields.id, sole::uuid4());
        sql.addInsert(tableFields.user_id, user_id);
        sql.addInsert(tableFields.ts, end);
        sql.addInsert(tableFields.type, type + 1);
        sql.addInsert(tableFields.user_mood, UserMoodUnknown);
        sql.addInsert(tableFields.day_rating, DayRatingUnknown);
        PGExecutor e(pool, sql);
    }

    return true;
}

bool DatabaseLogicWorktime::fetchWorktimes(const sole::uuid &user_id,
                                           const TimePoint &since,
                                           const TimePoint &until,
                                           rapidjson::Value &targetArray,
                                           rapidjson::MemoryPoolAllocator<> &alloc,
                                           std::string &message)
{
    PGSqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql += std::string(" and ") + tableFields.ts + " >= :since ";
    sql += std::string(" and ") + tableFields.ts + " <= :until ";
    MACRO_set(sql, since);
    MACRO_set(sql, until);
    sql += std::string(" order by ") + tableFields.ts;
    PGExecutor e(pool, sql);
    e.toJsonArray(targetArray, alloc);
    return true;
}
