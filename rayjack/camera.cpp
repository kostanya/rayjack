#include "camera.h"
#include "color.h"
#include "hit_record.h"

#include "stb_image_write.h"

#include <iostream>
#include <memory>

void Camera::render(const Hittable& world) {
    initialize();

    // create image data array in heap and point to it
    std::unique_ptr<uint8_t[]> imageData = std::make_unique<uint8_t[]>(imageWidth * imageHeight * 3);

    // Render
    int idx = 0;
    for (int j = 0; j < imageHeight; ++j) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            color pixelColor(0.0f, 0.0f, 0.0f);
            for (int sample = 0; sample < samplesPerPixel; ++sample) {
                Ray r = getRay(i, j);
                pixelColor += rayColor(r, maxRayBounce, world);
            }
            writeColor(imageData.get(), idx, pixelColor, samplesPerPixel);
        }
    }

    std::clog << "\rDone.\n";

    // if channel is 4, you can use alpha channel in png
    stbi_write_png("render_result.png", imageWidth, imageHeight, m_channel, imageData.get(),
                   imageWidth * m_channel);

    // You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
    // stbi_write_jpg("jpg_test.jpg", imageWidth, imageHeight, 3, imageData.get(), 100);
}

void Camera::initialize() {
    // Ensure image width and height are at least 1
    imageWidth = (imageWidth < 1) ? 1 : imageWidth;
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    m_center = point3(0.0f, 0.0f, 0.0f);

    // Determine viewport dimensions
    float focalLength = 1.0f;
    float viewportHeight = 2.0f;
    float viewportWidth = viewportHeight * imageWidth / imageHeight;

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    vec3 viewportU = vec3(viewportWidth, 0.0f, 0.0f);
    vec3 viewportV = vec3(0.0f, -viewportHeight, 0.0f);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    m_pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    m_pixelDeltaV = viewportV / static_cast<float>(imageHeight);

    // Calculate the location of the upper left pixel
    point3 viewportUpperLeft = m_center - vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
    m_startPixelLoc = viewportUpperLeft + 0.5f * (m_pixelDeltaU + m_pixelDeltaV);
}

Ray Camera::getRay(int i, int j) const {
    // Get randomly sampled camera ray for the pixel at location [i, j]
    point3 pixelCenter =
        m_startPixelLoc + (static_cast<float>(i) * m_pixelDeltaU) + (static_cast<float>(j) * m_pixelDeltaV);
    point3 pixelSample = pixelCenter + pixelSampleSquare();

    point3 rayOrigin = m_center;
    vec3 rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
}

vec3 Camera::pixelSampleSquare() const {
    // Return a random point in the square surrounding pixel at the origin
    float px = -0.5f + randomFloat();
    float py = -0.5f + randomFloat();

    return (px * m_pixelDeltaU) + (py * m_pixelDeltaV);
}

color Camera::rayColor(const Ray& r, int bounceLeft, const Hittable& world) const {
    if (bounceLeft == 0)
        return color(0.0f, 0.0f, 0.0f);

    HitRecord rec;
    if (world.hit(r, Interval(0.001, infinity), rec)) {
        vec3 direction = randomOnHemisphere(rec.normal);
        return 0.5f * rayColor(Ray(rec.p, direction), --bounceLeft, world);
    }

    vec3 unitDirection = glm::normalize(r.direction());
    float a = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
}
