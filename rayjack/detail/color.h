#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "utils.h"

void writeColor(uint8_t* imageData, int& idx, color pixelColor, int samplesPerPixel) {
    float r = pixelColor.r;
    float g = pixelColor.g;
    float b = pixelColor.b;

    // Divide the color by the number of samples
    float scale = 1.0f / samplesPerPixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000f, 0.999f);
    imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(r));
    imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(g));
    imageData[idx++] = static_cast<uint8_t>(256 * intensity.clamp(b));
}

#endif // COLOR_H