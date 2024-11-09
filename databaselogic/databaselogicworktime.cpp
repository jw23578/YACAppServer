#include "databaselogicworktime.h"

bool DatabaseLogicWorktime::selectWorktimeBefore(const sole::uuid &user_id, const TimePoint &ts, PGExecutor &e)
{
    SqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql.addCompare("and", tableFields.ts, "<", ts);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    sql += std::string(" order by ts desc limit 1");
    e.exec(sql);
    return e.size() > 0;
}

bool DatabaseLogicWorktime::selectWorktimeAfter(const sole::uuid &user_id,
                                                const TimePoint &ts,
                                                const std::set<WorktimeType> &typesAllowed,
                                                PGExecutor &e)
{
    SqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql.addCompare("and", tableFields.ts, ">", ts);
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    if (typesAllowed.size())
    {
        sql += std::string(" and type in (:typesallowed) ");
        MACRO_set(sql, typesAllowed);
    }
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
    SqlString sql("select * from ( ");
    for (int wt(WorkStartType); wt < WorktimeTypeCount; ++wt)
    {
        if (wt > 1)
        {
            sql += std::string(" union ");
        }
        sql += std::string(" select * from (select ts, type from ");
        sql += tableNames.t0012_worktime;
        sql += std::string(" where user_id = :user_id and type = :type") + ExtString::toString(wt);
        sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
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

    SqlString sql;
    sql.insert(tableNames.t0012_worktime);
    sql.addInsert(tableFields.id, sole::uuid4(), false);
    sql.addInsert(tableFields.user_id, user_id, false);
    sql.addInsert(tableFields.ts, ts, false);
    sql.addInsert(tableFields.type, type, false);
    sql.addInsert(tableFields.user_mood, user_mood, false);
    sql.addInsert(tableFields.day_rating, day_rating, false);
    sql.addInsert(tableFields.deleted_datetime, TimePointPostgreSqlNull, true);
    sql.addInsert(tableFields.deleted_appuser_id, ExtUuid::NullUuid, true);

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
        WorktimeType beforeType(static_cast<WorktimeType>(e.integer(tableFields.type)));
        if (type == WorkStartType)
        {
            if (beforeType != WorkEndType)
            {
                message = "Before Begin was not a WorkEnd";
                return false;
            }
        }
        if (type == PauseStartType)
        {
            if (beforeType == WorkEndType)
            {
                message = "Pause can not be inserted after WorkEnd";
                return false;
            }
            if (beforeType == PauseStartType)
            {
                message = "Pause can not be inserted in another Pause";
                return false;
            }
        }
        if (type == OffSiteWorkEndType)
        {
            if (beforeType == WorkEndType)
            {
                message = "OffsiteWork can not be inserted after WorkEnd";
                return false;
            }
            if (beforeType == PauseStartType)
            {
                message = "OffsiteWork can not be inserted in a Pause";
                return false;
            }
            if (beforeType == OffSiteWorkStartType)
            {
                message = "OffsiteWork can not be inserted in another OffsiteWork";
                return false;
            }
        }
    }
    if (type == WorkStartType || type == PauseStartType)
    {
        if (selectWorktimeAfter(user_id, begin, {}, e))
        {
            if (e.timepoint("ts") < end)
            {
                message = "There are already entries between Begin and End";
                return false;
            }
        }
    }
    if (type == OffSiteWorkStartType)
    {
        if (selectWorktimeAfter(user_id, begin, {WorkStartType, WorkEndType, OffSiteWorkStartType, OffSiteWorkEndType}, e))
        {
            if (e.timepoint("ts") < end)
            {
                message = "There are already entries between Begin and End";
                return false;
            }
        }
    }
    if (selectWorktimeAfter(user_id, end, {}, e))
    {
        WorktimeType afterType(static_cast<WorktimeType>(e.integer(tableFields.type)));
        if (type == WorkStartType)
        {
            if (afterType != WorkStartType)
            {
                message = "After End was not a WorkStart";
                return false;
            }
        }
        if (type == PauseStartType)
        {
            if (afterType == WorkStartType)
            {
                message = "End is after a WorkEnd";
                return false;
            }
        }
        if (type == OffSiteWorkStartType)
        {
            if (afterType == OffSiteWorkEndType)
            {
                message = "OffsiteWork can not be inserted in another OffsiteWork";
                return false;
            }
            if (afterType == PauseEndType)
            {
                message = "OffsiteWork can not be inserted in a Pause";
                return false;
            }
            if (afterType == WorkStartType)
            {
                message = "End is after a WorkEnd";
                return false;
            }
        }
    }

    {
        SqlString sql;
        sql.insert(tableNames.t0012_worktime);
        sql.addInsert(tableFields.id, sole::uuid4(), false);
        sql.addInsert(tableFields.user_id, user_id, false);
        sql.addInsert(tableFields.ts, begin, false);
        sql.addInsert(tableFields.type, type, false);
        sql.addInsert(tableFields.user_mood, UserMoodUnknown, false);
        sql.addInsert(tableFields.day_rating, DayRatingUnknown, false);
        sql.addInsert(tableFields.deleted_datetime, TimePointPostgreSqlNull, true);
        sql.addInsert(tableFields.deleted_appuser_id, ExtUuid::NullUuid, true);
        PGExecutor e(pool, sql);
    }
    {
        SqlString sql;
        sql.insert(tableNames.t0012_worktime);
        sql.addInsert(tableFields.id, sole::uuid4(), false);
        sql.addInsert(tableFields.user_id, user_id, false);
        sql.addInsert(tableFields.ts, end, false);
        sql.addInsert(tableFields.type, type + 1, false);
        sql.addInsert(tableFields.user_mood, UserMoodUnknown, false);
        sql.addInsert(tableFields.day_rating, DayRatingUnknown, false);
        sql.addInsert(tableFields.deleted_datetime, TimePointPostgreSqlNull, true);
        sql.addInsert(tableFields.deleted_appuser_id, ExtUuid::NullUuid, true);
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
    SqlString sql;
    sql.select(tableNames.t0012_worktime);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql += std::string(" and ") + tableFields.ts + " >= :since ";
    sql += std::string(" and ") + tableFields.ts + " <= :until ";
    sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
    MACRO_set(sql, since);
    MACRO_set(sql, until);
    sql += std::string(" order by ") + tableFields.ts;
    PGExecutor e(pool, sql);
    while (e.resultAvailable() && e.integer(tableFields.type) != 1)
    {
        e.next();
    }
    ORM2Postgres o2p(pool);
    o2p.toJsonArray(e, targetArray, alloc);
    return true;
}

bool DatabaseLogicWorktime::deleteWorktime(const sole::uuid &user_id,
                                           const sole::uuid &id,
                                           std::string &message)
{
    sole::uuid endId(ExtUuid::NullUuid);
    {
        SqlString sql;
        sql.select(tableNames.t0012_worktime);
        sql.addCompare("where", tableFields.user_id, "=", user_id);
        sql.addCompare("and", tableFields.deleted_datetime, "is", TimePointPostgreSqlNull);
        sql += std::string(" and ts >= (select ts from t0012_worktime where id = :id) ");
        sql += std::string(" and type = (select type + 1 from t0012_worktime where id = :id) ");
        sql += "order by ts limit 1";
        MACRO_set(sql, id);
        PGExecutor e(pool, sql);
        if (e.size())
        {
            endId = e.uuid(tableFields.id);
        }
    }
    SqlString sql;
    sql.update(tableNames.t0012_worktime);
    sql.addSet(tableFields.deleted_datetime, TimePointPostgreSqlNow, false);
    sql.addSet(tableFields.deleted_appuser_id, user_id, false);
    sql.addCompare("where", tableFields.user_id, "=", user_id);
    sql += " and deleted_datetime is null ";
    sql += std::string(" and ts >= (select ts from t0012_worktime where id = :id) ");
    if (endId != ExtUuid::NullUuid)
    {
        sql += std::string(" and ts <= (select ts from t0012_worktime where id = :endid) ");
        MACRO_set(sql, endId);
    }
    MACRO_set(sql, id);
    PGExecutor e(pool, sql);
    return true;
}
