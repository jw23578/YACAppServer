#include "extstring.h"
#include <sstream>
#include <algorithm>
#include <regex>
#include <stdlib.h>
#include <iomanip>
#include "definitions.h"

ExtString::ExtString()
{

}

std::string ExtString::randomString(int lowerCases, int upperCases, int numbers, int signs)
{
    std::string lower("abcdefghijklmnopqrstuvwxyz");
    std::string upper("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    std::string num("01234567890");
    std::string si("!§$%&/()=?+*#'-_.:,;@");
    std::string result;
    for (; lowerCases; --lowerCases)
    {
        result += lower[rand() % lower.size()];
    }
    for (; upperCases; --upperCases)
    {
        result += upper[rand() % upper.size()];
    }
    for (; numbers; --numbers)
    {
        result += num[rand() % num.size()];
    }
    for (; signs; --signs)
    {
        result += si[rand() % si.size()];
    }
    return shuffle(result);
}

std::string ExtString::shuffle(const std::string &source)
{
    std::string result(source);
    for (size_t i(0); i < result.size(); ++i)
    {
        swapChars(result, i, rand() % result.size());
    }
    return result;
}

void ExtString::swapChars(std::string &s, int indexA, int indexB)
{
    if (indexA == indexB)
    {
        return;
    }
    char h(s[indexA]);
    s[indexA] = s[indexB];
    s[indexB] = h;
}

std::string ExtString::coalesce(const std::string &value,
                                const std::string &coalesceValue)
{
    if (value.size())
    {
        return value;
    }
    return coalesceValue;
}

bool ExtString::contains(const std::string &s,
                         const std::string &needle)
{
    return s.find(needle) != std::string::npos;
}

void ExtString::eraseAll(std::string &s,
                         const std::string &needle)
{
    size_t p(s.find(needle));
    while (p != std::string::npos)
    {
        s.erase(p, needle.size());
        p = s.find(needle, p);
    }
}

void ExtString::replaceAll(std::string &s,
                           std::string const &needle,
                           std::string const &replacement)
{
    size_t pos = s.find(needle);
    while( pos != std::string::npos)
    {
        s.replace(pos, needle.size(), replacement);
        pos = s.find(needle, pos + replacement.size());
    }

}



std::string ExtString::between(const std::string &s,
                               const std::string &needleLeft,
                               const std::string &needleRight,
                               bool withNeedles)
{
    size_t leftPos(s.find(needleLeft));
    if (leftPos == std::string::npos)
    {
        return "";
    }
    size_t rightPos(s.find(needleRight, leftPos + needleLeft.size()));
    if (rightPos == std::string::npos)
    {
        return "";
    }
    if (withNeedles)
    {
        return s.substr(leftPos, rightPos - leftPos + needleRight.size());
    }
    size_t leftStart(leftPos + needleLeft.size());
    return s.substr(leftStart, rightPos - leftStart);
}

bool ExtString::leftRight(const std::string &orig,
                          const std::string &delimiter,
                          std::string &left,
                          std::string &right)
{
    size_t p(orig.find(delimiter));
    if (p == std::string::npos)
    {
        return false;
    }
    left = orig.substr(0, p);
    right = orig.substr(p + delimiter.size());
    return left.size() && right.size();
}

size_t ExtString::splitTuple(const std::string &orig,
                             const std::string &delimiter,
                             const std::string &openSign,
                             const std::string &closeSign,
                             std::vector<std::string> &result)
{
    return split(orig.substr(openSign.size(), orig.size() - openSign.size() - closeSign.size()), delimiter, result);
}

size_t ExtString::split(const std::string &orig,
                        const std::string &delimiter,
                        std::set<std::string> &result)
{
    size_t lastPos(0);
    size_t pos(orig.find(delimiter));
    while (pos != std::string::npos)
    {
        result.insert(orig.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = orig.find(delimiter, lastPos);
    }
    if (lastPos < orig.size())
    {
        result.insert(orig.substr(lastPos));
    }
    return result.size();
}

size_t ExtString::split(const std::string &orig,
                        const std::string &delimiter,
                        std::set<unsigned int> &result)
{
    size_t lastPos(0);
    size_t pos(orig.find(delimiter));
    while (pos != std::string::npos)
    {
        unsigned int v(0);
        to(orig.substr(lastPos, pos - lastPos), v);
        result.insert(v);
        lastPos = pos + delimiter.size();
        pos = orig.find(delimiter, lastPos);
    }
    if (lastPos < orig.size())
    {
        unsigned int v(0);
        to(orig.substr(lastPos), v);
        result.insert(v);
    }
    return result.size();
}

size_t ExtString::splitBetween(const std::string &orig,
                               const std::string &needleLeft,
                               const std::string &needleRight,
                               bool withNeedles,
                               const std::string &delimiter,
                               std::vector<std::string> &result)
{
    size_t leftPos(orig.find(needleLeft));
    if (leftPos == std::string::npos)
    {
        return 0;
    }
    size_t rightPos(orig.find(needleRight, leftPos + needleLeft.size()));
    if (rightPos == std::string::npos)
    {
        return 0;
    }
    if (withNeedles)
    {
        rightPos += needleRight.size();
    }
    else
    {
        leftPos += needleLeft.size();
    }
    size_t pos(orig.find(delimiter, leftPos));
    while (pos != std::string::npos && pos < rightPos)
    {
        result.push_back(orig.substr(leftPos, pos - leftPos));
        leftPos = pos + delimiter.size();
        pos = orig.find(delimiter, leftPos);
    }
    if (leftPos < rightPos)
    {
        result.push_back(orig.substr(leftPos, rightPos - leftPos));
    }
    return result.size();
}

size_t ExtString::split(const std::string &orig,
                        const std::string &delimiter,
                        std::vector<std::string> &result)
{
    size_t lastPos(0);
    size_t pos(orig.find(delimiter));
    while (pos != std::string::npos)
    {
        result.push_back(orig.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = orig.find(delimiter, lastPos);
    }
    if (lastPos < orig.size())
    {
        result.push_back(orig.substr(lastPos));
    }
    return result.size();
}

size_t ExtString::split(const std::string &orig,
                        const std::string &delimiter,
                        const std::string &quoteSign,
                        const std::string &escapeSign,
                        std::vector<std::string> &result)
{
    bool inQuote(false);
    size_t p(0);
    std::string part;
    while (p < orig.size())
    {
        if (!inQuote && orig.substr(p, quoteSign.size()) == quoteSign)
        {
            inQuote = true;
            p += quoteSign.size();
            continue;
        }
        if (inQuote && orig.substr(p, quoteSign.size()) == quoteSign
                && orig.substr(p - escapeSign.size(), escapeSign.size()) != escapeSign
                && orig.substr(p + quoteSign.size(), escapeSign.size()) != escapeSign)
        {
            inQuote = false;
            p += quoteSign.size();
            continue;
        }
        if (orig.substr(p, delimiter.size()) == delimiter)
        {
            if (part.size())
            {
                result.push_back(part);
                part = "";
            }
            p += delimiter.size();
            continue;
        }
        part += orig[p];
        ++p;
    }
    if (part.size())
    {
        result.push_back(part);
    }
    return result.size();
}

std::string ExtString::rightOf(const std::string &s, const std::string &needle)
{
    size_t p(s.find(needle));
    if (p == std::string::npos)
    {
        return "";
    }
    return s.substr(p + needle.size(), s.size());
}

bool ExtString::isNumeric(const std::string &s)
{
    if (s.size() == 0)
    {
        return false;
    }
    for (size_t i(0); i < s.size(); ++i)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

std::string& ExtString::ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& ExtString::rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& ExtString::trim(std::string &str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}

std::string ExtString::constLTrim(const std::string &str, const std::string &chars)
{
    return str.substr(str.find_first_not_of(chars));
}

std::string ExtString::constRTrim(const std::string &str, const std::string &chars)
{
    return str.substr(0, str.find_last_not_of(chars) + 1);
}

std::string ExtString::constTrim(const std::string &str, const std::string &chars)
{
    return constLTrim(constRTrim(str, chars), chars);
}

std::string ExtString::lower(const std::string &s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string ExtString::upper(const std::string &s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}


void ExtString::upperSelf(std::string &s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

void ExtString::lowerSelf(std::string &s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

std::string ExtString::escape(std::string text,
                              std::string const &needle,
                              std::string const &escapeSign)
{
    replaceAll(text, needle, escapeSign + needle);
    return text;
}


std::string ExtString::quote(std::string text,
                             const std::string &quoteSign,
                             const std::string &escapeSign)
{
    escape(text, quoteSign, escapeSign);
    return quoteSign + text + quoteSign;
}

std::string ExtString::unEscape(std::string text,
                                const std::string &needle,
                                const std::string &escapeSign)
{
    replaceAll(text, escapeSign + needle, needle);
    return text;
}

std::string ExtString::unQuote(std::string text,
                               const std::string &quoteSign,
                               const std::string &escapeSign)
{
    if (text.substr(0, quoteSign.size()) == quoteSign
            && text.substr(text.size() - quoteSign.size(), quoteSign.size()) == quoteSign)
    {
        text = text.substr(quoteSign.size(), text.size() - 2 * quoteSign.size());
    }
    return unEscape(text, quoteSign, escapeSign);
}

void ExtString::stars(std::string &s, size_t startPos, size_t length)
{
    for (size_t i(0); i < length; ++i)
    {
        s[i + startPos] = '*';
    }
}

std::string ExtString::urlDecode(std::string &src)
{
    std::string ret;
    ret.reserve(src.size());
    for (size_t i(0); i < src.size(); ++i)
    {
        if (src[i] == '%')
        {
            int code(0);
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &code);
            char ch(static_cast<char>(code));
            ret += ch;
            i += 2;
        }
        else
        {
            ret += src[i];
        }
    }
    return ret;
}

std::string ExtString::timepointToISO(const std::chrono::system_clock::time_point &tp)
{
    if (tp == std::chrono::system_clock::time_point())
    {
        return "";
    }
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream ss;
    ss << std::put_time(gmtime(&t), "%FT%TZ");
    return ss.str();
}

std::chrono::system_clock::time_point ExtString::toTimepoint(const std::string &s)
{
    if (s.find("1900-01-01T12:00:00") == 0)
    {
        return TimePointPostgreSqlNull;
    }
    std::tm tm;
    memset(&tm, 0, sizeof(tm));
    std::string format("%F %T");
    if (s.size() >= 11)
    {
        format[2] = s[10];
    }
    strptime(s.c_str(), format.c_str(), &tm);
    time_t t = mktime(&tm);

    std::chrono::time_point<std::chrono::system_clock> time;
    time = std::chrono::system_clock::from_time_t(t);
    auto timeMs = std::chrono::time_point_cast<std::chrono::milliseconds>(time);

    if (s.size() > 19)
    {
        int value(0);
        std::string strMSecs(s.substr(20, 3));
        ExtString::to(strMSecs, value);
        timeMs += std::chrono::milliseconds(value);
    }

    if (s.size() > 23)
    {
        int timeZone(0);
        std::string strTimeZone(s.substr(24, 2));
        ExtString::to(strTimeZone, timeZone);
        if (s.substr(23, 1) == "+")
        {
            timeMs += std::chrono::hours(timeZone);
        }
        if (s.substr(23, 1) == "-")
        {
            timeMs -= std::chrono::hours(timeZone);
        }
    }


    return timeMs;

    //    double fraction;
    //    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S") >> fraction;
    //    if (!ss)
    //    {
    //        return std::chrono::system_clock::now();
    //    }
    //    time_t secondsSinceEpoch = mktime(&tm);
    //    int64_t microseconds = fraction * 1'000'000;
    //    std::chrono::milliseconds milliseconds(secondsSinceEpoch * 1000);
    //    std::chrono::system_clock::time_point tp(milliseconds);
    //    return tp;
}



bool ExtString::emailIsValid(const std::string &email)
{
    static const std::regex pattern("^[_a-zA-Z0-9-]+(\\.[_a-zA-Z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,63})$");
    return (std::regex_match(email, pattern));
}
