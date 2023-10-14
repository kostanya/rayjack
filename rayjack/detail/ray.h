#ifndef RAY_H
#define RAY_H

#include "utils.h"

class Ray {
private:
    point3 m_origin;
    vec3 m_direction;

public:
    Ray() {}
    Ray(const point3& origin, const vec3& direction) : m_origin(origin), m_direction(direction) {}

    inline point3 origin() const { return m_origin; }
    inline vec3 direction() const { return m_direction; }
    inline point3 at(float t) const { return m_origin + t * m_direction; }
};

#endif // RAY_H