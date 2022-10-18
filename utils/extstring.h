#ifndef EXTSTRING_H
#define EXTSTRING_H

#include <string>
#include <vector>
#include <sstream>
#include <set>

class ExtString
{
public:
    ExtString();

    static std::string randomString(int lowerCases, int upperCases, int numbers, int signs);

    static std::string shuffle(std::string const &source);

    static void swapChars(std::string &s, int indexA, int indexB);

    static std::string coalesce(std::string const &value,
                                std::string const &coalesceValue);
    static bool contains(std::string const &s,
                         std::string const &needle);
    static void eraseAll(std::string &s,
                         std::string const &needle);

    static void replaceAll(std::string &s,
                           std::string const &needle,
                           std::string const &replacement);

    static std::string between(std::string const &s,
                               std::string const &needleLeft,
                               std::string const &needleRight,
                               bool withNeedles);
    static bool leftRight(std::string const &orig,
                          std::string const &delimiter,
                          std::string &left,
                          std::string &right);
    static size_t splitTuple(std::string const &orig,
                             std::string const &delimiter,
                             std::string const &openSign,
                             std::string const &closeSign,
                             std::vector<std::string> &result);
    static size_t split(std::string const &orig,
                        std::string const &delimiter,
                        std::set<std::string> &result);
    static size_t split(std::string const &orig,
                        std::string const &delimiter,
                        std::set<unsigned int> &result);
    static size_t splitBetween(const std::string &orig,
                               const std::string &needleLeft,
                               const std::string &needleRight,
                               bool withNeedles,
                               const std::string &delimiter,
                               std::vector<std::string> &result);
    static size_t split(std::string const &orig,
                        std::string const &delimiter,
                        std::vector<std::string> &result);
    static size_t split(std::string const &orig,
                        std::string const &delimiter,
                        const std::string &quoteSign,
                        const std::string &escapeSign,
                        std::vector<std::string> &result);
    static std::string rightOf(std::string const &s, std::string const &needle);

    static bool isNumeric(std::string const &s);


    static std::string &ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
    static std::string &rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
    static std::string &trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

    static std::string lower(const std::string &s);
    static std::string upper(const std::string &s);
    static void upperSelf(std::string &s);
    static void lowerSelf(std::string &s);

    static std::string escape(std::string text,
                              const std::string &needle,
                              const std::string &escapeSign);
    static std::string quote(std::string text,
                             const std::string &quoteSign = "\"",
                             const std::string &escapeSign = "\\");

    static std::string unEscape(std::string text,
                                const std::string &needle,
                                const std::string &escapeSign);
    static std::string unQuote(std::string text,
                               const std::string &quoteSign = "\"",
                               const std::string &escapeSign = "\\");

    static void stars(std::string &s, size_t startPos, size_t length);

    template <class T> static void to(std::string const &s, T &target)
    {
        std::stringstream ss(s);
        ss >> target;
    }

    template <class T> static std::string toString (const T& t)
    {
        std::stringstream ss;
        ss << std::fixed;
        ss << t;
        return ss.str();
    }
    static std::string urlDecode(std::string &src);
};

#endif // EXTSTRING_H
