#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
#include "stb_image.h"
#include <vector>
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>

// Load image data into a vector
std::vector<glm::u8vec4> loadImageData(const char* imagePath, int& width, int& height) {
    int channels = 3; // RGB

    // Load the image using STB Image library
    stbi_uc* imageData = stbi_load(imagePath, &width, &height, &channels, STBI_rgb);

     if (imageData == nullptr) {
        // Failed to load the image
        const char* error = stbi_failure_reason();
        if (error) {
            std::cout << "STB Image load failed: " << error << std::endl;
        }
        width = height = 0;
        return std::vector<glm::u8vec4>(); // Return an empty vector
    }

    // Convert the image data into a vector of glm::u8vec4 for compatibility with GLM
    std::vector<glm::u8vec4> imageVector;
    imageVector.reserve(width * height);

    for (int i = 0; i < width * height; ++i) {
        glm::u8vec4 pixel;
        pixel.r = imageData[i * 3];     // R
        pixel.g = imageData[i * 3 + 1]; // G
        pixel.b = imageData[i * 3 + 2]; // B
        imageVector.push_back(pixel);
    }

    stbi_image_free(imageData); // Free the image data memory allocated by STB Image

    return imageVector;
}

// Compare two image vectors for equality
bool compareImages(const std::vector<glm::u8vec4>& image1, const std::vector<glm::u8vec4>& image2) {
    if (image1.size() != image2.size()) {
        std::cout << "Images are of different sizes " << std::endl;
        return false; 
    }

    // Compare each pixel in both images
    int threshold = 40;
    for (size_t i = 0; i < image1.size(); ++i) {
        glm::u8vec4 pixel1 = image1[i];
        glm::u8vec4 pixel2 = image2[i];

        int avg1 = (pixel1.r + pixel1.g + pixel1.b) / 3;
        int avg2 = (pixel2.r + pixel2.g + pixel2.b) / 3;
        int diff = std::abs(static_cast<int>(avg1) - static_cast<int>(avg2));
        
        if (diff > threshold) {
            std::cout << "Images differ at the pixel "<< i << std::endl;
            std::cout << "difference "<< diff << std::endl;
            return false; 
        }
    }
    return true; // Images are identical
}


TEST_CASE("Camera ", "[Camera]") {
    // test scene
    HittableList world;
    auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.5);
    auto material_right  = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(std::make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(std::make_shared<Sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // test camera
    Camera cam;
    cam.imageWidth = 400;
    cam.imageHeight = 225;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;
    cam.verticalFieldOfView = 20;
    cam.lookFrom = point3(-2, 2, 1);
    cam.lookAt = point3(0, 0, -1);
    cam.viewUp = vec3(0, 1, 0);
    cam.defocusAngle = 10.0;
    cam.focusDist = 3.4;
    cam.numThreads = 10;


    SECTION("Image comparison test after rendering") {
        int width1, height1, width2, height2;
        std::vector<glm::u8vec4> referenceImageData = loadImageData("C:/Users/Sinem/source/repos/rayjack/tests/test_image.png", width2, height2);

        cam.render(world);
        std::vector<glm::u8vec4> testImageData = loadImageData("C:/Users/Sinem/source/repos/rayjack/build/render_result_thread_deneme.png", width1, height1);
        
        // Check if images are loaded successfully
        REQUIRE_FALSE(referenceImageData.empty());
        REQUIRE_FALSE(testImageData.empty());

        // Ensure images have the same dimensions for an accurate comparison
        REQUIRE(width1 == width2);
        REQUIRE(height1 == height2);

        bool identical = compareImages(referenceImageData, testImageData);
        REQUIRE(identical);

    }

    /*SECTION("Pixel color test") {
        color pixelColor = cam.pixelColor(10, 10, world);

        REQUIRE(pixelColor.r >= 0.0f);
        REQUIRE(pixelColor.g >= 0.0f);
        REQUIRE(pixelColor.b >= 0.0f);
        
    }*/

}
