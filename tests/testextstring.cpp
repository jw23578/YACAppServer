#include "testextstring.h"
#include "thirds/doctest/doctest/doctest.h"
#include "extstring.h"

TEST_CASE("testing trim functions")
{
    std::string original("  moin moin   ");
    SUBCASE("constTrim")
    {
        std::string a(ExtString::constTrim(original));
        CHECK(ExtString::constTrim(original) == "moin moin");
    }
    SUBCASE("constLTrim")
    {
        std::string a(ExtString::constLTrim(original));
        CHECK(ExtString::constLTrim(original) == "moin moin   ");
    }
    SUBCASE("constRTrim")
    {
        std::string a(ExtString::constRTrim(original));
        CHECK(ExtString::constRTrim(original) == "  moin moin");
    }
}

TestExtString::TestExtString()
{
}
