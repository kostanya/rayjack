#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "material.h"

TEST_CASE("Material scatter method tests", "[Material]") {
    color albedo(0.8, 0.6, 0.4);

    HitRecord rec;
    rec.p = point3(1.0, 2.0, 3.0);
    rec.normal = vec3(0.0, 1.0, 0.0);
    rec.frontFace = true;

    color attenuation(1.0, 1.0, 1.0);
    Ray scattered({1.0, 1.0, 1.0}, {1.0, 0.0, 0.0});

    SECTION("Lambertian scatter method") {
        Lambertian lambertian(albedo);

        Ray rayIn({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});

        bool scatterResult = lambertian.scatter(rayIn, rec, attenuation, scattered);
        REQUIRE(scatterResult == true);
    }

    SECTION("Metal material scattering behavior") {
        color albedo(0.7, 0.6, 0.5); 
        float fuzziness = 0.2; 
        Metal metal(albedo, fuzziness);

        Ray rayIn(rec.p, {1.0, -1.0, 1.0}); //rec.p : hit point

        bool scatteredResult = metal.scatter(rayIn, rec, attenuation, scattered);

        REQUIRE(scatteredResult == true);
        REQUIRE(attenuation == albedo); 
        float dotProduct = glm::dot(scattered.direction(), rec.normal);
        REQUIRE(dotProduct > 0.0f);
    }

    SECTION("Dielectric material scattering behavior") {
        float refractiveIndex = 1.5; 
        Dielectric dielectricMaterial(refractiveIndex);
       
        HitRecord rec;
        rec.p = point3(0.0, 0.0, 0.0);
        rec.normal = vec3(0.0, 0.0, 1.0);
        rec.frontFace = true;

        Ray rayIn(rec.p, {0.0, 0.0, -1.0});

        bool scatteredResult = dielectricMaterial.scatter(rayIn, rec, attenuation, scattered);

        REQUIRE(scatteredResult == true);
        REQUIRE(attenuation == color(1.0f, 1.0f, 1.0f)); // Attenuation should be white

        // Assuming the ray is entering from air and refracting through the material
        // checking if the scattered ray is refracted as expected
        vec3 refractedDirection = glm::normalize(scattered.direction());
        float dotProduct = glm::dot(refractedDirection, rec.normal);
        REQUIRE(dotProduct < 0.0f);
    }

}