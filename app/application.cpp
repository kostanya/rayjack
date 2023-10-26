#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/material.h>
#include <rayjack/sphere.h>

#include <rayjack/detail/utils.h>

int main() {
    HittableList world;

    auto materialGround = std::make_shared<Lambertian>(color(0.8f, 0.8f, 0.0f));
    auto materialCenter = std::make_shared<Lambertian>(color(0.7f, 0.3f, 0.3f));
    auto materialLeft = std::make_shared<Metal>(color(0.8f, 0.8f, 0.8f));
    auto materialRight = std::make_shared<Metal>(color(0.8f, 0.6f, 0.2f));

    world.add(std::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
    world.add(std::make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, materialCenter));
    world.add(std::make_shared<Sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
    world.add(std::make_shared<Sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

    Camera cam;
    cam.imageWidth = 400;
    cam.imageHeight = 225;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;

    cam.render(world);

    return 0;
}
