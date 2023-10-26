#pragma once

#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <glm/vec3.hpp>
#include <limits>
#include <random>

using vec3 = glm::vec3;
using color = glm::vec3;
using point3 = glm::vec3;

static std::uniform_real_distribution<float> distribution_0_0__1_0(0.0, 1.0);
static std::mt19937 generator;

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.14159265f;

// Utility Functions
inline float degreesToRadians(float degrees) { return degrees * pi / 180.0f; }

// Randoms
inline float randomFloat() { return distribution_0_0__1_0(generator); }
inline float randomFloat(float min, float max) { return min + (max - min) * randomFloat(); }

inline vec3 randomVec3() { return vec3(randomFloat(), randomFloat(), randomFloat()); }
inline vec3 randomVec3(float min, float max) {
    return vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

inline vec3 randomOnUnitSphere() { return glm::sphericalRand(1.0f); } // Random unit vector
inline vec3 randomUnitVector() { return randomOnUnitSphere(); }

inline vec3 randomOnHemisphere(const vec3& normal) {
    vec3 onUnitSphere = randomOnUnitSphere();
    if (glm::dot(onUnitSphere, normal) > 0.0f) // In the same hemisphere as the normal
        return onUnitSphere;
    else
        return -onUnitSphere;
}