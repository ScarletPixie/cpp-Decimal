#include "catch.hpp"

#include "../Decimal.hpp"

#include <asm/signal.h>
#include <csignal>

namespace Catch
{
    template<>
    struct StringMaker<Decimal>
    {
        static std::string convert(const Decimal& value) 
        {
            std::ostringstream oss;
            oss << value.str();
            return oss.str();
        }
    };
}

TEST_CASE("Test decimal returns a Decimal with the correct result", "[Decimal sum]")
{
    SECTION("Positive only")
    {
        REQUIRE((Decimal(2) + Decimal("2")) == Decimal(4));
        REQUIRE((Decimal(2.9) + Decimal("2")) == Decimal("4.9"));
        REQUIRE((Decimal(2.9) + Decimal("2.1")) == Decimal("5.0"));
        REQUIRE((Decimal(0) + Decimal("0.5")) == Decimal("0.5"));
        REQUIRE((Decimal(0) + Decimal("51")) == Decimal("51"));
        REQUIRE((Decimal(100.50) + Decimal("51")) == Decimal("151.50"));
    }
    SECTION("Mixed")
    {
        REQUIRE((Decimal(1) + Decimal(-1)) == Decimal("0"));
        REQUIRE((Decimal(-1) + Decimal(1)) == Decimal("0"));
        REQUIRE((Decimal(1) + Decimal(-2)) == Decimal("-1"));
        REQUIRE((Decimal(-2) + Decimal(1)) == Decimal("-1"));
        REQUIRE((Decimal(-1) + Decimal(-1)) == Decimal("-2"));
    }
    SECTION("Fractional")
    {
        raise(SIGTRAP);
        REQUIRE((Decimal(0.3) + Decimal(-1)) == Decimal("0.7"));
        REQUIRE((Decimal(-1) + Decimal(0.3)) == Decimal("0.7"));
        REQUIRE((Decimal(1.5) + Decimal(-2)) == Decimal("-0.5"));
        REQUIRE((Decimal(-2) + Decimal(1.5)) == Decimal("-0.5"));
        REQUIRE((Decimal(-1.1) + Decimal(-1.1)) == Decimal("-2.2"));
        REQUIRE((Decimal(0) + Decimal(-1.1)) == Decimal("-1.1"));
    }
}
