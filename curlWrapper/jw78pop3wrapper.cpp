#include "jw78pop3wrapper.h"
#include "utils/extstring.h"

jw78::Pop3Wrapper::Pop3Wrapper(const std::string &h,
                                 const std::string &u,
                                 const std::string &pwd):
    host(h),
    user(u),
    password(pwd)
{

}

bool jw78::Pop3Wrapper::retrieveEMail(int number, std::string &target)
{
    cw.pop3Retrieve(host,
                    user,
                    password,
                    number,
                    target);
    return target.size() > 0;
}

bool jw78::Pop3Wrapper::deleteEMail(int number)
{
    return cw.pop3Delete(host,
                         user,
                         password,
                         number);
}

void jw78::Pop3Wrapper::eMailSizes(std::vector<size_t> &sizes)
{
    std::string result;
    if (cw.pop3NumberAndSizes(host, user, password, result))
    {
        std::vector<std::string> lines;
        ExtString::split(result, "\r\n", lines);
        for (auto l : lines)
        {
            std::string r(ExtString::rightOf(l, " "));
            size_t size(0);
            ExtString::to(r, size);
            sizes.push_back(size);
        }
    }
}
