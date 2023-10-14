#pragma once

#include "hit_record.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "utils.h"

class Sphere : public Hittable {
private:
    point3 m_center;
    float m_radius;

public:
    Sphere(point3 center, float radius) : m_center(center), m_radius(radius) {}

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
};


