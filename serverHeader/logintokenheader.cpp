#include "logintokenheader.h"

LoginTokenHeader::LoginTokenHeader():Pistache::Http::Header::Header()
{

}

void LoginTokenHeader::parse(const std::string &data)
{
    value = data;
}

void LoginTokenHeader::write(std::ostream &stream) const
{
    stream << value;
}
