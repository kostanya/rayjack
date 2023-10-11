#ifndef HITTABLE_H
#define HITTABLE_H

#include "hit_record.h"
#include "interval.h"
#include "ray.h"

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif // HITTABLE_H