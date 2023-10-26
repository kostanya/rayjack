#include "sphere.h"

bool Sphere::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
    vec3 oc = r.origin() - m_center;
    float a = glm::dot(r.direction(), r.direction());
    float half_b = glm::dot(oc, r.direction());
    float c = glm::dot(oc, oc) - m_radius * m_radius;

    float disc = half_b * half_b - a * c;
    if (disc < 0)
        return false;
    float sqrtd = std::sqrt(disc);

    // find the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if (!rayT.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!rayT.surrounds(root))
            return false;
    }

    rec.p = r.at(root);
    rec.t = root;
    vec3 outwardNormal = (rec.p - m_center) / m_radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.mat = m_mat;

    return true;
}