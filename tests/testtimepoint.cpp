#include "testtimepoint.h"
#include <string>
#include "utils/extstring.h"
#include <iomanip>
#include <iostream>

TestTimepoint::TestTimepoint()
{
    std::string s("2022-11-27 14:58:49.593+00");
    std::chrono::system_clock::time_point tp(ExtString::toTimepoint(s));

    {
        std::string s2("2022-11-27T14:58:49.593+00");
        std::chrono::system_clock::time_point tp2(ExtString::toTimepoint(s2));
    }

    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time( &tm, "%FT%T%z" );
    std::string s2(ss.str());

    std::cout << s << std::endl;
    std::cout << s2 << std::endl;
}
