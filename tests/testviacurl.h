#ifndef TESTVIACURL_H
#define TESTVIACURL_H

#include <string>
#include "ormpersistenceinterface.h"

class TestViaCurl
{
    std::string baseUrl;
    ORMPersistenceInterface &opi;
public:
    TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi);
    bool run(std::string &resultMessage);
};

#endif // TESTVIACURL_H
