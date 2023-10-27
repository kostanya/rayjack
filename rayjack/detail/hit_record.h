#pragma once

#include "ray.h"
#include "utils.h"

#include <memory>

// To prevent circular dependency
class Material;

class HitRecord {
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<Material> mat;
    float t;
    bool frontFace;

    // Set the hit record's normal vector
    // NOTE: outwardNormal is assumed to have unit length
    inline void setFaceNormal(const Ray& r, const vec3& outwardNormal) {
        frontFace = glm::dot(r.direction(), outwardNormal) < 0.0f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};
