#ifndef TESTVIACURL_H
#define TESTVIACURL_H

#include <string>
#include "ormpersistenceinterface.h"
#include "curlWrapper/jw78curlwrapper.h"

class TestViaCurl
{
    jw78::CurlWrapper cw;
    std::string baseUrl;
    ORMPersistenceInterface &opi;
public:
    TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi);
    bool run(std::string &resultMessage);
private:
    bool post(const std::string &url, const std::string &jsonPayload, std::string &result, std::string &message);
};

#endif // TESTVIACURL_H
