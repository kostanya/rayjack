#include <Catch2/catch_test_macros.hpp>
#include "ray.h"

TEST_CASE("Test Ray Class") {
    SECTION("Test Ray Constructor") {
        Ray ray({0, 0, 0}, {1, 0, 0});

        REQUIRE(ray.origin() == point3(0, 0, 0));
        REQUIRE(ray.direction() == vec3(1, 0, 0));
    }

    SECTION("Test Ray 'at' method") {
        Ray ray({0, 0, 0}, {1, 0, 0});

        point3 point_at_t = ray.at(3);

        point3 expected_point = {3, 0, 0};

        REQUIRE(point_at_t == expected_point);
    }
}