#pragma once

#include "interval.h"
#include "utils.h"

void writeColor(uint8_t* imageData, int idx, color pixelColor) {
    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000f, 0.999f);
    imageData[idx] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.r));
    imageData[idx + 1] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.g));
    imageData[idx + 2] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.b));
}

void writeScanline(uint8_t* imageData, const std::vector<color>& pixels, const int startIdx) {
    int idx = startIdx;
    static const Interval intensity(0.000f, 0.999f);
    for (auto& pixelColor : pixels) {
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.r));
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.g));
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.b));
    }
}
