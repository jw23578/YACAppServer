#ifndef THIRDHEADER_H
#define THIRDHEADER_H

#include "pistache/http_header.h"

class ThirdHeader : public Pistache::Http::Header::Header
{
public:
    std::string value;
    NAME("YACAPP-Third");
    ThirdHeader();

    void parse(const std::string &data) override;
    void write(std::ostream &stream) const override;
};

#endif // THIRDHEADER_H
