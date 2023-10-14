#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/sphere.h>

int main() {
    HittableList world;
    world.add(std::make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f));

    Camera cam;
    cam.imageWidth = 400;
    cam.imageHeight = 225;
    cam.samplesPerPixel = 10;

    cam.render(world);

    return 0;
}
