#ifndef CACHEINTERFACE_H
#define CACHEINTERFACE_H

#include "databaselogics.h"

class CacheInterface
{
protected:
    DatabaseLogics &databaseLogics;
public:
    CacheInterface(DatabaseLogics &databaseLogics);
};

#endif // CACHEINTERFACE_H
