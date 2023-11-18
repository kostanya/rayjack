#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "interval.h"

using Approx = Catch::Approx;

TEST_CASE("Interval class tests", "[Interval]") {
    SECTION("Default constructor creates empty interval") {
        Interval interval;
        REQUIRE(interval.min == Approx(+infinity));
        REQUIRE(interval.max == Approx(-infinity));
    }

    SECTION("Parameterized constructor sets min and max values correctly") {
        Interval interval(1.0f, 5.0f);
        REQUIRE(interval.min == Approx(1.0f));
        REQUIRE(interval.max == Approx(5.0f));
    }

    SECTION("Contains method checks if value is within the interval") {
        Interval interval(1.0f, 5.0f);
        REQUIRE(interval.contains(3.0f) == true);
        REQUIRE(interval.contains(0.0f) == false);
        REQUIRE(interval.contains(6.0f) == false);
    }

    SECTION("Surrounds method checks if value is surrounded by the interval") {
        Interval interval(1.0f, 5.0f);
        REQUIRE(interval.surrounds(3.0f) == true);
        REQUIRE(interval.surrounds(1.0f) == false);
        REQUIRE(interval.surrounds(5.0f) == false);
    }

    SECTION("Clamp method clamps the value within the interval") {
        Interval interval(1.0f, 5.0f);
        REQUIRE(interval.clamp(0.0f) == Approx(1.0f));
        REQUIRE(interval.clamp(3.0f) == Approx(3.0f));
        REQUIRE(interval.clamp(7.0f) == Approx(5.0f));
    }

}
