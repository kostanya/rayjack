#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/material.h>
#include <rayjack/sphere.h>

int main() {
    HittableList world;

    auto materialGround = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto materialCenter = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto materialLeft = std::make_shared<Dielectric>(1.5);
    auto materialRight = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(std::make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, materialCenter));
    world.add(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.add(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.4, materialLeft));
    world.add(std::make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, materialRight));

    Camera cam;
    cam.imageWidth = 400;
    cam.imageHeight = 225;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;
    cam.verticalFieldOfView = 90;

    cam.verticalFieldOfView = 20;
    cam.lookFrom = point3(-2, 2, 1);
    cam.lookAt = point3(0, 0, -1);
    cam.viewUp = vec3(0, 1, 0);

    cam.render(world);

    return 0;
}
