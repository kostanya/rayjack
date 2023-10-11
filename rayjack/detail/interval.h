#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils.h"
#include <algorithm>

class Interval {
public:
    float min, max;

    // default interval is empty
    Interval() : min(+infinity), max(-infinity) {}
    Interval(float _min, float _max) : min(_min), max(_max) {}

    inline bool contains(float x) const { return (min <= x && x <= max); }

    inline bool surrounds(float x) const { return (min < x && x < max); }

    inline float clamp(float x) const {
        return (std::min(std::max(x, min), max));
    }

    static const Interval empty, universe;
};

const static Interval empty{+infinity, -infinity};
const static Interval universe{-infinity, +infinity};

#endif