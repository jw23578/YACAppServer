#include "mandantheader.h"

MandantHeader::MandantHeader()
{

}

void MandantHeader::parse(const std::string &data)
{
    value = data;
}

void MandantHeader::write(std::ostream &stream) const
{
    stream << value;
}
