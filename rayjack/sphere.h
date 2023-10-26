#pragma once

#include "hit_record.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "utils.h"

#include <memory>

class Sphere : public Hittable {
private:
    point3 m_center;
    float m_radius;
    std::shared_ptr<Material> m_mat;

public:
    Sphere(point3 center, float radius, std::shared_ptr<Material> mat) : m_center(center), m_radius(radius), m_mat(mat) {}

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
};
