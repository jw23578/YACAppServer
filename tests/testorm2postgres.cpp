#include "testorm2postgres.h"
#include "orm-mapper/orm2postgres.h"
#include "orm-mapper/orm2rapidjson.h"
#include "orm_implementions/t0009_appuser_logintoken.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include "pgormsqlimplementation.h"
#include "orm_implementions/t0033_words.h"
#include "postgres/pgormpersistence.h"
#include "JWUtils/definitions.h"
#include "orm_implementions/t0001_users.h"

TestORM2Postgres::TestORM2Postgres(PGConnectionPool &pool)
{
    PGORMSqlImplementation sqlImplementation(pool);
    PGORMPersistence opi(sqlImplementation);
    {
        t0001_users t0001;
        t0001.prepareFirstInsert();
        t0001.setloginemail("test@test.de");
        t0001.setuser_id(ExtUuid::generateUuid());
        opi.insertObject(t0001, ExtUuid::NullUuid);
        opi.deleteObject(t0001, ExtUuid::NullUuid);
    }


    t0033_words words;
    words.prepareFirstInsert();
    opi.insertObject(words, ExtUuid::NullUuid);
    words.word = "Jens";
    opi.insertObject(words, ExtUuid::NullUuid);

    t0033_words words2;
    opi.selectObject(words.id, words2);

    std::cout << words2.toString() << std::endl;

    opi.deleteObject(words2, NullUuid);

    t0009_appuser_logintoken ghost;
    ORMVector<t0009_appuser_logintoken> allT0009;
    opi.fetchAllObjects(allT0009);
    if (!allT0009.size())
    {
        return;
    }

    ORM2rapidjson o2j;
    rapidjson::Document document;
    document.SetObject();
    for (size_t i(0); i < allT0009.size(); ++i)
    {
        YACBaseObject &o(allT0009[i]);
        o2j.toJson(o,
                   document,
                   document.GetAllocator());

    }
    rapidjson::Value array;
    o2j.toJson(allT0009, array, document.GetAllocator());
    document.AddMember("array", array, document.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

    o2j.fromJson(document, ghost);

}
