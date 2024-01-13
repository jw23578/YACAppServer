#ifndef MANDANTHEADER_H
#define MANDANTHEADER_H

#include "pistache/http_header.h"

class MandantHeader : public Pistache::Http::Header::Header
{
public:
    std::string value;
    NAME("YACAPP-Mandant");
    MandantHeader();

    void parse(const std::string &data) override;
    void write(std::ostream &stream) const override;
};

#endif // MANDANTHEADER_H
