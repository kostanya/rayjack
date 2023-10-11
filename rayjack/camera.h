#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "utils.h"

class Camera {
public:
    float aspectRatio = 1.0f;  // Ratio of image width over height
    int imageWidth = 100;      // Rendered image width in pixel count
    int samplesPerPixel = 100; // Count of random samples for each pixel

    void render(const Hittable& world);

private:
    const int m_channel = 3; // Number of channels to be rendered
    int m_imageHeight;       // Rendered image height
    point3 m_center;         // Camera center
    point3 m_startPixelLoc;  // Location of pixel [0, 0]
    vec3 m_pixelDeltaU;      // Offset to pixel to the right
    vec3 m_pixelDeltaV;      // Offset to pixel below

    void initialize();
    Ray getRay(int i, int j) const;
    vec3 pixelSampleSquare() const;
    color rayColor(const Ray& r, const Hittable& world) const;
};

#endif // CAMERA_H
