#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/sphere.h>

int main() {
    HittableList world;
    world.add(std::make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f));

    Camera cam;
    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;

    cam.render(world);

    return 0;
}
