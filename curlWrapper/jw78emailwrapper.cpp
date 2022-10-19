#include "jw78emailwrapper.h"
#include <fstream>
#include <iostream>

void jw78::EMailWrapper::parse()
{
    if (parsed)
    {
        return;
    }
    parsed = true;
    size_t position(std::string::npos);
    while (true)
    {
        size_t start(position + 1);
        position = completeContent.find('\n', start);
        if (position == std::string::npos)
        {
            break;
        }
        std::string_view work(completeContent.substr(start, position - start));
        if (!work.size())
        {
            break;
        }
        if (work.substr(0, 5) == "From:")
        {
            fromLine = work;
        }
        if (work.substr(0, 3) == "To:")
        {
            toLine = work;
        }
        if (work.substr(0, 5) == "Date:")
        {
            dateLine = work;
        }
        if (work.substr(0, 8) == "Subject:")
        {
            subjectLine = work;
        }
        if (work.substr(0, 11) == "Message-ID:")
        {
            messageIdLine = work;
        }
    }
}

jw78::EMailWrapper::EMailWrapper(std::string const &uid,
                                 const std::string &c):
    uid(uid),
    completeContent(c)
{

}

void jw78::EMailWrapper::saveToFile(const std::string &filename)
{
    std::ofstream f(filename);
    f << completeContent;
}

std::string_view jw78::EMailWrapper::getFromLine()
{
    parse();
    return fromLine;
}

std::string_view jw78::EMailWrapper::getMessageIdLine()
{
    parse();
    return messageIdLine;
}
