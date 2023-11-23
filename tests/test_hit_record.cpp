#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "hit_record.h"

using Approx = Catch::Approx;

TEST_CASE("HitRecord class tests", "[HitRecord]") {
    SECTION("Setting face normal") {
        Ray r(point3(0, 0, 0), vec3(0, 0, -1));
        vec3 outwardNormal(0, 0, 1); 

        HitRecord hitRecord;
        hitRecord.setFaceNormal(r, outwardNormal);

        REQUIRE(hitRecord.frontFace == true);
        REQUIRE(hitRecord.normal == outwardNormal); 
    }
}
