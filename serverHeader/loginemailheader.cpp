#include "loginemailheader.h"

LoginEMailHeader::LoginEMailHeader():Pistache::Http::Header::Header()
{

}

void LoginEMailHeader::parse(const std::string &data)
{
    value = data;
}

void LoginEMailHeader::write(std::ostream &stream) const
{
    stream << value;
}
