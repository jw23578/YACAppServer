#include "testdatabaselogics.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

TestDatabaseLogics::TestDatabaseLogics(DatabaseLogics &databaseLogics)
{
    reducedsole::uuid id(reducedsole::uuid4());
    reducedsole::uuid app_id(reducedsole::uuid4());
    reducedsole::uuid owner_id(reducedsole::uuid4());
    reducedsole::uuid creater_id(reducedsole::uuid4());
    rapidjson::Document document;
    document.SetObject();
    std::string message;
    DatabaseLogicRightGroup &dlrg(databaseLogics.databaseLogicRightGroup);
    rapidjson::Value rightGroup;
    bool automatic(false);
    id = ExtUuid::NullUuid;
    bool request_allowed(false);
    bool visible_for_non_members(false);
    dlrg.deleteRightGroup(id, id, message);
    dlrg.deleteRightGroup(id, creater_id, message);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    databaseLogics.getLogStat().log(__FILE__, __LINE__, LogStatController::verbose, buffer.GetString());
}
