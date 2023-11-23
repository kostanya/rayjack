#include "render_thread.h"
#include "ray.h"
#include "hittable.h"
#include "hit_record.h"
#include "material.h"
#include "camera.h"

color rayColor(const Ray& r, int bounceLeft, const Hittable& world) {
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


color pixelColor() {
    color pixel;




    return pixel;
}



