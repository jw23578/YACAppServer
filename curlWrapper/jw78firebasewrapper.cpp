#include "jw78firebasewrapper.h"
#include "jw78curlwrapper.h"

jw78::FirebaseWrapper::FirebaseWrapper()
{

}

bool jw78::FirebaseWrapper::legacySend(const std::string &key,
                                       const std::string &payload,
                                       std::string &result)
{
    CurlWrapper cw;
    cw.headers.push_back(std::string("Authorization: key=") + key);
    cw.headers.push_back("Application/json");
    cw.headers.push_back("Content-type: application/json");
    std::string message;
    return cw.httpsPost("https://fcm.googleapis.com/fcm/send",
                        payload,
                        result,
                        message);
}
