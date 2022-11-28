#include "appidheader.h"

AppIdHeader::AppIdHeader()
{

}

void AppIdHeader::parse(const std::string &data)
{
    value = data;
}

void AppIdHeader::write(std::ostream &stream) const
{
    stream << value;
}
