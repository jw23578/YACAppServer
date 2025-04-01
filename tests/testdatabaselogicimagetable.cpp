#include "testdatabaselogicimagetable.h"

TestDatabaseLogicImageTable::TestDatabaseLogicImageTable(DatabaseLogicImageTable &databaseLogicImageTable)
{
    std::string message;
    reducedsole::uuid imageId;
    std::basic_string<std::byte> data;
    databaseLogicImageTable.storeImage(data, message, imageId);
}
