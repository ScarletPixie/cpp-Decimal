#include "catch.hpp"

#include "../Decimal.hpp"

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

TEST_CASE("Test decimal returns a Decimal with thr correct result", "[Decimal sum]")
{
    SECTION("Positive only")
    {
        REQUIRE((Decimal(2) + Decimal("2")) == Decimal(4));
        REQUIRE((Decimal(2.9) + Decimal("2")) == Decimal("4.9"));
        REQUIRE((Decimal(2.9) + Decimal("2.1")) == Decimal("5.0"));
        REQUIRE((Decimal(0) + Decimal("0.5")) == Decimal("0.5"));
        REQUIRE((Decimal(0) + Decimal("51")) == Decimal("51"));
        REQUIRE((Decimal(100.50) + Decimal("51")) == Decimal("151.50"));
        REQUIRE((Decimal(-10) + Decimal(-10)) == Decimal("-20"));
        REQUIRE((Decimal(5) + Decimal("-2")) == Decimal("3"));
    }
}
