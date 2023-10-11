#include "hittable_list.h"

bool HittableList::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    float closestSoFar = rayT.max;

    for (const auto& object : objects) {
        if (object->hit(r, Interval{rayT.min, closestSoFar}, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}