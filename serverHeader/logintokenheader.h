#ifndef LOGINTOKENHEADER_H
#define LOGINTOKENHEADER_H

#include "pistache/http_header.h"


class LoginTokenHeader : public Pistache::Http::Header::Header
{
public:
public:
    std::string value;
    NAME("YACAPP-LoginEMail");
    LoginTokenHeader();

    void parse(const std::string &data) override;
    void write(std::ostream &stream) const override;
};

#endif // LOGINTOKENHEADER_H
