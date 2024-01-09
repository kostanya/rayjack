#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "color.h"

using Approx = Catch::Approx;

/*TEST_CASE("Write color to image data", "[writeColor]") {
    constexpr int ImageDataSize = 3; 
    uint8_t imageData[ImageDataSize] = {0};

    SECTION("Test writing color for valid input") {
        color sampleColor = {1.0f, 0.25f, 0.5f}; 
        writeColor(imageData, 0, sampleColor); 
        const Interval intensity(0.000f, 0.999f);

        REQUIRE(imageData[0] == Approx(256 * intensity.clamp(sampleColor.r)).margin(1));
        REQUIRE(imageData[1] == Approx(256 * intensity.clamp(sampleColor.g)).margin(1));
        REQUIRE(imageData[2] == Approx(256 * intensity.clamp(sampleColor.b)).margin(1));
    }
}*/

TEST_CASE("Write scanline ", "[writeScanline]") {
    constexpr int ImageDataSize = 9; 
    uint8_t imageData[ImageDataSize] = {0};

    SECTION("Test writeScanline functionality") {
        const int startIdx = 0;
        const std::vector<color> testPixels = {
            {1.0f, 0.0f, 0.0f}, // R
            {0.0f, 1.0f, 0.0f}, // G
            {0.0f, 0.0f, 1.0f}  // B
        };

        writeScanline(imageData, testPixels, startIdx);

        // Red pixel (1.0f, 0.0f, 0.0f)
        REQUIRE(imageData[0] == Approx(255)); 
        REQUIRE(imageData[1] == Approx(0));   
        REQUIRE(imageData[2] == Approx(0));   

        // Green pixel (0.0f, 1.0f, 0.0f)
        REQUIRE(imageData[3] == Approx(0));   
        REQUIRE(imageData[4] == Approx(255));
        REQUIRE(imageData[5] == Approx(0));   

        // Blue pixel (0.0f, 0.0f, 1.0f)
        REQUIRE(imageData[6] == Approx(0));  
        REQUIRE(imageData[7] == Approx(0));  
        REQUIRE(imageData[8] == Approx(255)); 
    }

}

