#pragma once

#include "interval.h"
#include "utils.h"

inline float linearToGamma(float linearComponent) { return std::sqrt(linearComponent); }

void writeColor(uint8_t* imageData, int idx, color pixelColor, const int samplesPerPixel) {
    float r = pixelColor.r;
    float g = pixelColor.g;
    float b = pixelColor.b;

    // Divide the color by the number of samples
    if (samplesPerPixel==0){
        r = 0;
        g = 0;
        b = 0;
    }else{
        float scale = 1.0f / samplesPerPixel;
        r *= scale;
        g *= scale;
        b *= scale;
        }
    

    // Apply the linear to gamma transform
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000f, 0.999f);
    imageData[idx] = static_cast<uint8_t>(256 * intensity.clamp(r));
    imageData[idx+1] = static_cast<uint8_t>(256 * intensity.clamp(g));
    imageData[idx+2] = static_cast<uint8_t>(256 * intensity.clamp(b));
}
