#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "ray.h"
#include "utils.h"

class HitRecord {
public:
    point3 p;
    vec3 normal;
    float t;
    bool frontFace;

    // sets the hit record normal vector
    // NOTE: outwardNormal is assumed to have unit length
    inline void setFaceNormal(const Ray& r, const vec3& outwardNormal) {
        frontFace = glm::dot(r.direction(), outwardNormal) < 0.0f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

#endif // HIT_RECORD_H