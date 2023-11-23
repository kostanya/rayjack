#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/material.h>
#include <rayjack/sphere.h>

int main() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -1; a < 1; a++) {
        for (int b = -1; b < 1; b++) {
            auto choose_mat = randomFloat();
            point3 center(a + 0.9 * randomFloat(), 0.2, b + 0.9 * randomFloat());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random3() * random3();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random3(0.5, 1);
                    auto fuzz = randomFloat(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.imageHeight = 225;
    cam.imageWidth = 450;
    cam.samplesPerPixel = 50;
    cam.maxRayBounce = 50;

    cam.verticalFieldOfView = 20.0f;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0, 0, 0);
    cam.viewUp = vec3(0, 1, 0);

    cam.defocusAngle = 0.6f;
    cam.focusDist = 10.0f;

    cam.numThreads = 2;

    cam.render(world);

    return 0;
}
