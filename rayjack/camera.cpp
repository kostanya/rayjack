#include "camera.h"
#include "color.h"
#include "hit_record.h"
#include "material.h"
#include "render_thread_pool.h"

#include "stb_image_write.h"

#include <future>
#include <iostream>
#include <memory>

float Camera::render(const HittableList& world) {
    initialize();

    // Create image data array in heap and point to it
    std::unique_ptr<uint8_t[]> imageData = std::make_unique<uint8_t[]>(imageWidth * imageHeight * 3);

    float renderTime = 0;

    // Setup the task to render
    std::future<void> task = std::async(std::launch::async, [this, &imageData, &world, &renderTime] {
        const auto startTime = std::chrono::system_clock::now();

        // Create the thread pool (and renderer)
        RenderThreadPool pool(*this, this->numThreads);
        pool.setupRender();

        // Do the render
        pool.startRender(world);
        do {

            // Accumulate the progress
            const auto progress =
                static_cast<float>(pool.pixelsCompleted()) / static_cast<float>(pool.totalPixelCount());

            //std::cout << "\r%" << 100.0f * progress << " completed" << std::flush;


            // Sleep for 50 milliseconds to give more time to the CPU to actually render
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (!pool.renderCompleted());

        // Cleanly terminate threads
        pool.terminateThreads();

        const auto endTime = std::chrono::system_clock::now();

        renderTime = static_cast<float>(
                         std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) /
                     1000.0f;

        // Grab the render and save it
        pool.retrieveRender(imageData.get());
    });

    task.wait();

    // If channel is 4, you can use alpha channel in png
    stbi_write_png("render_result_thread_deneme.png", imageWidth, imageHeight, m_channel, imageData.get(),
                   imageWidth * m_channel);    // If CHANGE the image file name, also change in mainwindow.cpp and test_camera.cpp

    //std::cout << "\rRender completed in " << renderTime << " seconds\n";
    return renderTime;


    // You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
    // stbi_write_jpg("jpg_test.jpg", imageWidth, imageHeight, 3, imageData.get(), 100);

    /*
    // Render
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        std::clog << "\rScanlines remaining: " << (imageHeight - rowIdx) << ' ' << std::flush;

        std::vector<color> pixelColors(imageWidth);
        for (int columnIdx = 0; columnIdx < imageWidth; ++columnIdx) {
            // color pixColor = pixelColor(columnIdx, rowIdx, world);
            // writeColor(imageData.get(), j * imageWidth * 3 + i * 3, pixColor);
            pixelColors[columnIdx] = pixelColor(columnIdx, rowIdx, world);
        }

        writeScanline(imageData.get(), pixelColors, 3 * rowIdx * imageWidth);
    }

    std::clog << "\rDone.\n";

    */
}

void Camera::initialize() {
    // Ensure image width and height are at least 1
    imageWidth = (imageWidth < 1) ? 1 : imageWidth;
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    m_center = lookFrom;

    // Determine viewport dimensions
    float theta = degreesToRadians(verticalFieldOfView);
    float h = std::tan(theta / 2);
    float viewportHeight = 2.0f * h * focusDist;
    float viewportWidth = viewportHeight * imageWidth / imageHeight;

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame
    w = glm::normalize(lookFrom - lookAt);
    u = glm::normalize(glm::cross(viewUp, w));
    v = glm::cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    vec3 viewportU = viewportWidth * u;
    vec3 viewportV = viewportHeight * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    m_pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    m_pixelDeltaV = viewportV / static_cast<float>(imageHeight);

    // Calculate the location of the upper left pixel
    point3 viewportUpperLeft = m_center - (w * focusDist) - viewportU / 2.0f - viewportV / 2.0f;
    m_startPixelLoc = viewportUpperLeft + 0.5f * (m_pixelDeltaU + m_pixelDeltaV);

    // Calculate the camera defocus disk basis vectors
    float defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle / 2.0f));
    m_defocusDiskU = u * defocusRadius;
    m_defocusDiskV = v * defocusRadius;
}

Ray Camera::getRay(int i, int j) const {
    // Get a randomly sampled camera ray for the pixel at location [i,j],
    // originating from the camera defocus disk
    point3 pixelCenter =
        m_startPixelLoc + (static_cast<float>(i) * m_pixelDeltaU) + (static_cast<float>(j) * m_pixelDeltaV);
    point3 pixelSample = pixelCenter + pixelSampleSquare();

    point3 rayOrigin = (defocusAngle <= 0.0f) ? m_center : defocusDiskSample();
    vec3 rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
}

vec3 Camera::pixelSampleSquare() const {
    // Return a random point in the square surrounding pixel at the origin
    float px = -0.5f + randomFloat();
    float py = -0.5f + randomFloat();

    return (px * m_pixelDeltaU) + (py * m_pixelDeltaV);
}

point3 Camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk.
    vec3 p = randomInUnitDisk();
    return m_center + (p.x * m_defocusDiskU) + (p.y * m_defocusDiskV);
}

color Camera::rayColor(const Ray& r, int bounceLeft, const Hittable& world) const {
    if (bounceLeft == 0)
        return color(0.0f, 0.0f, 0.0f);

    HitRecord rec;
    if (world.hit(r, Interval(0.001, infinity), rec)) {
        Ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, bounceLeft - 1, world);

        return color(0.0f, 0.0f, 0.0f);
    }

    // Ray did not hit anything (Ray coming into the camera comes from infinity / sky)
    vec3 unitDirection = glm::normalize(r.direction());
    float a = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

color Camera::pixelColor(int i, int j, const Hittable& world) const {
    color color(0.0f, 0.0f, 0.0f);
    for (int sample = 0; sample < samplesPerPixel; ++sample) {
        Ray r = getRay(i, j);
        color += rayColor(r, maxRayBounce, world);
    }

    // Divide the color by the number of samples
    if (samplesPerPixel == 0) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    } else {
        float scale = 1.0f / samplesPerPixel;
        color.r *= scale;
        color.g *= scale;
        color.b *= scale;
    }

    // Apply the linear to gamma transform
    color.r = std::sqrt(color.r);
    color.g = std::sqrt(color.g);
    color.b = std::sqrt(color.b);

    return color;
}
