#pragma once

#include "hittable_list.h"
#include "utils.h"
#include <thread>

class Camera {
public:
    int imageWidth = 100;      // Rendered image width in pixels
    int imageHeight = 100;     // Rendered image height in pixels
    int samplesPerPixel = 100; // Count of random samples for each pixel
    int maxRayBounce = 10;     // Max number of ray bounces into scene
    uint32_t numThreads = std::thread::hardware_concurrency();

    float verticalFieldOfView = 90;              // Vertical view angle (field of view)
    point3 lookFrom = point3(0.0f, 0.0f, -1.0f); // Point camera is looking from
    point3 lookAt = point3(0.0f, 0.0f, 0.0f);    // Point camera is looking at
    vec3 viewUp = vec3(0.0f, 1.0f, 0.0f);        // Camera-relative (Global) "up" direction
    vec3 u, v, w;                                // Camera frame basis vectors

    float defocusAngle = 0.0f; // Variation angle of rays through each pixel
    float focusDist = 10.0f;   // Focus plane - camera center (lookFrom)

    float render(const HittableList& world);

    color pixelColor(int i, int j, const Hittable& world) const;

private:
    const int m_channel = 3; // Number of channels to be rendered
    point3 m_center;         // Camera center
    point3 m_startPixelLoc;  // Location of pixel [0, 0]
    vec3 m_pixelDeltaU;      // Offset to pixel to the right
    vec3 m_pixelDeltaV;      // Offset to pixel below
    vec3 m_defocusDiskU;     // Defocus disk horizontal radius
    vec3 m_defocusDiskV;     // Defocus disk vertical radius

    void initialize();
    Ray getRay(int i, int j) const;
    vec3 pixelSampleSquare() const;
    point3 defocusDiskSample() const;
    color rayColor(const Ray& r, int bounceLeft, const Hittable& world) const;
};
