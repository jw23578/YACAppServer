#ifndef LOGINEMAILHEADER_H
#define LOGINEMAILHEADER_H


#include "pistache/http_header.h"

class LoginEMailHeader : public Pistache::Http::Header::Header
{

public:
    std::string value;
    NAME("YACAPP-LoginEMail");
    LoginEMailHeader();

    void parse(const std::string &data) override;
    void write(std::ostream &stream) const override;
};


#endif // LOGINEMAILHEADER_H
