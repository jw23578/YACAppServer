#include "testorm2postgres.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0009_appuser_logintoken.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include "pgsqlimplementation.h"
#include "orm_implementions/t0033_words.h"
#include "orm/ormpersistenceinterface.h"

TestORM2Postgres::TestORM2Postgres(PGConnectionPool &pool)
{
    PGSqlImplementation sqlImplementation(pool);
    ORMPersistenceInterface opi(sqlImplementation);
    t0033_words words;
    opi.insertObject(words);


    ORM2Postgres o2p(pool);
    t0009_appuser_logintoken ghost;
    std::set<YACBaseObject*> allT0009;
    o2p.selectAll(ghost, allT0009);
    if (!allT0009.size())
    {
        return;
    }

    ORM2rapidjson o2j;
    rapidjson::Document document;
    document.SetObject();
    YACBaseObject &o(**allT0009.begin());
    o2j.toJson(o,
               document,
               document.GetAllocator());

    rapidjson::Value array;
    o2j.toJson(allT0009, array, document.GetAllocator());
    document.AddMember("array", array, document.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

    o2j.fromJson(document, ghost);

}
