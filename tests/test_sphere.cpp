#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "sphere.cpp"
#include "material.cpp"

using Approx = Catch::Approx;

TEST_CASE("Sphere::hit method tests", "[Sphere]") {
    auto testMaterial = std::make_shared<Lambertian>(color (1.0, 1.0, 1.0)); // Material is chosen as Lambertian without any intention
    Sphere sphere({0, 0, 0}, 2.0f, testMaterial);

    SECTION("Ray misses the sphere") {
        Ray missRay({5, 5, 5}, {0, 0, 1});
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(missRay, rayT, record);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray intersects the sphere") {
        Ray intersectRay({0, 0, -5}, {0, 0, 1});
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(intersectRay, rayT, record);
        REQUIRE(hitResult == true);
        REQUIRE(record.frontFace == true);
    }

    SECTION("Ray originates inside the sphere") {
        Ray insideRay({0, 0, 0}, {0, 0, 1});
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(insideRay, rayT, record);
        REQUIRE(hitResult == true);
        REQUIRE(record.frontFace == false);
    }

    SECTION("Ray misses the sphere due to restricted rayT") {
        Ray restrictedRay({0, 0, -5}, {0, 0, 1});
        Interval rayT(10.0, 20.0); 
        HitRecord record;

        bool hitResult = sphere.hit(restrictedRay, rayT, record);
        REQUIRE(hitResult == false);
    }

     SECTION("Ray hits the sphere but with a negative discriminant") {
        Ray negativeDiscriminantRay({0, 0, -5}, {0, 0, -1});
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(negativeDiscriminantRay, rayT, record);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray hits the sphere tangentially") {
        Ray tangentRay({-2, -2, 0}, {0, 4, 0});
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(tangentRay, rayT, record);
        REQUIRE(hitResult == true);
        REQUIRE(record.t == Approx(0.5).margin(0.001)); 
        REQUIRE(record.frontFace == false); 
    }

    SECTION("Ray hits the sphere from inside") {
        Ray insideRay({0, 0, 0}, {1, 0, 0}); 
        Interval rayT(0.001, 1000.0);
        HitRecord record;

        bool hitResult = sphere.hit(insideRay, rayT, record);

        REQUIRE(hitResult == true);
        REQUIRE(record.t == Approx(2.0).margin(0.001)); 
        REQUIRE(record.frontFace == false); 
    }

    SECTION("Ray misses the sphere due to a very large rayT") {
        Ray missRay({0, 0, -5}, {0, 0, -1}); 
        Interval rayT(0.001, std::numeric_limits<float>::infinity()); // Large rayT range
        HitRecord record;

        bool hitResult = sphere.hit(missRay, rayT, record);
        REQUIRE(hitResult == false);
    }
}