#pragma once

#include "hittable.h"
#include "utils.h"

class Camera {
public:
    int imageWidth = 100;      // Rendered image width in pixels
    int imageHeight = 100;     // Rendered image height in pixels
    int samplesPerPixel = 100; // Count of random samples for each pixel
    int maxRayBounce = 10;     // Max number of ray bounces into scene

    float verticalFieldOfView = 90;              // Vertical view angle (field of view)
    point3 lookFrom = point3(0.0f, 0.0f, -1.0f); // Point camera is looking from
    point3 lookAt = point3(0.0f, 0.0f, 0.0f);    // Point camera is looking at
    vec3 viewUp = vec3(0.0f, 1.0f, 0.0f);        // Camera-relative (Global) "up" direction
    vec3 u, v, w;                                // Camera frame basis vectors

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
    color rayColor(const Ray& r, int bounceLeft, const Hittable& world) const;
};
