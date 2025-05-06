#include "catch.hpp"

#include "../Decimal.hpp"


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
        REQUIRE((Decimal(0) + Decimal("0.5")) == Decimal("0.5"));
        REQUIRE((Decimal(0) + Decimal("51")) == Decimal("51"));
    }
}
