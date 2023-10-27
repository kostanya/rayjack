#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/material.h>
#include <rayjack/sphere.h>

int main() {
    HittableList world;

    auto materialGround = std::make_shared<Lambertian>(color(0.8f, 0.8f, 0.0f));
    auto materialCenter = std::make_shared<Lambertian>(color(1.0f, 0.0f, 0.0f));
    auto materialLeft = std::make_shared<Dielectric>(1.5f);
    auto materialRight = std::make_shared<Metal>(color(0.8f, 0.6f, 0.2f), 0.0f);

    world.add(std::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
    world.add(std::make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, materialCenter));
    world.add(std::make_shared<Sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
    world.add(std::make_shared<Sphere>(point3(1.5f, 0.0f, -2.0f), 0.6f, materialRight));

    Camera cam;
    cam.imageWidth = 400;
    cam.imageHeight = 225;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;

    cam.render(world);

    return 0;
}
