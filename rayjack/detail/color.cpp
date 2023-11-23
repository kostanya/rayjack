#include "color.h"

// Writing calculated output scanline onto image
void writeScanline(uint8_t* imageData, const std::vector<color>& pixels, const int startIdx) {
    int idx = startIdx;
    static const Interval intensity(0.000f, 0.999f);
    for (auto& pixelColor : pixels) {
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.r));
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.g));
        imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(pixelColor.b));
    }
}