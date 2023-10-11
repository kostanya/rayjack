#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hit_record.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    inline void clear() { objects.clear(); }
    inline void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
};

#endif // HITTABLE_LIST_H