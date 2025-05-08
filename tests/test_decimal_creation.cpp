#include "catch.hpp"

#include "../Decimal.hpp"

#include <string>

TEST_CASE("Test Decimal instantiation with invalid values will throw exception", "[Decimal instantiation]")
{
    SECTION("Valid number with multiple signals")
    {
        REQUIRE_THROWS_AS(Decimal("++10"), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal("++"), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal("-"), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal(""), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal(" "), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal("50ab"), const Decimal::InvalidFormatException&);
        REQUIRE_THROWS_AS(Decimal("50-"), const Decimal::InvalidFormatException&);
    }
}

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

        REQUIRE(Decimal("+0").str() == "0");
        REQUIRE(Decimal("+800").num() == 800);
        REQUIRE(Decimal(800).str() == "800");

        const std::string& largeNum = "9999999999999999999999999999999999999999999999999999999999999999";
        REQUIRE(Decimal(largeNum).str() == largeNum);
        
        REQUIRE(Decimal("0001.0").num() == 1);
        REQUIRE(Decimal("+0001.0").str() == "1");
        REQUIRE(Decimal("+0101.10").str() == "101.1");
    }

    SECTION("Simple negative number")
    {
        REQUIRE(Decimal(-10).num() == -10);
        REQUIRE(Decimal("-10").str() == "-10");
        
        REQUIRE(Decimal(-111).num() == -111);
        REQUIRE(Decimal("-1").str() == "-1");
        REQUIRE(Decimal("-0").str() == "0");
        
        const std::string& largeNum = "-9999999999999999999999999999999999999999999999999999999999999999";
        REQUIRE(Decimal(largeNum).str() == largeNum);
        REQUIRE(Decimal("-0001.0").num() == -1);
        REQUIRE(Decimal("-0001.0").str() == "-1");
    }

    SECTION("Fractional number")
    {
        REQUIRE(Decimal(-10.5).num() == -10.5);
        REQUIRE(Decimal("8.8888").str() == "8.8888");
        REQUIRE(Decimal(0.1).num() == 0.1);
        REQUIRE(Decimal("0.1").str() == "0.1");

        const std::string& largeNum = "-9999999999999999999999999999999999999999999999999999999999999999.000000000000000000000000000000021";
        REQUIRE(Decimal(largeNum).str() == largeNum);
        REQUIRE(Decimal("-0001.100").num() == -1.1);
        REQUIRE(Decimal("-0001.100").str() == "-1.1");
    }
}
