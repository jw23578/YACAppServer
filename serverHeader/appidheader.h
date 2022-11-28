#ifndef APPIDHEADER_H
#define APPIDHEADER_H

#include "pistache/http_header.h"

class AppIdHeader : public Pistache::Http::Header::Header
{
public:
    std::string value;
    NAME("YACAPP-AppId");
    AppIdHeader();

    void parse(const std::string &data) override;
    void write(std::ostream &stream) const override;
};

#endif // APPIDHEADER_H
