#ifndef TESTVIACURL_H
#define TESTVIACURL_H

#include <string>
#include "ormpersistenceinterface.h"
#include "curlWrapper/jw78curlwrapper.h"
#include "extrapidjson.h"
#include "yacappservermethodnames.h"

class TestViaCurl
{
    jw78::CurlWrapper cw;
    std::string baseUrl;
    ORMPersistenceInterface &opi;
    reducedsole::uuid appId = ExtUuid::stringToUuid("00000000-0000-0000-0000-000000000001");
    YACAPPServerMethodNames methodNames;
public:
    TestViaCurl(int port, const std::string &baseUrl, ORMPersistenceInterface &opi);
    bool run(std::string &resultMessage);
private:
    bool post(jw78::CurlWrapper &cw,
              const std::string &method,
              const std::string &jsonPayload,
              rapidjson::Document &resultJSON,
              std::string &message,
              bool expected);
    bool testRegisterUser(const std::string &loginEMail,
                          const std::string &password);
};

#endif // TESTVIACURL_H
