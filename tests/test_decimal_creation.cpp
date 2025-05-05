#include "catch.hpp"

#include "../Decimal.hpp"

#include <string>

TEST_CASE("Test Decimal instantiation creates a Decimal object with the correct value", "[Decimal instantiation]")
{
    SECTION("Simple positive number")
    {
        REQUIRE(Decimal(0).num() == 0);
        REQUIRE(Decimal(0).str() == "0");
        REQUIRE(Decimal("0").num() == 0);
        REQUIRE(Decimal("0").str() == "0");

        REQUIRE(Decimal("42").num() == 42);
        REQUIRE(Decimal(42).str() == "42");
        REQUIRE(Decimal("7").num() == 7);
        REQUIRE(Decimal(7).str() == "7");

        const std::string& largeNum = "9999999999999999999999999999999999999999999999999999999999999999";
        REQUIRE(Decimal(largeNum).str() == largeNum);
    }
}
