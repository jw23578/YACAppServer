#include "thirdheader.h"

ThirdHeader::ThirdHeader()
{

}

void ThirdHeader::parse(const std::string &data)
{
    value = data;
}

void ThirdHeader::write(std::ostream &stream) const
{
    stream << value;
}
