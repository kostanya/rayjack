#pragma once

#include "hittable.h"
#include "utils.h"

class Camera {
public:
    int imageWidth = 100;      // Rendered image width in pixels
    int imageHeight = 100;     // Rendered image height in pixels
    int samplesPerPixel = 100; // Count of random samples for each pixel

    void render(const Hittable& world);

private:
    const int m_channel = 3; // Number of channels to be rendered
    point3 m_center;         // Camera center
    point3 m_startPixelLoc;  // Location of pixel [0, 0]
    vec3 m_pixelDeltaU;      // Offset to pixel to the right
    vec3 m_pixelDeltaV;      // Offset to pixel below

    void initialize();
    Ray getRay(int i, int j) const;
    vec3 pixelSampleSquare() const;
    color rayColor(const Ray& r, const Hittable& world) const;
};


