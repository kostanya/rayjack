#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "color.h"

using Approx = Catch::Approx;

TEST_CASE("Linear to Gamma conversion", "[linearToGamma]") {
    SECTION("Test linearToGamma with positive input") {
        float result = linearToGamma(4.0f);
        REQUIRE(result == Approx(2.0f).epsilon(0.001f));
    }

    SECTION("Test linearToGamma with zero input") {
        float result = linearToGamma(0.0f);
        REQUIRE(result == Approx(0.0f).epsilon(0.001f));
    }

}

TEST_CASE("Write color to image data", "[writeColor]") {
    constexpr int ImageDataSize = 3; 
    
    uint8_t imageData[ImageDataSize] = {0};
    SECTION("Test writing color for valid input") {
        color sampleColor = {1.0f, 0.25f, 0.5f}; 
        const int samplePerPixel = 4;
        writeColor(imageData, 0, sampleColor, samplePerPixel); 
        const Interval intensity(0.000f, 0.999f);

        REQUIRE(imageData[0] == Approx(256 * intensity.clamp(std::sqrt(1.0f/samplePerPixel))).margin(1));
        REQUIRE(imageData[1] == Approx(256 * intensity.clamp(std::sqrt(0.25f/samplePerPixel))).margin(1));
        REQUIRE(imageData[2] == Approx(256 * intensity.clamp(std::sqrt(0.5f/samplePerPixel))).margin(1));
    }

    SECTION("Test writing color for zero samples per pixel") {
        color sampleColor = {1.0f, 1.0f, 1.0f}; // White color
        const int samplePerPixel = 0;
        writeColor(imageData, 0, sampleColor, samplePerPixel); 

        REQUIRE(imageData[0] == 0);
        REQUIRE(imageData[1] == 0);
        REQUIRE(imageData[2] == 0);
    }

}

