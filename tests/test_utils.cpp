#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "utils.h"

using Approx = Catch::Approx;

TEST_CASE("Degrees to Radians conversion") {
    const float pi = 3.14159265f;
    REQUIRE(degreesToRadians(0.0f) == Approx(0.0f));
    REQUIRE(degreesToRadians(180.0f) == Approx(pi));
    REQUIRE(degreesToRadians(360.0f) == Approx(2 * pi));
}

TEST_CASE("Random Float Generation") {
    float random_val = randomFloat();
    REQUIRE(random_val >= 0.0f);
    REQUIRE(random_val <= 1.0f);

    float min = -5.0f;
    float max = 5.0f;
    random_val = randomFloat(min, max);
    REQUIRE(random_val >= min);
    REQUIRE(random_val <= max);
}

TEST_CASE("Check if a vector is near zero") {
    vec3 small_vector = vec3(1e-9f, 1e-9f, 1e-9f);
    REQUIRE(isNearZero(small_vector));
    
    vec3 large_vector = vec3(0.1f, 0.2f, 0.3f);
    REQUIRE(!isNearZero(large_vector));
}

TEST_CASE("Reflection of a vector") {
    vec3 v = vec3(1.0f, 1.0f, 1.0f);
    vec3 n = vec3(0.0f, 1.0f, 0.0f);
    vec3 reflected = reflect(v, n);
    
    REQUIRE(reflected.x == Approx(1.0f));
    REQUIRE(reflected.y == Approx(-1.0f));
    REQUIRE(reflected.z == Approx(1.0f));
    REQUIRE(glm::length(reflected) == Approx(glm::length(v))); 
    REQUIRE(glm::dot(reflected, n) == Approx(-glm::dot(v, n))); // Angle between reflected and normal should be equal to angle between original and normal but in opposite direction
}

TEST_CASE("Refraction of a vector") {
    vec3 uv = vec3(1.0f, 0.0f, 0.0f);
    vec3 n = vec3(0.0f, 1.0f, 0.0f);
    float refraction_ratio = 1.5f;
    vec3 refracted = refract(uv, n, refraction_ratio);
    vec3 refracted_unit = glm::normalize(refracted);

    float sin_theta = glm::length(glm::cross(uv, n));
    float expected_sin_theta = refraction_ratio * sin_theta;
    
    REQUIRE(glm::length(refracted) > 0.0f); 
    REQUIRE(sin_theta * refraction_ratio == Approx(expected_sin_theta)); // Snell's Law

}